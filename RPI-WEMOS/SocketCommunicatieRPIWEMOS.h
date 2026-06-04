#ifndef SOCKETCOMMUNICATIERPIWEMOS_H
#define SOCKETCOMMUNICATIERPIWEMOS_H

#include <QObject>
#include <QString>
#include <atomic>
#include <thread>
#include <mutex>

// ============================================================================
//  SocketCommunicatieRPIWEMOS — eigen socket-implementatie (rauwe BSD-sockets,
//  geen QTcpSocket/library). De interface is identiek gebleven aan de vorige
//  versie, zodat de rest van de code ongewijzigd blijft.
//
//  - Luistert op POORT_WEMOS_DATA voor binnenkomende sensordata van RPI-BUS.
//  - Verbindt naar RPI-BUS:POORT_RPIBUS_COMMANDS om commando's te versturen.
// ============================================================================
class SocketCommunicatieRPIWEMOS : public QObject {
    Q_OBJECT

public:
    explicit SocketCommunicatieRPIWEMOS(QObject *parent = nullptr);
    ~SocketCommunicatieRPIWEMOS();

    bool verbind();
    void verzendData(const QString &bericht);
    QString ontvangData();
    bool checkConnectieStatus();

signals:
    void nieuwLogBericht(QString bericht);

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
