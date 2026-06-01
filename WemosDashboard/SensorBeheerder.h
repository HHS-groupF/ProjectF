#ifndef SENSORBEHEERDER_H
#define SENSORBEHEERDER_H

#include <string>
#include <iostream>
#include "MqttClient.h"

class SensorBeheerder {
private:
    MqttClient& _mqtt;

public:
    SensorBeheerder(MqttClient& mqtt);
    void verwerkBeweging(const std::string& status);
};

#endif