#ifndef SOCKETCOMMUNICATIERPIBUS_H
#define SOCKETCOMMUNICATIERPIBUS_H

#include <QObject>
#include <QString>
#include <atomic>
#include <thread>
#include <mutex>
#include <QTimer>


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
    void luisterLus();               
    bool zorgVoorSensorVerbinding();  

    int serverFd = -1;   
    int sensorFd = -1;   

    std::atomic<bool> gestopt{false};
    std::thread luisterThread;
    std::mutex sensorMutex;

    QTimer *heartbeatTimer = nullptr;
};

#endif // SOCKETCOMMUNICATIERPIBUS_H
