#define FAZIS 36

void setup() {
  // Serial bekapcsolasa, megvarasa
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  // Init vege
  Serial.println("Initialization Finished!");
}

int val = 0;
double measuredVolt = 0.0;

void loop() {
  val = analogRead(FAZIS);
  measuredVolt = valueToVolt(val);

  /*Serial.print(val);
  Serial.print(" -> ");
  Serial.print(measuredVolt);
  Serial.print("V -> ");
  Serial.print(measuredVolt-1.5);
  Serial.print("V -> ");
  Serial.print(calcAmperfromRawVolt(measuredVolt));
  Serial.println("A");*/


  Serial.println(calcAmperfromRawVolt(measuredVolt));

  //delay(100);
}

double valueToVolt(int value){
  const double maxSample = 4096.0;
  const double maxVoltage = 3.3;

  return (double)value/maxSample*maxVoltage;
}

double calcAmperfromRawVolt(double rawVolt){
  rawVolt -= 1.5;                      // Ezzel vissza toljuk a 0-ba
  double rawAmper = rawVolt / 100.0;    // U/R, ahol R = 100 Ohm
  return rawAmper * 1000.0;             // Tekercs miatti valtoszam, 1000x kisebb aramot merunk mint eredetileg ezert szorozzuk vissza
}