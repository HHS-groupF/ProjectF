#include "config.h"
#include "MqttClient.h"
#include "TafelBeheerder.h"
#include "LichtkrantBeheerder.h"
#include "SensorBeheerder.h" 
#include "DashboardUI.h"     
#include <iostream>
#include <string>

int main() {
    std::cout << "--- Restaurant Systeem Gestart ---" << std::endl;

    
    MqttClient mqtt(MQTT_CLIENT_ID, MQTT_SERVER, MQTT_PORT);
    DashboardUI ui;
    TafelBeheerder tafelBeheerder(mqtt);
    LichtkrantBeheerder lichtkrant(mqtt);
    SensorBeheerder sensorBeheerder(mqtt); 

    // --- CALLBACK LOGICA ---
    mqtt.setCallback([&tafelBeheerder, &sensorBeheerder](std::string topic, std::string payload){
        if (topic == "sensor/beweging") {
            sensorBeheerder.verwerkBeweging(payload);
        } 
        
        else {
            tafelBeheerder.verwerkBericht(topic, payload);
        }
    });

    // --- UI EVENTS (Tafels) ---
    tafelBeheerder.setUIEvents(
        [&lichtkrant, &ui](int id){ 
            std::cout << "\r\033[2K[ALARM] Tafel " << id << " roept!\n"; 
            lichtkrant.stuurBericht("MSG:Tafel " + std::to_string(id) + " roept!");
            ui.toonPrompt();
        },
        [&lichtkrant, &ui](int id){ 
            std::cout << "\r\033[2K[INFO] Tafel " << id << " is ok.\n"; 
            lichtkrant.stuurBericht("MSG:Tafel " + std::to_string(id) + " is ok.");
            ui.toonPrompt();
        }
    );

   
    mqtt.subscribe(nullptr, "sensor/beweging");
    mqtt.loop_start();
    ui.toonMenu(); 
    
    std::string invoer;
    
    while (std::getline(std::cin, invoer)) {
        if (invoer == "q") {
            break;
        } else if (invoer.empty()) {
            ui.toonMenu(); 
            continue; 
        } else if (invoer.rfind("menu ", 0) == 0) { 
            std::string bericht = invoer.substr(5); 
            lichtkrant.stuurBericht("MENU:" + bericht);
            std::cout << "\r\033[2K--> Vaste loop ingesteld op: " << bericht << "\n";
            ui.toonPrompt();
        } else if (invoer.rfind("msg ", 0) == 0) { 
            std::string bericht = invoer.substr(4); 
            lichtkrant.stuurBericht("MSG:" + bericht);
            std::cout << "\r\033[2K--> Snelle announcement gestuurd: " << bericht << "\n";
            ui.toonPrompt();
        } else {
            try {
               
                int id = std::stoi(invoer);
                tafelBeheerder.resetTafel(id);
                std::cout << "\r\033[2K--> Signaal gestuurd naar Wemos voor tafel " << id << "\n";
                ui.toonPrompt();
            } catch (...) {
                ui.toonFoutmelding("Ongeldig commando. Druk op Enter voor het menu.");
            }
        }
    }

    mqtt.loop_stop();
    return 0;
}