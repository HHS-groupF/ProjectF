#include "TafelBeheerder.h"

TafelBeheerder::TafelBeheerder(MqttClient& mqttNetwerk) : netwerk(mqttNetwerk) {}

void TafelBeheerder::verwerkBericht(std::string topic, std::string payload) {
    size_t eersteSlash = topic.find('/');
    size_t tweedeSlash = topic.rfind('/');

    if (eersteSlash != std::string::npos && tweedeSlash != std::string::npos) {
        int id = std::stoi(topic.substr(eersteSlash + 1, tweedeSlash - eersteSlash - 1));

        if (payload == "HELP") {
            if (_actieveTafels.find(id) == _actieveTafels.end()) {
                _actieveTafels.insert(id);
                if (onTafelHulpNodig) onTafelHulpNodig(id);
            }
        } else if (payload == "OK") {
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
