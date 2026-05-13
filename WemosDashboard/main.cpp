#include "MqttClient.h"
#include "TafelBeheerder.h"
#include <iostream>

int main() {
    std::cout << "--- Restaurant Systeem Gestart ---" << std::endl;

    MqttClient mqtt("PiController", "127.0.0.1", 1883);
    TafelBeheerder beheerder(mqtt);

    // Koppel netwerk aan beheerder
    mqtt.setCallback([&beheerder](std::string t, std::string p){
        beheerder.verwerkBericht(t, p);
    });

    // Koppel beheerder aan de console output
    beheerder.setUIEvents(
        [](int id){ std::cout << "\n[ALARM] Tafel " << id << " roept! \n> "; },
        [](int id){ std::cout << "\n[INFO] Tafel " << id << " is ok. \n> "; }
    );

    mqtt.loop_start();

    // Console Input Loop
    std::string invoer;
    std::cout << "\nTyp het tafelnummer in om de lamp uit te zetten (of 'q' om te stoppen).\n> ";
    
    while (std::cin >> invoer && invoer != "q") {
        try {
            int id = std::stoi(invoer);
            beheerder.resetTafel(id);
            std::cout << "--> Signaal gestuurd naar Wemos voor tafel " << id << "\n> ";
        } catch (...) {
            std::cout << "Ongeldig nummer.\n> ";
        }
    }

    mqtt.loop_stop();
    return 0;
}