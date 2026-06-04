#ifndef SYSTEEMCONFIG_H
#define SYSTEEMCONFIG_H

#include <QString>

namespace Config {
// Netwerkinstellingen
const QString RPI_WEMOS_IP = "127.0.0.1";
const QString RPI_BUS_IP   = "127.0.0.1";

// Bifrost (eigen socket-protocol naar de Wemos-devices)
constexpr int POORT_BIFROST         = 9000; // Waar Heimdall op luistert (Wemos-clients verbinden hier)

constexpr int POORT_WEMOS_DATA      = 8080; // Waar WEMOS op luistert (binnenkomende sensordata)
constexpr int POORT_RPIBUS_COMMANDS = 8081; // Waar RPI-BUS op luistert (binnenkomende commando's)

// Sensor Drempelwaarden (Limieten)
constexpr double TEMP_WAARSCHUWING = 28.0;
constexpr double TEMP_BRAND        = 45.0;

constexpr double CO2_WAARSCHUWING  = 1200.0;
constexpr double CO2_BRAND         = 1500.0;

constexpr double HUM_WAARSCHUWING  = 65.0;

// Timers & Intervallen (in milliseconden)
constexpr int SENSOR_TIMER_INTERVAL = 2000; // Interval voor het genereren/uitlezen van sensoren
constexpr int UI_TIMER_INTERVAL     = 100;  // Refresh rate van het dashboard

// RGB Sfeerlicht kleur presets (formaat: "R,G,B")
const QString RGB_WIT   = "255,255,255";
const QString RGB_WARM  = "255,120,20";
const QString RGB_ROOD  = "255,0,0";
const QString RGB_BLAUW = "0,0,255";
const QString RGB_GROEN = "0,255,0";
}

#endif // SYSTEEMCONFIG_H