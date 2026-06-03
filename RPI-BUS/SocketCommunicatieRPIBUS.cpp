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
        QString inkomend = QString::fromUtf8(client->readAll().trimmed());
        emit inkomendCommando(inkomend);
    });
    connect(client, &QTcpSocket::disconnected, client, &QTcpSocket::deleteLater);
}

void SocketCommunicatieRPIBUS::verzendData(const QString &bericht) {
    // 1. Verbind als dat nog niet zo is
    if (sensorSocket->state() == QAbstractSocket::UnconnectedState) {
        sensorSocket->connectToHost(Config::RPI_WEMOS_IP, Config::POORT_WEMOS_DATA);
    }

    // 2. Schrijf de data ALTIJD weg. Als de status 'Connecting' is, buffert Qt dit
    // in het geheugen en stuurt het weg zodra de verbinding open is. Geen dataverlies meer!
    sensorSocket->write(bericht.toUtf8());
    sensorSocket->flush();
}