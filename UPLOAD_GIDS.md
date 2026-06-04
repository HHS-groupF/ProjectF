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

> De oude MQTT-/experiment-mappen (`WemosDashboard/`, `Socketverbinding/`,
> `Lichtkran/`) zijn verwijderd. Het actieve systeem bestaat uit de vier mappen
> hierboven; de lichtkrant zit nu in de Wemos-tafel sketch.

**Dataketen in het kort** (volledig schema in Bijlage B):
```
STM32 ──CAN──► RPI-BUS ──TCP 8080/8081──► RPI-WEMOS ──Bifrost 9000──► 2× Wemos
```

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
| CAN-interface (STM32↔BUS) | `RPI-BUS/main.cpp` → `canBus.start("can0")` | `can0`, 500 kbit/s |

> **Let op:** de Qt-config staat standaard op `127.0.0.1`. Dat werkt alleen als
> alles op één machine draait. Vul de echte IP-adressen in zodra de Pi's en
> Wemos-en op het netwerk staan.

---

## 3. Voorbereiding (eenmalig)

- **Raspberry Pi's:** Raspberry Pi OS geïnstalleerd, op het netwerk, SSH of
  toetsenbord/scherm beschikbaar.
- **Qt:** Qt6 met **Widgets + Charts** (voor RPI-WEMOS) en **SerialBus** (voor de
  CAN-bus in RPI-BUS). Geen QtMqtt of QtNetwork nodig — alle netwerkcommunicatie
  loopt over onze eigen sockets. SerialBus installeer je op de Pi met
  `sudo apt install qt6-serialbus-dev`.
- **CAN-bus op de BUS-Pi:** een CAN-controller (bijv. MCP2515 via SPI) als
  SocketCAN-interface `can0`. Zet die omhoog vóór je RPI-BUS start:
  ```bash
  sudo ip link set can0 up type can bitrate 500000
  ```
  (500 kbit/s = dezelfde snelheid als de STM32, zie §9). Check met `ip -details link show can0`.
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

RPI-BUS is de schakel tussen de STM32 (CAN) en het dashboard (TCP). Hij leest de
sensordata van de STM32 over `can0` en stuurt die als eigen tekstregels
(`SENSOR …` / `STATUS …`, geen JSON-library) door naar RPI-WEMOS.

1. Kopieer de map `RPI-BUS/` naar de BUS-Pi.
2. Zorg dat `can0` omhoog staat (zie §3) — anders verbindt de CAN-laag niet.
3. Open `RPI-BUS/CMakeLists.txt` in **Qt Creator** (of build via terminal).
4. Controleer `RPI-BUS/SysteemConfig.h`: vul `RPI_WEMOS_IP` in (IP van de WEMOS-Pi).
5. Build & run.

**Bouwen via terminal (zonder Qt Creator):**
```bash
cd RPI-BUS
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/RPI-BUS
```
Verwacht in de console:
- `"[RPI-BUS] Server luistert naar commando's op poort 8081"`
- `"[CAN-BUS] Succesvol verbonden met can0"`
- bij inkomende STM32-frames: `"[CAN-BUS] Geparseerde Data - ... | Type: CO2 | Waarde: ..."`

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
meer**; Bifrost draait op `WiFiClient` uit de ESP8266-core. De matrix-libraries
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
draait op `WiFiClient` uit de core).

**Aansluitingen (`config.h`):**
- PIR-sensor → `D1`
- RGB rood → `D2`, groen → `D3`, blauw → `D5`

