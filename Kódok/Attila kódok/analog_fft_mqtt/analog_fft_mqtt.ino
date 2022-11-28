#include <WiFi.h>              //built in library to connect via Wifi
#include <PubSubClient.h>      //MQTT lib (https://github.com/knolleary/pubsubclient)
#include <arduinoFFT.h>        //FFT lib (https://github.com/kosme/arduinoFFT) 

//######################################|MEASURMENT SETTINGS|####################################################################################

//-------------------------------------------|Can be setted|-------------------------------------------------------------------------------------
#define SAMPLE_QUANTITY 4096            //sample quantity, must be power of 2
#define RESULT_QUANTITY 5               //SET: how many of the highest peaks from fft result you want
#define MICRO_PIN 36                    //SET: to the pin number where the microfon analog input is

//------------------------------------------|Should not change|----------------------------------------------------------------------------------
double samples_Re[SAMPLE_QUANTITY];     // collected data real part by the sensor 
double samples_Im[SAMPLE_QUANTITY];     // collected data imaginary part by the sensor 
int results_Hz[RESULT_QUANTITY];        // will contain the major herz peaks from fft
double results_Ampl[RESULT_QUANTITY];   // will contain the major herz amplitudos from fft
int nextSapleIndex = 0;                 // assistant: where to put the next collected sensor data
unsigned long beforeCollection;         // assistant: for calculate the collection time
unsigned long beforeFFT;                // assistant: for caculate the fft running time

arduinoFFT FFT = arduinoFFT();          // FTT set up
//##############################################################################################################################################


//######################################|MQTT CONNECTION SET|###################################################################################

//--------------------------------------|Can be setted|-----------------------------------------------------------------------------------------
#define MIC_TOPIC "topic/microphon"     //SET: to the topic where MQTT server will publish

IPAddress mqttServer(172,22,0,79);    //SET: to the mqtt server ip address
const char* SSID = "I40TK-office";             //SET: to the wifi name
const char* PWD = "Ipar4Ir0d4";         //SET: to the wifi password

//--------------------------------------|Should not change|-------------------------------------------------------------------------------------
#define MQTT_PORT 1883                   // will need for MQTT connection port set 

String msg_out;                          // for publishing data via MQTT
WiFiClient espClient;                   // for Wifi set up
PubSubClient client(espClient);         // for MQTT connection set up
//##############################################################################################################################################


void setup() {
  Serial.begin(115200);

  while(!Serial);
   delay(20);

  //Wifi connection set up
  connectToWiFi();

  //MQTT connection set up
  client.setServer(mqttServer, MQTT_PORT);

  //set for next collecting
  beforeCollection = micros();
}

void loop() {
  //checking for Wifi connection loss
  WiFi.mode(WIFI_STA);
  if (!client.connected())
    reconnect();

  //checking for MQTT connection loss
  client.loop();

  //data collection or fft calculation and result send
  fft();
}

//#############################################|Assistant functions for data process|############################################################

void fft(){
  //collecting datas
  if(nextSapleIndex < SAMPLE_QUANTITY){
    samples_Re[nextSapleIndex] = analogRead(MICRO_PIN);
    samples_Im[nextSapleIndex++] = 0.0;
  }
  //process datas
  else{
    unsigned long CollectingTime = micros()-beforeCollection;
    double CollectingTimeD = CollectingTime;

    Serial.println("***COLLECTING TIME*** " + String(CollectingTime));
    beforeFFT = micros(); //just good to know the fft calculation time

    //FFT calculation
    FFT.Windowing(samples_Re, SAMPLE_QUANTITY, FFT_WIN_TYP_RECTANGLE, FFT_FORWARD);
    FFT.Compute(samples_Re, samples_Im, SAMPLE_QUANTITY, FFT_FORWARD);
    FFT.ComplexToMagnitude(samples_Re, samples_Im, SAMPLE_QUANTITY);


    unsigned long FFTime = micros()-beforeFFT;
    Serial.println("***FFT ALGORTIHM TIME*** " + String(FFTime));

    //sampling frequency calculation
    double CollectTimeInSec = CollectingTimeD / 1000000.0;  // convert to secundum
    double sampling_feq = 4096.0 / CollectTimeInSec;           // result is around 5300Hz

    // collect the highest peaks
    MajorPeaks(samples_Re, SAMPLE_QUANTITY, sampling_feq, results_Hz);

    //sending to MQTT server
    sendMajorPeaks();

    //set up for the next collecting
    nextSapleIndex = 0;
    beforeCollection = micros();
  }
}

