#ifndef SYSTEEMCONFIG_H
#define SYSTEEMCONFIG_H

#include <QString>

namespace Config {
// Netwerkinstellingen
const QString RPI_WEMOS_IP = "145.52.127.190";
const QString RPI_BUS_IP   = "145.52.127.183";

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
}

#endif // SYSTEEMCONFIG_H