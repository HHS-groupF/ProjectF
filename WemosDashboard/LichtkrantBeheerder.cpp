#include "LichtkrantBeheerder.h"

LichtkrantBeheerder::LichtkrantBeheerder(MqttClient& mqttNetwerk) : netwerk(mqttNetwerk) {}

void LichtkrantBeheerder::stuurBericht(const std::string& bericht) {
    netwerk.stuurBericht("wemos/lichtkrant", bericht);
}