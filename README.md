# Témalabor-I40-2022

## Bevezető

**Feladatot elvégezték**: Nemes Attila, Nemes Axel Roland

**Konzulens**: Kovács László

## Wiring Diagram

TODO: Milyen összeköttetések vannak, színek szerint, stb...

## Szenzorok

### MLX90614

Használt könyvtár: [Adafruit_MLX90614](https://github.com/adafruit/Adafruit-MLX90614-Library)

[Dokumentáció](http://adafruit.github.io/Adafruit-MLX90614-Library/html/class_adafruit___m_l_x90614.html) hozzá.

#### Előnyök

- Kontaktmentes hőmérséklte mérés
- Celsiust és Fahrenheitot is képes mérni
- Kifejezetten pontos

#### Hátrányok

- Nem tapasztaltam

#### Tapasztalat

- Egyszerű volt használni a könyvtárral.

### Fordulatszám mérő (Opto kapu)

#### Előnyök

- Nem kellett külön könyvtárat használni hozzá
- Interrupt függvénnyel megbízható, helyes eredményt ad

#### Hátrányok

- Nem tapasztaltam

#### Megjegyzés

Érdemes interrupt megoldással figyelni azt, hogy jelez-e a szenzor így nem/nehezen lehet lemaradni egy-egy jelzésről.

### Árammérő

Házilag készített mérő berendezés. Mind a három fázis mérésére alkalmas.

![Árammérő kapcsolási rajza](https://github.com/neaxro/T-malabor-I40-2022/blob/main/K%C3%A9pek/arammero_diagram.png)

A képen látható **mérhető feszültség**ből 3 darab van, minden fázisnak 1-1 db. Ezeket anaolg lehet olvasni, hogy mekkora feszültség van rajta és abból könnyen vissza lehet számolni. Az 1.65V-os kimenet nem mindig pontosan ennyi (nekem kb. 1.49V-1.55V között mozgott). Ezt is érdemes mérni és ezzel korrigálni a mérést.

A fentebb látható diagrammoknál lehet részletesebben látni, hogy melyik színű kábel pontosan mit is jelent.

### ESP-k

#### ESP-32

Librarby hozzá

EN/BOOT button

Arduino IDE board config leírása

#### ESP-8266

Librarby hozzá

EN/BOOT button

Arduino IDE board config leírása

## Egyéb könyvtárak

### MQTT Kliens

Használt könytár: [EspMQTTClient](https://github.com/plapointe6/EspMQTTClient)
Az **Axi** féle kódokban ez volt használva. Könnyű a használata, hatékony és eléggé jó dokumentáció tartozik hozzá.

### Szenzorokkal egyszerű kommunikáció (I2C)

Használt könyvtár: [Wire](https://www.arduino.cc/reference/en/language/functions/communication/wire/)
Néhány esetben ez könnyebé tette egy-egy szenzor elérését I2C-n keresztül.

## Attila Tapasztalatok

Ide jöhet szerintem a rizsa, amit ha akarnak elolvasnak. Lehet napló szerűség is.
MI jött be, mi nem. Mivel foglalkoztál és mit éreztél hasznosnak...

## Axi Tapasztalatok

Feladatom az árammérő, hőmérséklet mérő és a fordulatszám mérő szenzorok elkészítése volt. Mivel a hardver komponensek készen voltak így nekem csak a kódott kellett megírni. Nehézségek voltak, hogy nem volt pontosan ledokumentálva, hogy pontosan milyen topológia szerint vannak összekötve a szenzorok. Viszont miután végig követtem a kábeleket onnantól fogva egyszerű volt a történet.

Minden szenzornál más és más volt a bökkenő, hiszen semmi sem ment egyből tökéletesen természetesen... Még mielőtt neki kezdtem volna az egyes méréseknek először az alap dolgokkal foglalkoztam kicsit. Így az első 1-2 hétben csak ismerkedtem a könyvtárakkal és főleg az MQTT-vel.

**Javaslatom** az, hogy csak egy-egy dologgal ismerkedj. Próbáld ki csak magában az MQTT klien-szerver kapcsolatot, próbálj ki több könyvtárat és válaszd ki ami a legszimőatikusabb. Én is így tettem és ha egyszer az elején csinálsz egy jó alapot onnantól fogva a későbbiek során sokkal egyszerűbb dolgod lesz.

### MQTT kapcsolat

Mivel a projekt alapja volt egy megbízható és hatékony kommunikációs protokoll ezért ezt próbáltam meg minél jobban megismerni. A projekt alatt MQTT-t használtunk ami szerintem kifejezetten hatékony ahhoz képest, hogy milyen egyszerű a használata.

#### MQTT Kliens

Több könytár kipróbálása után én [ezt](https://github.com/plapointe6/EspMQTTClient) a könytárat választottam. Elfogadható dokumentáció és példa volt hozzá ahhoz, hogy ki tudjak belőle indulni. Ezt használtam a többi szenzornál is amit én csináltam.

Amikor kiismertem a könyvtárat alőre csináltam egy sablonos MQTT kliens kódot amit már csak fel kellett használnom a későbbi szenzoroknál és így akkor már nem kellett az MQTT kliens kóddal többet foglalkozni.

#### MQTT Szerver

Itt történt meg az első zsákutca a projekt során. Először a **Beywise** szervert használtam főleg azért, mert sok fórumon top 3 legjobb MQTT szerver között volt illetve, mert ingyenes. Sajnos már az ismerkedés elején kibúj a szög a zsákból. Borzalmas volt az egész. Nem volt hatékony, lassú és folyamatosan lefagyott. Hostol a szerver egy webes felületet ahol lehet kezelni az egész szervert ami kecsegtetően hagzik de borzalmas az egész. Egy óra alatt nem bírtam csinálni egy olyan diagramot amin 3 adatot egyszerre meg tudtam volna jeleníteni (pedig volt rá lehetőség, tálcán kínálta a UI...) csak éppen működni nem akart szegény. Ezért **NEM AJÁNLOM A HASZNÁLATÁT**.

Másik nekifutásra a konzulensünk által javasolt **mosquitto MQTT szervert** használtuk. Ez nagyon megbízható volt, gyors és robosztus meglátásom szerint. Egy hátránya volt a Beywise-hoz képest, hogy nem járt hozzá webes megjelenítő így külön kliens programot kellett használni, hogy áttekinthetően lehessen látni a topic-okat és az adatokat.

##### TODO: szerver telepítése, elindítás, stb...

### Fordulatszám mérő

Első nehézség abban volt hogy az előre elkészített fordulatszám mérő berendezés előre be volt kötve és nem volt ledokumentálva, hogy melyik ESP mely vezetékeken kommunikál az opto kapuval. Viszont miután megtaláltam onnantól fogva egyszerűbbé vált minden.

A mérést egy opto kapu segítségével oldottam meg. Ami nagyjából így néz ki.

![Átlagos optokapu](https://github.com/neaxro/T-malabor-I40-2022/blob/main/K%C3%A9pek/optokapu.jpg)

Működési elve, hogy a rés között fény halad. Egyik irányból kibocsátja, másik irányból érzékeli azt. Feszültség változással tudja jelezni, azt ha képes mérni a fényt azaz nem gátolja semmi a fény terjedését illetve azt is ha valami gátolja a terjedést. Ehhez egy tárcsát használtam amin előre meghatározva, egyenlő távolságban 6 darab lyuk van rajta. Ezeknél a lyukaknál fog érzékelni a kapu. Két érzékelési idő közötti idővel pedig vissza lehet számolni a sebességet azaz az RPM-et amire kíváncsiak vagyunk.

A számolást először folyamatos átlagolással csináltam meg. Az arról szólt, hogy egy kör alatt számolt lyukak közötti időt átlagoltam így kaptam egy átlagos körülfordulási időt, majd ebből számoltam ki az RPM-et. Ezzel az volt a baj, hogy túlságosan finom jelet kaptam így a nagyobb kilengéseket, eltéréseket nehezen (vagy egyáltalán nem) lehetett mérni.

Második nekifutásra ablakos módszerrel folytattam. Egy 6 elemű többe írtam mindig a mért lyuk különbséget és az egész tömbböt átlagolat. Persze minden egyes tömbbe írás után a mutató értékét növeltem, ha pedig túl csordult volna akkor a tömb elejére raktam. (Hasonló mint OPre-ben az órás megoldás memórai kezelésnél). Ezzel a megoldással ugyan finom jelet kaptunk viszont az ingadozásokat is jól lehetett mérni vele.

#### Interrupt

Ennél a feladatnál nagyon fotnos, hogy interrupt függvény segítségével oldjuk meg. Én is így tettem. Ennek a lényege, hogy amikor a lábon felfelé/lefelé futó él jelenik meg vagy él változás ([beállítás függő](https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/)) akkor a megadott függvény le fog futni. Így nem fogunk lemaradni egy mérésről sem.

#### Lecsengés

Mivel a megldás alapja egy interrupt függvény használata ezért el lehet gondolkodni mi van akkor ha leáll az eszterga? Mivel nem fog forogni a kerék így nem fog interrupt generálódni és így nem fog frissülni a tömbb ezért úgymond be fog fagyni/akadni a tömb tartalma. Így nem fog lenullázódni és leállni.

Ennek orvoslása érdekében folyamatosan figyelem, hogy bekövetkezik-e ez a "befagyás" jelenség. Ha igen akkor nullázom a tömböt és a jelet kétféleképpen nullázom:

- *FINOM_EJTES = true*
  Ekkor a jel fokozatosan fog lecsengeni ezzel "szimulálva" a motor leállását hiszen az is szépen lassan fog leállni.
- *FINOM_EJTES = false*
  Ekkor az RPM értékét egyből lenullázza.

Ez persze kis módosítás vagy extra de legalább ezzel lehet állítani a preferált módot ha erre később szükség lenne.

### Hőmérő

Ez egyszerű mérésnek tűnt és az is volt. Ebben az egyetlen nehézség az az volt, hogy megkeressem mi is az adott szenzor pontos neve. Mivel nem volt ledokumentálva előző csapatok által és még a szenzoron sem volt jelezve a neve ezért kutakodni kellett. Szerencsére Bixby Vision-el sikerült megtalálni a szenzort és egy nagyon egyszerű és kiváló könyvtárat. A könytárhoz dokumentácó [itt](http://adafruit.github.io/Adafruit-MLX90614-Library/html/class_adafruit___m_l_x90614.html) is elérhető. Ezzel kényelmesen lehet nem csak Celsiusban, hanem Farenheitban mérni is.

Végül már csak annyi dolgom volt, hogy MQTT szervernek elküdljem a mért adatokat.

Érdekessége ennek a mérőnek ami nagyon meglepett, hogy érintkezés nélkül eléggé pontosan mér kifejezetten gyorsan.

### Árammérő

Ez volt a legizgalmasabb mérésem a projekt alatt, nagyon élveztem elkészíteni. Maga az elektronika készen volt viszont ahhoz, hogy ezt meg tudhassam csinálni meg kellett értenem. Ezután pedig kezdtem is neki a mérés elkészítésének. Mind a három fázist kellett mérni amit a mért 0-3.3V feszültségből lehetett vissza számolni. Ehhez analóg olvasást kellett végre hajtani de szerencsére az ESP32-n rengeteg analóg láb van.

A programból is látszódik, hogy vissza kellett számolni a feszültséget mivel az analóg olvasás 0-4096 tartományban mér. Ezután használni kellett az Ohm törvényt meg figyelembe kellett venni a tekercs általi szorzót. Miután ezeket elkészítettem vízmelegítővel és hajszárítóval ki is próbáltam.

#### Adatok gyűjtése

Egyszerűnek hangzik de sajnos mégsem az. A baj az, hogy a mért adatokat amiket a soros portról olvasunk azokat kéne átmésolni excel-ben, hogy maradandó grafikont készíthessünk. (Persze lehetne soros plotterről is de az nagyon kiszámíthatatlanul képes ugrálni).
Arduino IDE-ben nem lehet soros monitorról rendesen másolni így más megoldást kellett választanom. A [Device Monitoring Studio](https://hhdsoftwaredocs.online/dms/introduction/overview.html)t választottam. Ingyenes viszont cserébe 10 napos próba verziót kapunk. Erre a kis, gyors mérésre viszont ez tökéletes. (Az arduino értékeit legegyszerűbben a **Console View**/**Consol View (legacy)** típusú monitoring móddal lehet kimásolni).

Miután mértem adatokat a következő grafikonok születtek meg:

![Árammérő első mérés](https://github.com/neaxro/T-malabor-I40-2022/blob/main/K%C3%A9pek/arammero_1.png)

Ez a legelső, kezdeti mérés volt még csak egy fázisra de már itt is szépen kirajzolódik a szinusz görbe.

![Árammérő második mérés](https://github.com/neaxro/T-malabor-I40-2022/blob/main/K%C3%A9pek/arammero_2.png)

Ez a hajszárító mérése ugyan úgy csak egy fázis lett mérve. Kicsit csúnya a jel de ez a Device Monitoring Studio hibája. Sajnos nem megbízható eszköz...

![Árammérő harmadik mérés](https://github.com/neaxro/T-malabor-I40-2022/blob/main/K%C3%A9pek/arammero_3.png)

Ez pedig a végleges mérés amit egy vízforralón végeztem el, most már mind a három fázisra. Szépen látható ahogy két fázis (kék, zöld) egymást váltják míg a harmadik, sárga színű fázis nulla közelben marad hiszen az a föld.

Ez a mérés tetszett a legjobban mert kevés baj volt vele és mert itt tanultam a legtöbbet az áramméréssel kapcsolatban. Nem mellesleg nagyon szép grafikonokat lehetett készíteni.

