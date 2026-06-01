#ifndef TAFELBEHEERDER_H
#define TAFELBEHEERDER_H

#include "MqttClient.h"
#include <string>
#include <functional>
#include <set>


class TafelBeheerder {
private:
    MqttClient& netwerk;
    std::function<void(int)> onTafelHulpNodig; 
    std::function<void(int)> onTafelGeholpen;  
    std::set<int> _actieveTafels; 

public:
    TafelBeheerder(MqttClient& mqttNetwerk);
    void verwerkBericht(std::string topic, std::string payload);
    void resetTafel(int id);                                     
    void setUIEvents(std::function<void(int)> hulpNodig, std::function<void(int)> geholpen);
};

#endif
