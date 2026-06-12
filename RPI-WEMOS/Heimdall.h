#ifndef HEIMDALL_H
#define HEIMDALL_H

#include "IBerichtKanaal.h"
#include <QString>
#include <atomic>
#include <thread>
#include <mutex>
#include <vector>

// ============================================================================
//  Heimdall — de Bifrost-server (eigen implementatie, rauwe BSD-sockets).
// ----------------------------------------------------------------------------
//  De Wemos-clients (Valkyrie / SocketWemos) verbinden hiernaartoe over TCP/IP.
//  Er wordt geluisterd op INADDR_ANY, dus zowel over kabel (RJ45/eth0) als WiFi.
//
//  Berichten heten Runes en zijn één tekstregel:
//        "topic<spatie>payload\n"
//  bv.   "tafel/1/reset RESET\n"  of  "sensor/rgb/set 255,120,20\n"
//
//  Heimdall stuurt uitgaande Runes naar alle verbonden clients (broadcast);
//  elke Wemos filtert zelf op het topic dat hij nodig heeft.
// ============================================================================
class Heimdall : public IBerichtKanaal {
    Q_OBJECT
public:
    explicit Heimdall(QObject *parent = nullptr);
    ~Heimdall() override;

    bool start(int poort) override;                                        // begin te luisteren
    void publiceer(const QString &topic, const QString &payload) override; // stuur Rune naar alle clients

    // runeOntvangen() en logBericht() zijn geërfd van IBerichtKanaal.

private:
    void accepteerLus();              // accepteert nieuwe clients (eigen thread)
    void verwerkClient(int clientFd); // leest Runes van één client (eigen thread)

    int serverFd = -1;
    int poort = 0;
    std::atomic<bool> gestopt{false};
    std::thread acceptThread;
    std::vector<int> clients;     // verbonden client-sockets ("Bridges")
    std::mutex clientsMutex;
};

#endif // HEIMDALL_H
