#ifndef SENSORBEHEERDER_H
#define SENSORBEHEERDER_H

#include <string>
#include <iostream>
#include <chrono>
#include <mutex>
#include "MqttClient.h"
#include "config.h"

class SensorBeheerder {
private:
    MqttClient& _mqtt;
    bool _timerActief = false;                                // loopt er een timer?
    std::chrono::steady_clock::time_point _geenBewegingSinds; // tijdstip van laatste "NEE"
    std::mutex _mutex; // voorkomt dat thread en MQTT callback elkaar verstoren

public:
    SensorBeheerder(MqttClient& mqtt);
    void verwerkBeweging(const std::string& status);
    void tick(); // elke seconde aanroepen — checkt of de timer voorbij is
};

#endif