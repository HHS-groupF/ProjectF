#ifndef RPIWEMOS_SOCKET_H
#define RPIWEMOS_SOCKET_H

#include <string>
#include <chrono>
#include <mutex>
#include <atomic>

#include "config.h"

// Klasse voor het opzetten en beheren van TCP-socketcommunicatie voor de Wemos
class SocketCommunicatie {
private:
    std::string ipAdresDoel; // IP waar we berichten naartoe sturen
    int poort;               // De poort voor zowel zenden als ontvangen
    bool isVerbonden;        // Status van de verbinding
    std::atomic<bool> stopThreads; // Flag om de loops in de threads netjes te kunnen stoppen

    int server_fd;           // File descriptor van de server socket
    std::string laatsteData; // Opslag voor het laatst ontvangen bericht
    std::mutex data_mutex;   // Zorgt dat data veilig uitgelezen kan worden door threads
    std::chrono::time_point<std::chrono::steady_clock> laatsteOntvangstTijd; // Tijdstip van laatste teken van leven

public:
    // De WEMOS stuurt standaard naar de BUS-machine; waarden komen uit config.h
    SocketCommunicatie(std::string ipAdresDoel = Config::IP_RPIBUS, int poort = Config::POORT);
    ~SocketCommunicatie();

    bool verbind(); // Start de ontvangstserver en achtergrondtaken
    void verzendData(std::string bericht); // Stuur een bericht naar het doel-IP
    std::string ontvangData(); // Lees de nieuwste data uit en maak de buffer leeg
    bool checkConnectieStatus(); // Controleert op een timeout (bijv. Wemos offline of wifi weg)
};

#endif
