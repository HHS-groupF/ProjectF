#ifndef SOCKETCOMMUNICATIERPIBUS_H
#define SOCKETCOMMUNICATIERPIBUS_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>

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
    void nieuweVerbinding();
private:
    QTcpServer *commandServer;
    QTcpSocket *sensorSocket;
};

#endif // SOCKETCOMMUNICATIERPIBUS_H
