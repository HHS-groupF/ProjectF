# Upload-gids — ProjectF naar de hardware

Deze gids beschrijft stap voor stap hoe je elk onderdeel van het project op de
juiste hardware zet. Werk de onderdelen bij voorkeur in de aangegeven volgorde af.

---

## 1. Overzicht: welke map hoort bij welke hardware

| Map in de repo | Draait op | Toolchain | Taal/Type |
|---|---|---|---|
| `RPI-WEMOS/` | Raspberry Pi "WEMOS" (met scherm) | Qt Creator + CMake | Qt6 GUI |
| `RPI-BUS/` | Raspberry Pi "BUS" | Qt Creator + CMake | Qt6 console |
| `Wemos-tafel/Wemos-tafel/` | Wemos D1 mini #1 (ESP8266) | Arduino IDE | C++ (.ino) |
| `Wemos-tafel/Sensor/` | Wemos D1 mini #2 (ESP8266) | Arduino IDE | C++ (.ino) |
| `Ventilator/` | STM32 (L4-serie) | STM32CubeIDE | C |
| `Lichtkran/` | (los testscript voor LED-matrix) | Arduino IDE | C++ (.ino) |

> De mappen `Socketverbinding/` en `WemosDashboard/` zijn **oudere versies** en
> worden niet meer op hardware gezet. Het actieve systeem zijn de Qt-projecten
> `RPI-WEMOS` en `RPI-BUS`.

---

## 2. Netwerk & poorten — eerst alle adressen gelijkzetten

Voordat je iets uploadt: zorg dat alle apparaten dezelfde IP-adressen en poorten
gebruiken. Dit zijn de plekken die **moeten kloppen**:

