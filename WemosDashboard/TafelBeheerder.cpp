#include "TafelBeheerder.h"

TafelBeheerder::TafelBeheerder(MqttClient& mqttNetwerk) : netwerk(mqttNetwerk) {}

void TafelBeheerder::verwerkBericht(std::string topic, std::string payload) {
    // Haal het tafelnummer uit het topic, bijv. "tafel/3/status" → id = 3
    size_t eersteSlash = topic.find('/');
    size_t tweedeSlash = topic.rfind('/');

    if (eersteSlash != std::string::npos && tweedeSlash != std::string::npos) {
        int id = std::stoi(topic.substr(eersteSlash + 1, tweedeSlash - eersteSlash - 1));

        if (payload == "HELP") {
            // Alleen reageren als deze tafel nog niet actief is (voorkomt dubbele meldingen)
            if (_actieveTafels.find(id) == _actieveTafels.end()) {
                _actieveTafels.insert(id);
                if (onTafelHulpNodig) onTafelHulpNodig(id);
            }
        } else if (payload == "OK") {
            // Tafel uit de actieve set halen
            _actieveTafels.erase(id);
            if (onTafelGeholpen) onTafelGeholpen(id);
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
