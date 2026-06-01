#ifndef LICHTKRANTBEHEERDER_H
#define LICHTKRANTBEHEERDER_H

#include "MqttClient.h"
#include <string>

class LichtkrantBeheerder {
private:
    MqttClient& netwerk;

public:
    LichtkrantBeheerder(MqttClient& mqttNetwerk);
    void stuurBericht(const std::string& bericht);
};

#endif