**Voor het uploaden aanpassen in `config.h`:** `WIFI_SSID`, `WIFI_PASSWORD`,
`PI_IP_ADRES` (IP van de Heimdall-Pi, gelijk aan Wemos #1).

**Uploaden:** zelfde stappen als §7 (board = Wemos D1 mini, 115200 baud).

---

## 9. Ventilator (STM32) via STM32CubeIDE

**Map:** `Ventilator/`.

De STM32 leest de klimaatsensoren via I²C en stuurt de waarden over de **CAN-bus**
naar RPI-BUS.

**Hardware:** STM32 (L4-serie, gebruikt `stm32l4xx_hal.h`).
- I2C1 → SGP30 CO2-sensor (adres `0x58`) en SHT3x temp/vocht (adres `0x44`)
- CAN1 → de CAN-bus; **een CAN-transceiver nodig** (bijv. SN65HVD230 of MCP2551)
  tussen `CAN1_TX`/`CAN1_RX` en de bus. Bitrate **500 kbit/s**.
- USART2 → 115200 baud (debug/print)
- TIM2 → timing van de I²C-metingen
- Ventilator-uitgang → `PB0`, statusled `LD3` → `PB3`, noodstop-drukknop → `PA12`

**Wat er over CAN gaat (STM32 → Pi):** per sensor één CAN-ID met 4 databytes
(little-endian 32-bit float):
- `0x100` = CO2 · `0x101` = temperatuur · `0x102` = luchtvochtigheid

**Stappen:**
1. Importeer het project in **STM32CubeIDE** met de bijbehorende `.ioc`
   (I2C1, CAN1, USART2, TIM2, GPIO zoals hierboven).
2. Sluit de STM32 via ST-Link/USB aan en koppel de CAN-transceiver aan dezelfde
   bus als de BUS-Pi (gemeenschappelijke GND, 120 Ω terminering aan beide uiteinden).
3. Build → Run/Debug om te flashen.
4. Seriële terminal op **115200 baud**; verwacht:
   `"[INFO] STM32 CAN-Sensor Node opgestart (TX & RX Actief)!"` en daarna
   `"[TX] Verzenden -> CO2: ... | Temp: ... | Vocht: ..."`

---

## 10. Opstartvolgorde & test

Zet de onderdelen in deze volgorde aan, anders missen verbindingen elkaar:

0. **CAN-interface `can0`** op de BUS-Pi omhoog zetten (zie §3).
1. **RPI-WEMOS** (Qt dashboard) — start Heimdall (poort 9000) en luistert op 8080. Moet draaien vóór de Wemos-en.
2. **RPI-BUS** (Qt) — verbindt met `can0` + RPI-WEMOS; leest STM32-data en stuurt die door, luistert op 8081 voor commando's.
3. **STM32 / Ventilator** — meet de sensoren en stuurt de waarden over CAN.
4. **Wemos #1 en #2** — verbinden met WiFi en daarna met Heimdall (de Pi).

**Snelle test:**
- STM32 aan → de grafieken (CO2/temp/vocht) op het dashboard beginnen te lopen (data via CAN → RPI-BUS → socket).
- Druk op de tafelknop (Wemos #1) → dashboard toont "Tafel 1 vraagt hulp" + LED.
- Beweeg voor de PIR (Wemos #2) → sfeerlicht-status wordt groen op het dashboard.
- In het dashboard "Reset Tafel" / "Update Lichtkrant" / "Stel In" (RGB) testen.

---

## 11. Checklist & veelvoorkomende problemen

- [ ] Alle IP-adressen in de `config`/`SysteemConfig`-bestanden komen overeen.
- [ ] RPI-WEMOS draait (Heimdall luistert op 9000) **vóór** je de Wemos-en aanzet.
- [ ] `PI_IP_ADRES` op de Wemos-en = IP van de RPI-WEMOS-Pi; poort 9000 aan beide kanten.
- [ ] WiFi-SSID/wachtwoord kloppen (Serial Monitor toont "WiFi verbonden!").
- [ ] Qt-modules Charts (RPI-WEMOS) + SerialBus (RPI-BUS) geïnstalleerd.
- [ ] `can0` staat omhoog op 500 kbit/s (`ip link show can0`), zelfde bitrate als de STM32.
- [ ] Wemos-board en poort correct gekozen in Arduino IDE.
- [ ] STM32 seriële poort op 115200 baud.

| Probleem | Waarschijnlijke oorzaak |
|---|---|
| Dashboard-LED "Socketverbinding" blijft rood | RPI-BUS draait niet of verkeerd IP in `SysteemConfig.h` |
| `[CAN-BUS] Kan niet verbinden met interface can0` | `can0` niet omhoog of geen CAN-controller → `sudo ip link set can0 up type can bitrate 500000` |
| Wemos meldt `[Bifrost] verbinden met Heimdall mislukt` | RPI-WEMOS draait niet, of `PI_IP_ADRES`/`BIFROST_POORT` klopt niet |
| Wemos blijft op "." in Serial Monitor | WiFi-SSID/wachtwoord fout |
| Tafelknop/RGB doet niets op het dashboard | Wemos niet verbonden met Heimdall (check serial: "verbonden met Heimdall") |
| Geen sensordata in grafieken | STM32 stuurt niet (transceiver/terminering/bitrate), of RPI-BUS niet verbonden met WEMOS poort 8080 |

---

## Bijlage A — Het Bifrost-protocol (Pi ↔ Wemos)

Bifrost is ons eigen, minimale protocol bovenop een gewone TCP-verbinding — geen
MQTT, geen broker, geen messaging-library. Het werkt identiek over kabel (RJ45)
en WiFi.

**Rollen**
- **Heimdall** = de server. Draait op de RPI-WEMOS-Pi en luistert op poort `9000`
  (op alle interfaces). Meerdere Wemos-en mogen tegelijk verbonden zijn.
- **SocketWemos** (Valkyrie-rol) = de client. Draait op elke Wemos en verbindt
  naar Heimdall.

**Berichtformaat — een "Rune" is precies één tekstregel:**
```
topic<spatie>payload\n
```
- Alles vóór de eerste spatie = het **topic** (bevat zelf nooit een spatie).
- Alles erna = de **payload** (mag spaties bevatten).
- De `\n` markeert het einde; de ontvanger buffert binnenkomende bytes tot hij een
  `\n` ziet en verwerkt dan die regel. Zo blijven berichten netjes gescheiden,
  ook als TCP ze samenvoegt of opsplitst.

**Alle topics in het systeem:**

| Topic | Richting | Payload | Betekenis |
|---|---|---|---|
| `tafel/<id>/status` | Wemos → Pi | `HELP` / `OK` | tafel vraagt hulp / is geholpen |
| `tafel/<id>/reset`  | Pi → Wemos | `RESET` | zet de tafel-LED uit |
| `sensor/beweging`   | Wemos → Pi | `JA` / `NEE` | PIR detecteert (geen) beweging |
| `sensor/rgb/set`    | Pi → Wemos | `R,G,B` of `UIT` | sfeerlicht instellen |
| `wemos/lichtkrant`  | Pi → Wemos | `MENU:<tekst>` / `MSG:<tekst>` | vaste loop / eenmalige melding |

**Routing (bewust simpel gehouden)**
- Uitgaand stuurt Heimdall elke Rune naar **alle** verbonden Wemos-en (broadcast);
  elke Wemos filtert zelf op het topic dat hij nodig heeft en negeert de rest.
- Inkomend gaat elke Rune van een Wemos naar het dashboard.

Zo gedraagt Bifrost zich als MQTT's publish/subscribe, maar volledig met eigen
code en zonder broker. De andere twee verbindingen (BUS↔WEMOS en STM32↔BUS)
staan hier los van — zie Bijlage B.

---

## Bijlage B — Architectuur & de drie verbindingen

De volledige dataketen, allemaal met **eigen implementaties** (geen kant-en-klare
netwerk- of messaging-libraries):

```
STM32 (Ventilator)
   │  CAN-bus  (can0, 500 kbit/s) — 32-bit float per CAN-ID
   ▼
RPI-BUS  ── TCP 8080 (sensordata, tekstregels) ──►  RPI-WEMOS (dashboard)
         ◄─ TCP 8081 (commando's) ──────────────────┘
                                                     │  Bifrost / TCP 9000
                                                     ▼
                                           Wemos #1 (tafel + lichtkrant)
                                           Wemos #2 (PIR + RGB)
```

| Verbinding | Techniek | Aan beide kanten |
|---|---|---|
| **STM32 ↔ RPI-BUS** | CAN-bus | STM32: HAL CAN1 · Pi: QtSerialBus + SocketCAN (`can0`) via `CanBusCommunicatieRPIBUS` |
| **RPI-BUS ↔ RPI-WEMOS** | eigen rauwe POSIX-sockets | `SocketCommunicatieRPIBUS` ↔ `SocketCommunicatieRPIWEMOS` · eigen tekstregels · poort 8080/8081 |
| **RPI-WEMOS ↔ Wemos** | Bifrost over TCP | Pi: rauwe POSIX-socket (`Heimdall`) · Wemos: `WiFiClient` (`SocketWemos`) · poort 9000 |

**Over de transportlagen:**
- Op de Pi's gebruiken we kale POSIX-sockets (`socket()/bind()/listen()/...`) met
  een eigen achtergrond-thread voor het lezen.
- Op de ESP8266 levert **`WiFiClient`** de TCP-bytestream — de core-tegenhanger van
  een socket. (De échte lwIP-socket-API is op die core niet beschikbaar; vandaar
  WiFiClient.) Het *protocol* erbovenop, Bifrost, is volledig eigen code.

**CAN-payload (STM32 → Pi):** elke sensor is een eigen CAN-ID met 4 databytes
(little-endian 32-bit float):
`0x100` = CO2 · `0x101` = temperatuur · `0x102` = luchtvochtigheid.

**Berichtformaat RPI-BUS ↔ RPI-WEMOS** (eigen tekstregels, handmatig geparset met
splitsen op spaties — géén JSON-library):

| Regel | Richting | Betekenis |
|---|---|---|
| `SENSOR <nodeId> <type> <waarde>` | BUS → WEMOS | sensormeting, bv. `SENSOR 256 CO2 750` |
| `STATUS <brand> <overrule> <ventilator>` | BUS → WEMOS | systeemstatus met 1/0, bv. `STATUS 0 0 1` |
| `HEARTBEAT` | BUS → WEMOS | levensteken (elke 2 s) → status-LED van de socketverbinding |
| `COMMAND: ALARM_OVERRULED` | WEMOS → BUS | gebruiker heft het brandalarm op |

Net als Bifrost is elke regel met `\n` afgesloten en wordt hij bij de ontvanger
met een simpele `split()` uit elkaar gehaald.
