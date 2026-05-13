/**
 * @file RPIBUS_Socket.h
 * @brief Header-bestand voor de SocketCommunicatie klasse (BUS versie).
 * * Bevat de klassedefinitie voor het opzetten en beheren van TCP-socketcommunicatie.
 * Ondersteunt asynchrone ontvangst en automatische heartbeat-controle.
 */

#ifndef RPIBUS_SOCKET_H
#define RPIBUS_SOCKET_H

#include <string>
#include <chrono>
#include <mutex>
#include <atomic>

/**
 * @class SocketCommunicatie
 * @brief Beheert de socket communicatie voor de Raspberry Pi BUS.
 * * Deze klasse zorgt voor het opzetten van een server-socket om data en heartbeats 
 * te ontvangen, en bevat logica om berichten naar een doel-IP te sturen.
 */
class SocketCommunicatie {
private:
    std::string ipAdresDoel;                                                /**< Het IP-adres waarnaar berichten worden verstuurd. */
    int poort;                                                              /**< De poort waarop gecommuniceerd wordt. */
    bool isVerbonden;                                                       /**< Statusvlag die aangeeft of de verbinding actief is. */
    std::atomic<bool> stopThreads;                                          /**< Vlag om threads veilig af te sluiten (indien geïmplementeerd). */
    int server_fd;                                                          /**< File descriptor voor de server socket. */
    std::string laatsteData;                                                /**< Buffer voor de laatst ontvangen data. */
    std::mutex data_mutex;                                                  /**< Mutex voor thread-safe toegang tot data. */
    std::chrono::time_point<std::chrono::steady_clock> laatsteOntvangstTijd;/**< Tijdstip van het laatst ontvangen bericht. */

public:
    /**
     * @brief Constructor voor SocketCommunicatie.
     * @param ipAdresDoel Het IP-adres van de doelmachine.
     * @param poort De netwerkpoort voor de communicatie.
     */
    SocketCommunicatie(std::string ipAdresDoel, int poort);

    /**
     * @brief Destructor. Sluit de openstaande server socket.
     */
    ~SocketCommunicatie();

    /**
     * @brief Start de server en de bijbehorende luister- en heartbeat-threads.
     * @return true als de socket succesvol is aangemaakt en gebonden, anders false.
     */
    bool verbind();

    /**
     * @brief Verstuurt een bericht over TCP naar het ingestelde doel-IP.
     * @param bericht De string-data die verzonden moet worden.
     */
    void verzendData(std::string bericht);

    /**
     * @brief Haalt de laatst ontvangen data op en leegt de buffer.
     * @return Een string met de laatst ontvangen data, of een lege string als er geen nieuwe data is.
     */
    std::string ontvangData();

    /**
     * @brief Controleert of er recent (binnen 5 seconden) nog een bericht of heartbeat is ontvangen.
     * @return true als de verbinding nog actief lijkt, anders false.
     */
    bool checkConnectieStatus();
};

#endif