void sendMajorPeaks(){
  //sending peaks for mqtt server
  for(int i = 0; i < RESULT_QUANTITY; i++){
    //set up the message
    if(i == 0){
      msg_out = "[" + String(results_Hz[i]) + ":" + String(results_Ampl[i]) + ", ";
    }
    else if(i == RESULT_QUANTITY-1 && RESULT_QUANTITY != 1){
      msg_out = msg_out + String(results_Hz[i]) + ":" + String(results_Ampl[i]) + "]";
    }
    else{
      msg_out = msg_out + String(results_Hz[i]) + ":" + String(results_Ampl[i]) + ", ";
    }
  }

  Serial.println(msg_out);
  //sending the message
  if (! client.publish(MIC_TOPIC, msg_out.c_str())) {
    Serial.println("Failed");
  } else {
    Serial.println("OK!");
  }
}

//-----------------------------------------rework of the arduinoFFT.cpp MajorPeak function-------------------------------------------------------
//from collecting only the major peak to collect given number major peaks
void MajorPeaks(double *vD, uint16_t samples, double samplingFrequency, int *peaks)
{
  int peaksYIndex[RESULT_QUANTITY];
  for(int k = 0; k < RESULT_QUANTITY; k++){
    peaksYIndex[k] = -1;
    peaks[k] = 0;
    results_Ampl[k] = 0;
  }

	for(int ind = 0; ind < RESULT_QUANTITY; ind++){
    double maxY = 0;
    uint16_t IndexOfMaxY = 0;
    //If sampling_frequency = 2 * max_frequency in signal,
    //value would be stored at position samples/2
    for (uint16_t i = 1; i < ((samples >> 1) + 1); i++) {
      if ((vD[i-1] < vD[i]) && (vD[i] > vD[i+1])) {

        bool has_been = false;
        for(int pi = 0; pi < RESULT_QUANTITY; pi++)
          if(i == peaksYIndex[pi])
            has_been = true;

        if (vD[i] > maxY && !has_been) {
          maxY = vD[i];
          IndexOfMaxY = i;
        }
      }
    }
    double delta = 0.5 * ((vD[IndexOfMaxY-1] - vD[IndexOfMaxY+1]) / (vD[IndexOfMaxY-1] - (2.0 * vD[IndexOfMaxY]) + vD[IndexOfMaxY+1]));
    double interpolatedX = ((IndexOfMaxY + delta)  * samplingFrequency) / (samples-1);
    if(IndexOfMaxY==(samples >> 1)) //To improve calculation on edge values
      interpolatedX = ((IndexOfMaxY + delta)  * samplingFrequency) / (samples);
    // returned value: interpolated frequency peak apex
    peaksYIndex[ind] = IndexOfMaxY;
    peaks[ind] = interpolatedX;
    results_Ampl[ind] = vD[IndexOfMaxY] / SAMPLE_QUANTITY;
  }
}
//################################################################################################################################################

//##########################################|Assistant functions for Wifi and MQTT connection|####################################################

void connectToWiFi() {
  Serial.print("Connecting to ");

  WiFi.begin(SSID, PWD);
  Serial.println(SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("Connected.");

  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    // Attempt to connect
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe(MIC_TOPIC);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      Serial.println(WiFi.localIP());
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

//giving orders via mqtt -> does not need now
void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
}
//################################################################################################################################################
