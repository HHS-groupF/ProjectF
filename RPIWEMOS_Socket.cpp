#include "RPIWEMOS_Socket.h"
#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

SocketCommunicatie::SocketCommunicatie(std::string ip, int p) {
    // Let op: De parameters worden hier overschreven door vaste waarden.
    ipAdresDoel = "10.0.0.1";
    poort = 50001;
    isVerbonden = false;
    stopThreads = false;
    server_fd = -1;
    laatsteOntvangstTijd = std::chrono::steady_clock::now();
}

SocketCommunicatie::~SocketCommunicatie() {
    stopThreads = true; // Zorgt dat de while-loops in de threads stoppen
    // Sluit de poort netjes af als het object wordt vernietigd
    if (server_fd >= 0) close(server_fd);
}

bool SocketCommunicatie::verbind() {
    struct sockaddr_in address;
    int opt = 1;

    // Maak een TCP socket aan
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) return false;

    // Zorg ervoor dat we de poort direct opnieuw kunnen gebruiken na een crash of herstart
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Luister op alle netwerkinterfaces
    address.sin_port = htons(poort);

    // Koppel de socket aan de poort en begin met luisteren (max 3 in de wachtrij)
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) return false;
    if (listen(server_fd, 3) < 0) return false;

    isVerbonden = true;
    laatsteOntvangstTijd = std::chrono::steady_clock::now();

    // Start een achtergrond-thread (detach) om inkomende verbindingen af te handelen
    std::thread([this]() {
        struct sockaddr_in client_address;
        int addrlen = sizeof(client_address);
        char buffer[1024];

        while (!this->stopThreads) {
            // Wacht op een inkomende connectie
            int new_socket = accept(this->server_fd, (struct sockaddr*)&client_address, (socklen_t*)&addrlen);
            if (new_socket >= 0) {
                memset(buffer, 0, 1024); 
                int bytesRead = read(new_socket, buffer, 1024);
                
                if (bytesRead > 0) {
                    std::string ontvangen(buffer);
                    
                    // Update de timer omdat we zojuist iets ontvangen hebben
                    this->laatsteOntvangstTijd = std::chrono::steady_clock::now();

                    // Sla alleen echte data op in de buffer, negeer heartbeats voor de hoofdapplicatie
                    if (ontvangen.find("Heartbeat") == std::string::npos) {
                        std::lock_guard<std::mutex> lock(this->data_mutex);
                        this->laatsteData = ontvangen;
                        std::cout << "\n[Nieuwe Data Ontvangen]: " << ontvangen << std::endl;
                    } else {
                        std::cout << "[Heartbeat Ontvangen]: " << ontvangen << std::endl;
                    }
                }
                close(new_socket); // Verbinding weer sluiten na het lezen
            }
            // Kleine pauze om de CPU niet voor 100% te belasten
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }).detach();

    // Start een tweede achtergrond-thread die elke 2 seconden een teken van leven (heartbeat) verstuurt
    std::thread([this]() {
        while (!this->stopThreads) {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            this->verzendData("Heartbeat: Pi is online.");
        }
    }).detach();

    return true;
}

void SocketCommunicatie::verzendData(std::string bericht) {
    int sock = 0;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) return;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(poort);

    // Zet het IP adres om naar het juiste formaat
    if (inet_pton(AF_INET, ipAdresDoel.c_str(), &serv_addr.sin_addr) <= 0) {
        close(sock);
        return;
    }

    // Probeer te verbinden en stuur het bericht
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) >= 0) {
        send(sock, bericht.c_str(), bericht.length(), 0);
    }
    close(sock);
}

std::string SocketCommunicatie::ontvangData() {
    std::lock_guard<std::mutex> lock(data_mutex); // Lock om thread-safety problemen te voorkomen
    std::string data = laatsteData;
    laatsteData = ""; // Maak de string leeg zodat we niet twee keer hetzelfde uitlezen
    return data;
}

bool SocketCommunicatie::checkConnectieStatus() {
    // Check hoelang het geleden is sinds het laatste bericht (data of heartbeat)
    auto nu = std::chrono::steady_clock::now();
    auto verstreken = std::chrono::duration_cast<std::chrono::seconds>(nu - laatsteOntvangstTijd).count();

    // Als we 5 seconden niks gehoord hebben, gaan we ervan uit dat de verbinding weg is
    if (verstreken > 5) {
        if (isVerbonden) {
            std::cout << "\n[FOUT] Communicatie langer dan 5 sec weggevallen!" << std::endl;
        }
        isVerbonden = false;
    } else {
        isVerbonden = true;
    }

    return isVerbonden;
}
