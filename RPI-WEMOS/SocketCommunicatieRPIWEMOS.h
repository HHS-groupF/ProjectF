#ifndef SOCKETCOMMUNICATIERPIWEMOS_H
#define SOCKETCOMMUNICATIERPIWEMOS_H

#include "IBusVerbinding.h"
#include <QString>
#include <atomic>
#include <thread>
#include <mutex>

// ============================================================================
//  SocketCommunicatieRPIWEMOS — eigen socket-implementatie (rauwe BSD-sockets,
//  geen QTcpSocket/library). 
//
//  - Luistert op POORT_WEMOS_DATA voor binnenkomende sensordata van RPI-BUS.
//  - Verbindt naar RPI-BUS:POORT_RPIBUS_COMMANDS om commando's te versturen.
// ============================================================================
class SocketCommunicatieRPIWEMOS : public IBusVerbinding {
    Q_OBJECT

public:
    explicit SocketCommunicatieRPIWEMOS(QObject *parent = nullptr);
    ~SocketCommunicatieRPIWEMOS() override;

    bool verbind() override;
    void verzendData(const QString &bericht) override;
    QString ontvangData() override;
    bool checkConnectieStatus() override;

    // nieuwLogBericht() is geërfd van IBusVerbinding.

private:
    void luisterLus();               // accepteert + leest sensordata (eigen thread)
    bool zorgVoorStuurVerbinding();  // (her)verbindt de stuur-socket naar RPI-BUS

    int serverFd = -1;   // luistert op POORT_WEMOS_DATA
    int stuurFd  = -1;   // verbinding naar RPI-BUS voor commando's

    std::atomic<bool> gestopt{false};
    std::thread luisterThread;
    std::mutex dataMutex;
    std::mutex stuurMutex;
    QString laatsteData;
    std::atomic<qint64> laatsteOntvangstTijd{0};
};

#endif // SOCKETCOMMUNICATIERPIWEMOS_H
