#ifndef TAFELBEHEERDER_H
#define TAFELBEHEERDER_H

#include "MqttClient.h"
#include <string>
#include <functional>
#include <set>

// Verwerkt MQTT berichten en geeft events door aan de UI
class TafelBeheerder {
private:
    MqttClient& netwerk;
    std::function<void(int)> onTafelHulpNodig; // Wordt aangeroepen als een tafel HELP stuurt
    std::function<void(int)> onTafelGeholpen;  // Wordt aangeroepen als een tafel OK stuurt
    std::set<int> _actieveTafels; // Houdt bij welke tafels momenteel hulp nodig hebben

public:
    TafelBeheerder(MqttClient& mqttNetwerk);
    void verwerkBericht(std::string topic, std::string payload); // Verwerkt een binnenkomend MQTT bericht
    void resetTafel(int id);                                     // Stuurt RESET naar de Wemos van die tafel
    void setUIEvents(std::function<void(int)> hulpNodig, std::function<void(int)> geholpen);
};

#endif
