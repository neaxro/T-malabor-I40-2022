# Témalabor-I40-2022

## Bevezető

## Wiring Diagram

TODO: Milyen összeköttetések vannak, színek szerint, stb...

## Szenzorok

### Példa szenzor

**Library link ha kell hozzá:** *link helye*

#### Előnyök

- Valami

#### Hátrányok

- Valami

#### Tapasztalat

- Egyszerű volt használni

### ESP-k

#### ESP-32

Librarby hozzá

EN/BOOT button

Arduino IDE board config leírása

#### ESP-8266

Librarby hozzá

EN/BOOT button

Arduino IDE board config leírása

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

