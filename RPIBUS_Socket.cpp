/**
 * @file RPIBUS_Socket.cpp
 * @brief Implementatie van de SocketCommunicatie klasse (BUS versie).
 */

#include "RPIBUS_Socket.h"
#include <iostream>
#include <thread>
#include <array>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;
using namespace std::chrono;

/**
 * @brief Initialiseert de socket communicatie instellingen.
 * @param ip Doel IP-adres.
 * @param p Poortnummer.
 */
SocketCommunicatie::SocketCommunicatie(string ip, int p) 
    : ipAdresDoel(move(ip)), poort(p), isVerbonden(false), server_fd(-1) {
    laatsteOntvangstTijd = steady_clock::now();
}

/**
 * @brief Ruimt de socket resources netjes op bij vernietiging van het object.
 */
SocketCommunicatie::~SocketCommunicatie() {
    isVerbonden = false;
    if (server_fd >= 0) close(server_fd);
}

/**
 * @brief Zet een TCP server op en start threads voor ontvangst en heartbeats.
 * * Maakt een niet-blokkerende afhandeling van inkomende connecties mogelijk
 * via een gedetacheerde thread. Start ook een zend-thread voor "I am alive" heartbeats.
 * * @return true bij succesvol binden en luisteren, false bij een fout.
 */
bool SocketCommunicatie::verbind() {
    struct sockaddr_in address{}; 
    int opt = 1;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) return false;

    // Zorgt ervoor dat de poort direct hergebruikt kan worden na herstart
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(poort);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) return false;
    if (listen(server_fd, 3) < 0) return false;

    isVerbonden = true;
    laatsteOntvangstTijd = steady_clock::now();

    // Thread voor het ontvangen van inkomende verbindingen en data
    thread([this]() {
        struct sockaddr_in client_address{};
        socklen_t addrlen = sizeof(client_address);
        array<char, 1024> buffer{}; 

        while (this->isVerbonden) {
            int new_socket = accept(this->server_fd, (struct sockaddr*)&client_address, &addrlen);
            if (new_socket >= 0) {
                buffer.fill(0); 
                ssize_t bytesRead = read(new_socket, buffer.data(), buffer.size() - 1);
                
                if (bytesRead > 0) {
                    string ontvangen(buffer.data(), bytesRead);
                    this->laatsteOntvangstTijd = steady_clock::now();

                    // Filter heartbeats eruit voor de datastroom
                    if (ontvangen.find("Heartbeat") == string::npos) {
                        lock_guard<mutex> lock(this->data_mutex);
                        this->laatsteData = ontvangen;
                        cout << "\n[BUS - Nieuwe Data Ontvangen]: " << ontvangen << endl;
                    } else {
                        cout << "[BUS - Heartbeat Ontvangen]: " << ontvangen << endl;
                    }
                }
                close(new_socket);
            }
        }
    }).detach();

    // Thread voor het periodiek verzenden van een heartbeat
    thread([this]() {
        while (this->isVerbonden) {
            this_thread::sleep_for(seconds(2));
            this->verzendData("Heartbeat: BUS Pi is online.");
        }
    }).detach();

    return true;
}

/**
 * @brief Maakt een tijdelijke client-socket aan en verstuurt data.
 * @param bericht De tekst die via TCP verzonden moet worden.
 */
void SocketCommunicatie::verzendData(const string& bericht) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return;

    struct sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(poort);

    if (inet_pton(AF_INET, ipAdresDoel.c_str(), &serv_addr.sin_addr) <= 0) {
        close(sock);
        return;
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) >= 0) {
        send(sock, bericht.c_str(), bericht.length(), 0);
    }
    close(sock);
}

/**
 * @brief Thread-safe ophalen van nieuwe data.
 * @return De ontvangen string. De interne buffer wordt na ophalen direct leeggemaakt.
 */
string SocketCommunicatie::ontvangData() {
    lock_guard<mutex> lock(data_mutex);
    string data = laatsteData;
    laatsteData.clear(); 
    return data;
}

/**
 * @brief Bewaakt de verbinding op basis van de laatste ontvangsttijd.
 * * Als er meer dan 5 seconden geen data of heartbeat is binnengekomen,
 * wordt de verbinding als verbroken beschouwd.
 * * @return true zolang de verbinding actief is, anders false.
 */
bool SocketCommunicatie::checkConnectieStatus() {
    auto nu = steady_clock::now();
    auto verstreken = duration_cast<seconds>(nu - laatsteOntvangstTijd).count();

    if (verstreken > 5) {
        if (isVerbonden) {
            cout << "\n[FOUT] BUS Communicatie langer dan 5 sec weggevallen!" << endl;
        }
        isVerbonden = false;
    } else {
        isVerbonden = true;
    }

    return isVerbonden;
}

// Opmerking: Deze resterende accolade stond in de originele code. Mogelijk onnodig als er geen namespace open is.
}
