#ifndef SOCKETCOMMUNICATIERPIWEMOS_H
#define SOCKETCOMMUNICATIERPIWEMOS_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMutex>

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

private slots:
    void nieuweVerbinding();
    void leesData();

private:
    QTcpServer *server;
    QTcpSocket *stuurSocket;
    QString laatsteData;
    QMutex dataMutex;
    qint64 laatsteOntvangstTijd = 0;
};

#endif // SOCKETCOMMUNICATIERPIWEMOS_H
