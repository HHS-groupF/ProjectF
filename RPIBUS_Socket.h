#ifndef RPIBUS_SOCKET_H
#define RPIBUS_SOCKET_H

#include <string>
#include <chrono>
#include <mutex>
#include <atomic> // Toegevoegd voor veilige achtergrond threads

class SocketCommunicatie {
private:
    std::string ipAdresDoel;
    int poort;
    bool isVerbonden;
    std::atomic<bool> stopThreads; // Voorkomt dat de verbinding permanent sterft

    int server_fd;
    std::string laatsteData;
    std::mutex data_mutex;
    std::chrono::time_point<std::chrono::steady_clock> laatsteOntvangstTijd;

public:
    SocketCommunicatie(std::string ipAdresDoel, int poort);
    ~SocketCommunicatie();

    bool verbind();
    void verzendData(std::string bericht);
    std::string ontvangData();
    bool checkConnectieStatus();
};

#endif
