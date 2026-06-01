#include "SocketCommunicatieRPIBUS.h"
#include "SysteemConfig.h"
#include <QDebug>

SocketCommunicatieRPIBUS::SocketCommunicatieRPIBUS(QObject *parent)
    : QObject(parent)
{
    commandServer = new QTcpServer(this);
    sensorSocket = new QTcpSocket(this);

    connect(commandServer, &QTcpServer::newConnection, this, &SocketCommunicatieRPIBUS::nieuweVerbinding);
}

SocketCommunicatieRPIBUS::~SocketCommunicatieRPIBUS() {
    if (commandServer->isListening()) commandServer->close();
    if (sensorSocket->isOpen()) sensorSocket->close();
}

void SocketCommunicatieRPIBUS::start() {
    if(commandServer->listen(QHostAddress::Any, Config::POORT_RPIBUS_COMMANDS)) {
        qDebug() << "[RPI-BUS] Server luistert naar commando's op poort" << Config::POORT_RPIBUS_COMMANDS;
    }
    sensorSocket->connectToHost(Config::RPI_WEMOS_IP, Config::POORT_WEMOS_DATA);
}

void SocketCommunicatieRPIBUS::nieuweVerbinding() {
    QTcpSocket* client = commandServer->nextPendingConnection();
    connect(client, &QTcpSocket::readyRead, [this, client]() {
        QString inkomend = client->readAll().trimmed();
        emit inkomendCommando(inkomend);
    });
    connect(client, &QTcpSocket::disconnected, client, &QTcpSocket::deleteLater);
}

void SocketCommunicatieRPIBUS::verzendData(const QString &bericht) {
    if (sensorSocket->state() == QAbstractSocket::ConnectedState) {
        sensorSocket->write(bericht.toUtf8());
        sensorSocket->flush();
    } else if (sensorSocket->state() == QAbstractSocket::UnconnectedState) {
        sensorSocket->connectToHost(Config::RPI_WEMOS_IP, Config::POORT_WEMOS_DATA);
    }
}