#include "RPIBUS_Socket.h"
#include <iostream>
#include <thread>

int main() {
    // Instantieer het communicatie-object met het doel-IP en de poort uit config.h
    SocketCommunicatie communicator(Config::IP_RPIWEMOS, Config::POORT);

    // Probeer de server te starten en te binden aan de poort
    if (communicator.verbind()) {
        std::cout << "Systeem gestart..." << std::endl;
    } else {
        std::cerr << "Fout bij starten!" << std::endl;
        return 1; // Sluit af met een error code
    }
    
    // De oneindige hoofd-loop van het programma
    while (true) {
        // Kijk of de verbinding er nog is (er moet binnen 5s een heartbeat/data binnenkomen)
        if (!communicator.checkConnectieStatus()) {
            std::cout << "Verbinding verbroken..." << std::endl;
        }

        // Kijk of er nog bruikbare data (geen heartbeats) binnengekomen is
        std::string data = communicator.ontvangData();
        if (!data.empty()) {
            std::cout << "Hoofdprogramma ontving: " << data << std::endl;
        }

        // Wacht even voor we weer checken, anders gaat de CPU op 100% draaien
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    return 0;
}
