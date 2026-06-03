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
| MQTT-broker IP | `RPI-WEMOS/SysteemConfig.h` → `MQTT_BROKER` | IP van de WEMOS-Pi |
| MQTT-broker IP (Wemos #1 + #2) | `Wemos-tafel/.../config.h` → `PI_IP_ADRES` | zelfde als `MQTT_BROKER` |
| WiFi-naam + wachtwoord | `Wemos-tafel/.../config.h` → `WIFI_SSID` / `WIFI_PASSWORD` | jouw netwerk |
| TCP-poorten | `SysteemConfig.h` (beide) | `8080` (data), `8081` (commando's) |
| MQTT-poort | overal | `1883` |

> **Let op:** de Qt-config staat standaard op `127.0.0.1`. Dat werkt alleen als
> alles op één machine draait. Vul de echte IP-adressen in zodra de Pi's en
> Wemos-en op het netwerk staan.

---

## 3. Voorbereiding (eenmalig)

- **Raspberry Pi's:** Raspberry Pi OS geïnstalleerd, op het netwerk, SSH of
  toetsenbord/scherm beschikbaar.
- **Qt:** Qt6 met de modules **Widgets, Charts, Network en Mqtt** (QtMqtt is een
  losse module — controleer of die geïnstalleerd is).
- **Arduino IDE:** met ESP8266-board-ondersteuning
  (Boardmanager-URL: `http://arduino.esp8266.com/stable/package_esp8266com_index.json`).
- **STM32CubeIDE** voor het STM32-deel.

---

## 4. MQTT-broker (Mosquitto) op de WEMOS-Pi

De Wemos-en én het dashboard praten via MQTT. Zet de broker op de WEMOS-Pi.

```bash
sudo apt update
sudo apt install -y mosquitto mosquitto-clients
```

Maak/gebruik de config uit `WemosDashboard/MQTT-confid-pls-read.txt`:

```conf
listener 1883
allow_anonymous true
```

Plaats dit in `/etc/mosquitto/conf.d/projectf.conf` en herstart:

```bash
sudo systemctl restart mosquitto
sudo systemctl enable mosquitto
```

Test:
```bash
mosquitto_sub -h localhost -t '#' -v   # luistert naar alle topics
```

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
   - `MQTT_BROKER` = `127.0.0.1` (broker draait lokaal) of het IP van de broker
4. Zorg dat de modules **Charts** en **Mqtt** beschikbaar zijn (zie §3).
5. Build & run — het dashboard-venster verschijnt.

**Bouwen via terminal:**
```bash
cd RPI-WEMOS
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/RPI-WEMOS
```

In het logboek-venster zou moeten verschijnen:
`"Netwerk backend gestart..."` en `"MQTT verbonden met broker."`

---

## 7. Wemos #1 — Tafel & Lichtkrant (ESP8266)

**Map:** `Wemos-tafel/Wemos-tafel/` (open `Wemos-tafel.ino` in Arduino IDE — alle
`.h`/`.cpp` in die map openen automatisch mee als tabbladen).

**Benodigde libraries** (Library Manager):
- `ESP8266WiFi` (komt met de ESP8266-board-package)
- `PubSubClient`
- `MD_Parola`
- `MD_MAX72xx`
- `SPI` (standaard)

**Aansluitingen (default in de config):**
- Tafelknop → `D1`, tafel-LED → `D2`, tafel-ID = `1` (zie `tafel_config_unit1.h`)
- LED-matrix Chip Select → `D8`, aantal modules = `4` (zie `Wemos-tafel.ino`)

**Voor het uploaden aanpassen in `config.h`:**
- `WIFI_SSID`, `WIFI_PASSWORD`
- `PI_IP_ADRES` = IP van de MQTT-broker (de WEMOS-Pi)

**Uploaden:**
1. Sluit de Wemos via USB aan.
2. Tools → Board: "LOLIN(WEMOS) D1 R2 & mini", juiste poort kiezen.
3. Upload (pijl-knop). Open Serial Monitor op **115200 baud** ter controle.

> Voor een tweede tafel-unit: gebruik `tafel_config_unit2.h` (tafel-ID 2) door die
> include bovenin `Wemos-tafel.ino` te wisselen.

---

## 8. Wemos #2 — Bewegingssensor & Sfeerlicht (ESP8266)

**Map:** `Wemos-tafel/Sensor/` (open `Sensor.ino`).

**Benodigde libraries:** `ESP8266WiFi`, `PubSubClient`.

**Aansluitingen (`config.h`):**
- PIR-sensor → `D1`
- RGB rood → `D2`, groen → `D3`, blauw → `D5`

**Voor het uploaden aanpassen in `config.h`:** `WIFI_SSID`, `WIFI_PASSWORD`,
`PI_IP_ADRES` (broker-IP, gelijk aan Wemos #1).

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

1. **MQTT-broker** (WEMOS-Pi) — moet als eerste draaien.
2. **RPI-BUS** (Qt) — begint sensordata te genereren en luistert op 8081.
3. **RPI-WEMOS** (Qt dashboard) — verbindt met broker + BUS.
4. **Wemos #1 en #2** — verbinden met WiFi en de broker.
5. **STM32 / Ventilator** — start de klimaatregeling.

**Snelle test:**
- Druk op de tafelknop (Wemos #1) → dashboard toont "Tafel 1 vraagt hulp" + LED.
- Beweeg voor de PIR (Wemos #2) → sfeerlicht-status wordt groen op het dashboard.
- In het dashboard "Reset Tafel" / "Update Lichtkrant" / "Stel In" (RGB) testen.

---

## 11. Checklist & veelvoorkomende problemen

- [ ] Alle IP-adressen in de drie `config`/`SysteemConfig`-bestanden komen overeen.
- [ ] MQTT-broker draait en is bereikbaar (`mosquitto_sub -h <broker-ip> -t '#' -v`).
- [ ] WiFi-SSID/wachtwoord kloppen (Serial Monitor toont "WiFi verbonden!").
- [ ] Qt-modules Charts + Mqtt geïnstalleerd (anders faalt CMake bij `find_package`).
- [ ] Wemos-board en poort correct gekozen in Arduino IDE.
- [ ] STM32 seriële poort op 115200 baud.

| Probleem | Waarschijnlijke oorzaak |
|---|---|
| Dashboard-LED "Socketverbinding" blijft rood | RPI-BUS draait niet of verkeerd IP in `SysteemConfig.h` |
| "MQTT verbinding verbroken" | broker draait niet of `MQTT_BROKER`-IP klopt niet |
| Wemos blijft op "." in Serial Monitor | WiFi-SSID/wachtwoord fout |
| CMake-fout `Could not find Qt6 Mqtt` | QtMqtt-module niet geïnstalleerd → zie Bijlage A |
| Geen sensordata in grafieken | RPI-BUS niet verbonden met WEMOS poort 8080 |

---

## Bijlage A — QtMqtt vanaf bron installeren (Raspberry Pi)

QtMqtt zit níet in de apt-repository en moet je zelf bouwen tegen je geïnstalleerde
Qt6. Onderstaande stappen zijn getest op een Raspberry Pi 5 met **Qt 6.8.2**.

**1. Controleer je exacte Qt6-versie** (de tag moet hiermee overeenkomen):
```bash
qmake6 --version
```

**2. Build-gereedschap + benodigde dev-pakketten:**
```bash
sudo apt update
sudo apt install -y git cmake ninja-build build-essential \
                    qt6-base-dev qt6-base-private-dev qt6-charts-dev
```
> `qt6-base-private-dev` is cruciaal: QtMqtt gebruikt private Qt-API
> (`Qt6::CorePrivate`). Zonder dit pakket faalt CMake met de melding dat
> `.../qt6/QtCore/<versie>` niet bestaat.

**3. QtMqtt klonen op de juiste versie-tag** (vervang `6.8.2` door jouw versie):
```bash
cd ~
git clone https://code.qt.io/qt/qtmqtt.git
cd qtmqtt
git checkout v6.8.2
```

**4. Bouwen en installeren in `/usr`** (zodat het systeem-Qt het vindt):
```bash
mkdir build && cd build
cmake -GNinja -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr ..
ninja
sudo ninja install
sudo ldconfig
```

**5. Controleren of het gelukt is:**
```bash
find /usr -name "Qt6MqttConfig.cmake"
```
Komt er een pad terug (bijv. `/usr/lib/aarch64-linux-gnu/cmake/Qt6Mqtt/...`),
dan is `find_package(Qt6 REQUIRED COMPONENTS Mqtt)` tevreden. Daarna in
Qt Creator: **Build → Clear CMake Configuration** en opnieuw configureren.

> **Tip (terminal-plakprobleem):** krijg je fouten als `Unable to locate package
> qt6-base-dev~` of `^[[200~sudo`, dan plakt je terminal "bracketed paste"-tekens
> mee. Zet dat uit met `printf '\e[?2004l'` of typ de commando's handmatig.
