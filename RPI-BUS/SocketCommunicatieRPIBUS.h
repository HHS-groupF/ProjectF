#ifndef SOCKETCOMMUNICATIERPIBUS_H
#define SOCKETCOMMUNICATIERPIBUS_H

#include <QObject>
#include <QString>
#include <atomic>
#include <thread>
#include <mutex>
#include <QTimer>

// ============================================================================
//  SocketCommunicatieRPIBUS — eigen socket-implementatie (rauwe BSD-sockets,
//  geen QTcpSocket/library). De interface is identiek gebleven aan de vorige
//  versie, zodat de rest van de code ongewijzigd blijft.
//
//  - Luistert op POORT_RPIBUS_COMMANDS voor binnenkomende commando's van RPI-WEMOS.
//  - Verbindt naar RPI-WEMOS:POORT_WEMOS_DATA om sensordata te versturen.
// ============================================================================
class SocketCommunicatieRPIBUS : public QObject {
    Q_OBJECT

public:
    explicit SocketCommunicatieRPIBUS(QObject *parent = nullptr);
    ~SocketCommunicatieRPIBUS();

    void start();
    void verzendData(const QString &bericht);

signals:
    void inkomendCommando(const QString &commando);
private slots:
    void verzendHeartbeat();
private:
    void luisterLus();                // accepteert + leest commando's (eigen thread)
    bool zorgVoorSensorVerbinding();  // (her)verbindt de sensor-socket naar RPI-WEMOS

    int serverFd = -1;   // luistert op POORT_RPIBUS_COMMANDS
    int sensorFd = -1;   // verbinding naar RPI-WEMOS voor sensordata

    std::atomic<bool> gestopt{false};
    std::thread luisterThread;
    std::mutex sensorMutex;

    QTimer *heartbeatTimer = nullptr;
};

#endif // SOCKETCOMMUNICATIERPIBUS_H
