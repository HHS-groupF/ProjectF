#include "SocketCommunicatieRPIWEMOS.h"
#include "SysteemConfig.h"
#include <QDateTime>

SocketCommunicatieRPIWEMOS::SocketCommunicatieRPIWEMOS(QObject *parent) : QObject(parent) {
    server = new QTcpServer(this);
    stuurSocket = new QTcpSocket(this);
    connect(server, &QTcpServer::newConnection, this, &SocketCommunicatieRPIWEMOS::nieuweVerbinding);
}

SocketCommunicatieRPIWEMOS::~SocketCommunicatieRPIWEMOS() {
    if (server->isListening()) server->close();
    if (stuurSocket->isOpen()) stuurSocket->close();
}

bool SocketCommunicatieRPIWEMOS::verbind() {
    stuurSocket->connectToHost(Config::RPI_BUS_IP, Config::POORT_RPIBUS_COMMANDS);
    return server->listen(QHostAddress::Any, Config::POORT_WEMOS_DATA);
}

void SocketCommunicatieRPIWEMOS::nieuweVerbinding() {
    QTcpSocket *client = server->nextPendingConnection();
    connect(client, &QTcpSocket::readyRead, this, &SocketCommunicatieRPIWEMOS::leesData);
    connect(client, &QTcpSocket::disconnected, client, &QTcpSocket::deleteLater);
}

void SocketCommunicatieRPIWEMOS::leesData() {
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    QByteArray inkomendeData = socket->readAll();
    QMutexLocker lock(&dataMutex);
    laatsteData += QString::fromUtf8(inkomendeData);
    laatsteOntvangstTijd = QDateTime::currentMSecsSinceEpoch();
}

void SocketCommunicatieRPIWEMOS::verzendData(const QString &bericht) {
    // Start alleen een nieuwe verbinding als de socket echt los is; bij een
    // verbinding die nog bezig is ('Connecting') zou een tweede connectToHost
    // een Qt-waarschuwing geven.
    if (stuurSocket->state() == QAbstractSocket::UnconnectedState) {
        stuurSocket->connectToHost(Config::RPI_BUS_IP, Config::POORT_RPIBUS_COMMANDS);
    }

    // write() buffert de data terwijl de socket nog verbindt en verstuurt het
    // zodra de verbinding klaar is. Zo gaat het bericht (bijv. een eenmalig
    // commando van de gebruiker) niet verloren.
    stuurSocket->write(bericht.toUtf8());
    stuurSocket->flush();
}

QString SocketCommunicatieRPIWEMOS::ontvangData() {
    QMutexLocker lock(&dataMutex);
    QString data = laatsteData;
    laatsteData.clear();
    return data;
}

bool SocketCommunicatieRPIWEMOS::checkConnectieStatus() {
    if (laatsteOntvangstTijd == 0) return false;
    qint64 nu = QDateTime::currentMSecsSinceEpoch();
    return (nu - laatsteOntvangstTijd) < 5000;
}
