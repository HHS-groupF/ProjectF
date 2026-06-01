#include "SensorBeheerder.h"

SensorBeheerder::SensorBeheerder(MqttClient& mqtt) : _mqtt(mqtt) {}

void SensorBeheerder::verwerkBeweging(const std::string& status) {
    if (status == "JA") {
        _mqtt.stuurBericht("sensor/rgb/set", "AAN");
        std::cout << "\r\033[2K[SENSOR] Beweging gedetecteerd, sfeerlicht AAN.\n> " << std::flush;
    } 
    else if (status == "NEE") {
        _mqtt.stuurBericht("sensor/rgb/set", "UIT");
        std::cout << "\r\033[2K[SENSOR] Geen beweging meer, sfeerlicht UIT.\n> " << std::flush;
    }
}