| Instelling | Bestand | Waarde (voorbeeld) |
|---|---|---|
| IP van de BUS-Pi | `RPI-WEMOS/SysteemConfig.h` → `RPI_BUS_IP` | bijv. `192.168.x.10` |
| IP van de WEMOS-Pi | `RPI-BUS/SysteemConfig.h` → `RPI_WEMOS_IP` | bijv. `192.168.x.11` |
| IP van de Heimdall-Pi (Wemos #1 + #2) | `Wemos-tafel/.../config.h` → `PI_IP_ADRES` | IP van de WEMOS-Pi |
| WiFi-naam + wachtwoord | `Wemos-tafel/.../config.h` → `WIFI_SSID` / `WIFI_PASSWORD` | jouw netwerk |
| TCP-poorten (BUS↔WEMOS) | `SysteemConfig.h` (beide) | `8080` (data), `8081` (commando's) |
| Bifrost-poort (Pi↔Wemos) | `RPI-WEMOS/SysteemConfig.h` → `POORT_BIFROST` + Wemos `config.h` → `BIFROST_POORT` | `9000` |

> **Let op:** de Qt-config staat standaard op `127.0.0.1`. Dat werkt alleen als
> alles op één machine draait. Vul de echte IP-adressen in zodra de Pi's en
> Wemos-en op het netwerk staan.

---

## 3. Voorbereiding (eenmalig)

- **Raspberry Pi's:** Raspberry Pi OS geïnstalleerd, op het netwerk, SSH of
  toetsenbord/scherm beschikbaar.
- **Qt:** Qt6 met de modules **Widgets, Charts en Network**. (Geen QtMqtt nodig —
  de Wemos-communicatie loopt nu over ons eigen Bifrost-socketprotocol.)
- **Arduino IDE:** met ESP8266-board-ondersteuning
  (Boardmanager-URL: `http://arduino.esp8266.com/stable/package_esp8266com_index.json`).
- **STM32CubeIDE** voor het STM32-deel.

---

## 4. Communicatie Pi ↔ Wemos — Bifrost (geen broker nodig)

De Wemos-en praten met het dashboard via ons eigen **Bifrost**-socketprotocol
(zie Bijlage A voor hoe het werkt). Er is **geen MQTT-broker (Mosquitto) en geen
QtMqtt** meer nodig: de RPI-WEMOS start zelf een Bifrost-server (**Heimdall**) op
poort `9000`, waarop de Wemos-en (clients, **SocketWemos**) verbinden. Je hoeft
hier dus niets te installeren — alleen ervoor zorgen dat RPI-WEMOS draait vóór de
Wemos-en (zie §10).

---

## 5. RPI-BUS (Qt6) op de BUS-Pi

1. Kopieer de map `RPI-BUS/` naar de BUS-Pi.
2. Open `RPI-BUS/CMakeLists.txt` in **Qt Creator** (of build via terminal).
3. Controleer `RPI-BUS/SysteemConfig.h`: vul `RPI_WEMOS_IP` in (IP van de WEMOS-Pi).
4. Build & run.

**Bouwen via terminal (zonder Qt Creator):**
```bash
cd RPI-BUS
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/RPI-BUS
```
Verwacht: `"[RPI-BUS] Server luistert naar commando's op poort 8081"`.

---

## 6. RPI-WEMOS (Qt6 dashboard) op de WEMOS-Pi

1. Kopieer de map `RPI-WEMOS/` naar de WEMOS-Pi.
2. Open `RPI-WEMOS/CMakeLists.txt` in **Qt Creator**.
3. Controleer `RPI-WEMOS/SysteemConfig.h`:
   - `RPI_BUS_IP` = IP van de BUS-Pi
   - `POORT_BIFROST` = `9000` (poort waarop Heimdall op de Wemos-en wacht)
4. Zorg dat de module **Charts** beschikbaar is (zie §3).
5. Build & run — het dashboard-venster verschijnt.

**Bouwen via terminal:**
```bash
cd RPI-WEMOS
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/RPI-WEMOS
```

In het logboek-venster zou moeten verschijnen:
`"Netwerk backend gestart..."` en `"Bifrost (Heimdall) luistert op poort 9000..."`

---

## 7. Wemos #1 — Wemos Tafel (ESP8266)

Je uploadt hier **één sketch: de "Wemos tafel"** (`Wemos-tafel.ino`). Belangrijk:
diezelfde sketch stuurt óók de lichtkrant (LED-matrix) aan — dat is geen aparte
upload, het zit allemaal in deze ene sketch. Je hoeft dus alleen de Wemos-tafel
code te flashen; de lichtkrant-functionaliteit komt automatisch mee.

**Map:** `Wemos-tafel/Wemos-tafel/` (open `Wemos-tafel.ino` in Arduino IDE — alle
`.h`/`.cpp` in die map, inclusief `Lichtkrant.h`/`.cpp`, openen automatisch mee
als tabbladen).

**Benodigde libraries** (Library Manager) — **geen MQTT-library (PubSubClient)
meer**; Bifrost gebruikt de kale sockets uit de ESP8266-core. De matrix-libraries
blijven nodig omdat de lichtkrant in deze sketch zit:
- `ESP8266WiFi` (komt met de ESP8266-board-package)
- `MD_Parola`
- `MD_MAX72xx`
- `SPI` (standaard)

**Aansluitingen (default in de config):**
- Tafelknop → `D1`, tafel-LED → `D2`, tafel-ID = `1` (zie `tafel_config_unit1.h`)
- LED-matrix Chip Select → `D8`, aantal modules = `4` (zie `Wemos-tafel.ino`)

**Voor het uploaden aanpassen in `config.h`:**
- `WIFI_SSID`, `WIFI_PASSWORD`
- `PI_IP_ADRES` = IP van de Pi waar Heimdall draait (de WEMOS-Pi); `BIFROST_POORT` staat al op `9000`

**Uploaden:**
1. Sluit de Wemos via USB aan.
2. Tools → Board: "LOLIN(WEMOS) D1 R2 & mini", juiste poort kiezen.
3. Upload (pijl-knop). Open Serial Monitor op **115200 baud** ter controle.

> Voor een tweede tafel-unit: gebruik `tafel_config_unit2.h` (tafel-ID 2) door die
> include bovenin `Wemos-tafel.ino` te wisselen.

---

## 8. Wemos #2 — Bewegingssensor & Sfeerlicht (ESP8266)

**Map:** `Wemos-tafel/Sensor/` (open `Sensor.ino`).

**Benodigde libraries:** alleen `ESP8266WiFi` (geen PubSubClient meer — Bifrost
gebruikt de kale core-sockets).

**Aansluitingen (`config.h`):**
- PIR-sensor → `D1`
- RGB rood → `D2`, groen → `D3`, blauw → `D5`

**Voor het uploaden aanpassen in `config.h`:** `WIFI_SSID`, `WIFI_PASSWORD`,
`PI_IP_ADRES` (IP van de Heimdall-Pi, gelijk aan Wemos #1).

**Uploaden:** zelfde stappen als §7 (board = Wemos D1 mini, 115200 baud).

---

## 9. Ventilator (STM32) via STM32CubeIDE

**Map:** `Ventilator/`.

**Hardware:** STM32 (L4-serie, gebruikt `stm32l4xx_hal.h`).
- I2C1 → SGP30 CO2-sensor (adres `0x58`) en SHT3x temp/vocht (adres `0x44`)
- USART2 → 115200 baud (debug/print)
- TIM2 → timing van de metingen
- Ventilator-uitgang → `PB0`, statusled `LD3` → `PB3`, noodstop-drukknop → `PA12`

**Stappen:**
1. Importeer het project in **STM32CubeIDE** (`File → Import → Existing Projects`),
   of voeg de `.c`/`.h`-bestanden toe aan je bestaande CubeIDE-project met de
   bijbehorende `.ioc` (I2C1, USART2, TIM2, GPIO zoals hierboven).
2. Sluit de STM32 via ST-Link/USB aan.
3. Build (hamer-icoon) → Run/Debug om te flashen.
4. Open een seriële terminal op **115200 baud**; je zou moeten zien:
   `"[INFO] Systeem succesvol opgestart in C met Hardware Timer!"`

---

## 10. Opstartvolgorde & test

Zet de onderdelen in deze volgorde aan, anders missen verbindingen elkaar:

1. **RPI-WEMOS** (Qt dashboard) — start de Bifrost-server (Heimdall) op poort 9000 en verbindt met de BUS. Moet draaien vóór de Wemos-en.
2. **RPI-BUS** (Qt) — begint sensordata te genereren en luistert op 8081.
3. **Wemos #1 en #2** — verbinden met WiFi en daarna met Heimdall (de Pi).
4. **STM32 / Ventilator** — start de klimaatregeling.

**Snelle test:**
- Druk op de tafelknop (Wemos #1) → dashboard toont "Tafel 1 vraagt hulp" + LED.
- Beweeg voor de PIR (Wemos #2) → sfeerlicht-status wordt groen op het dashboard.
- In het dashboard "Reset Tafel" / "Update Lichtkrant" / "Stel In" (RGB) testen.

---

## 11. Checklist & veelvoorkomende problemen

- [ ] Alle IP-adressen in de `config`/`SysteemConfig`-bestanden komen overeen.
- [ ] RPI-WEMOS draait (Heimdall luistert op 9000) **vóór** je de Wemos-en aanzet.
- [ ] `PI_IP_ADRES` op de Wemos-en = IP van de RPI-WEMOS-Pi; poort 9000 aan beide kanten.
- [ ] WiFi-SSID/wachtwoord kloppen (Serial Monitor toont "WiFi verbonden!").
- [ ] Qt-module Charts geïnstalleerd (anders faalt CMake bij `find_package`).
- [ ] Wemos-board en poort correct gekozen in Arduino IDE.
- [ ] STM32 seriële poort op 115200 baud.

| Probleem | Waarschijnlijke oorzaak |
|---|---|
| Dashboard-LED "Socketverbinding" blijft rood | RPI-BUS draait niet of verkeerd IP in `SysteemConfig.h` |
| Wemos meldt `[Bifrost] verbinden met Heimdall mislukt` | RPI-WEMOS draait niet, of `PI_IP_ADRES`/`BIFROST_POORT` klopt niet |
| Wemos blijft op "." in Serial Monitor | WiFi-SSID/wachtwoord fout |
| Tafelknop/RGB doet niets op het dashboard | Wemos niet verbonden met Heimdall (check serial: "verbonden met Heimdall") |
| Geen sensordata in grafieken | RPI-BUS niet verbonden met WEMOS poort 8080 |

---

## Bijlage A — Hoe het Bifrost-protocol werkt (kort)

Bifrost is ons eigen, minimale protocol over **rauwe TCP/IP-sockets** (geen MQTT,
geen libraries). Het werkt identiek over kabel (RJ45) en WiFi.

**Rollen**
- **Heimdall** = de server. Draait op de RPI-WEMOS-Pi, luistert op poort `9000`
  (op alle netwerkkaarten, dus zowel eth0/RJ45 als WiFi).
- **SocketWemos** (Valkyrie-rol) = de client. Draait op elke Wemos en verbindt
  naar Heimdall.

**Bericht = een "Rune": één tekstregel**, afgesloten met een newline:
```
topic<spatie>payload\n
```
Voorbeelden:
```
tafel/1/status HELP          (Wemos → Pi: tafel 1 vraagt hulp)
tafel/1/reset RESET          (Pi → Wemos: zet tafel 1 terug)
sensor/beweging JA           (Wemos → Pi: beweging gedetecteerd)
sensor/rgb/set 255,120,20    (Pi → Wemos: zet sfeerlicht warm-oranje)
wemos/lichtkrant MSG:Welkom  (Pi → Wemos: tekst op de lichtkrant)
```
De ontvanger splitst elke regel op de **eerste spatie**: alles ervoor is het
`topic`, alles erna de `payload` (mag spaties bevatten).

**Routing (bewust simpel gehouden)**
- Heimdall stuurt elk uitgaand bericht naar **alle** verbonden Wemos-en (broadcast);
  elke Wemos kijkt zelf of het topic voor hem bedoeld is en negeert de rest.
- Inkomende berichten van een Wemos gaan naar het dashboard.

Dit gedraagt zich als MQTT's publish/subscribe, maar met eigen code en zonder
broker. De BUS↔WEMOS-verbinding (zie Bijlage B) staat hier los van.

---

## Bijlage B — De twee socketverbindingen (allebei eigen implementatie)

Het hele systeem gebruikt **geen kant-en-klare netwerk-libraries** (geen MQTT,
geen `QTcpSocket`). Beide verbindingen zijn met **rauwe BSD-sockets**
(`socket()/bind()/listen()/accept()/connect()/send()/recv()`) zelf geschreven:

| Verbinding | Klassen | Poort(en) | Berichtformaat |
|---|---|---|---|
| **Pi ↔ Wemos** | `Heimdall` (server, Pi) ↔ `SocketWemos` (client, ESP8266) | `9000` | Bifrost-Runes: `topic payload\n` (Bijlage A) |
| **Pi ↔ Pi** (BUS ↔ WEMOS) | `SocketCommunicatieRPIBUS` ↔ `SocketCommunicatieRPIWEMOS` | `8080` (sensordata), `8081` (commando's) | JSON-regels, bv. `{"type":"sensor","sensorId":"CO2","waarde":750}` |

- Op de Pi's gebruiken we POSIX-sockets; op de ESP8266 de lwIP-BSD-sockets uit
  de board-core. In beide gevallen draait het lezen in een eigen achtergrond-thread.
- Alles loopt over TCP/IP en werkt daardoor identiek over kabel (RJ45) en WiFi.
