#include "RPIBUS_Socket.h" // Of RPIWEMOS_Socket.h afhankelijk van de Pi
#include <iostream>
#include <thread>

int main() {
    SocketCommunicatie communicator("10.0.0.2", 50001);

    if (communicator.verbind()) {
        std::cout << "Systeem gestart..." << std::endl;
    } else {
        std::cerr << "Fout bij starten!" << std::endl;
        return 1;
    }
    while (true) {
        if (!communicator.checkConnectieStatus()) {
            std::cout << "Verbinding verbroken..." << std::endl;
        }

        std::string data = communicator.ontvangData();
        if (!data.empty()) {
            std::cout << "Hoofdprogramma ontving: " << data << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    return 0;
}
