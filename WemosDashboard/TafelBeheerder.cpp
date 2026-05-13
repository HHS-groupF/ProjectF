#include "TafelBeheerder.h"

TafelBeheerder::TafelBeheerder(MqttClient& mqttNetwerk) : netwerk(mqttNetwerk) {}

void TafelBeheerder::verwerkBericht(std::string topic, std::string payload) {
    // Haal id uit "tafel/X/status"
    size_t eersteSlash = topic.find('/');
    size_t tweedeSlash = topic.rfind('/');
    
    if (eersteSlash != std::string::npos && tweedeSlash != std::string::npos) {
        int id = std::stoi(topic.substr(eersteSlash + 1, tweedeSlash - eersteSlash - 1));
        
        // Vuur de juiste UI actie af
        if (payload == "HELP" && onTafelHulpNodig) {
            onTafelHulpNodig(id);
        } else if (payload == "OK" && onTafelGeholpen) {
            onTafelGeholpen(id);
        }
    }
}

void TafelBeheerder::resetTafel(int id) {
    std::string topic = "tafel/" + std::to_string(id) + "/reset";
    netwerk.stuurBericht(topic, "RESET");
}

void TafelBeheerder::setUIEvents(std::function<void(int)> hulpNodig, std::function<void(int)> geholpen) {
    onTafelHulpNodig = hulpNodig;
    onTafelGeholpen = geholpen;
}