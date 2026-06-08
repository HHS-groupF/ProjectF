#include "SocketCommunicatieRPIBUS.h"
#include "SysteemConfig.h"

#include <QDebug>
#include <string>
#include <cstring>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

SocketCommunicatieRPIBUS::SocketCommunicatieRPIBUS(QObject *parent) : QObject(parent) {}

SocketCommunicatieRPIBUS::~SocketCommunicatieRPIBUS() {
    gestopt = true;

    if (serverFd >= 0) {
        ::shutdown(serverFd, SHUT_RDWR);
        ::close(serverFd);
        serverFd = -1;
    }
    if (luisterThread.joinable()) luisterThread.join();

    std::lock_guard<std::mutex> lock(sensorMutex);
    if (sensorFd >= 0) { ::close(sensorFd); sensorFd = -1; }
}

void SocketCommunicatieRPIBUS::start() {
    //SAFEGUARD: Voorkom dubbel starten en crashen
    if (luisterThread.joinable()) {
        qDebug() << "[RPI-BUS] Server is al gestart! Tweede startpoging genegeerd.";
        return;
    }

    serverFd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd < 0) { qDebug() << "[RPI-BUS] socket() mislukt"; return; }

    int opt = 1;
    ::setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in address;
    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons(Config::POORT_RPIBUS_COMMANDS);

    if (::bind(serverFd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        qDebug() << "[RPI-BUS] bind mislukt"; ::close(serverFd); serverFd = -1; return;
    }
    if (::listen(serverFd, 4) < 0) {
        qDebug() << "[RPI-BUS] listen mislukt"; ::close(serverFd); serverFd = -1; return;
    }

    qDebug() << "[RPI-BUS] Server luistert naar commando's op poort" << Config::POORT_RPIBUS_COMMANDS;

    luisterThread = std::thread(&SocketCommunicatieRPIBUS::luisterLus, this);

    zorgVoorSensorVerbinding();

    // --- HEARTBEAT TIMER STARTEN ---
    heartbeatTimer = new QTimer(this);
    connect(heartbeatTimer, &QTimer::timeout, this, &SocketCommunicatieRPIBUS::verzendHeartbeat);
    heartbeatTimer->start(2000); 
}


void SocketCommunicatieRPIBUS::luisterLus() {
    while (!gestopt) {
        struct sockaddr_in clientAddr;
        socklen_t len = sizeof(clientAddr);
        int clientFd = ::accept(serverFd, (struct sockaddr*)&clientAddr, &len);
        if (clientFd < 0) {
            if (gestopt) break;
            continue;
        }

        std::string buffer;
        char temp[1024];
        while (!gestopt) {
            ssize_t n = ::recv(clientFd, temp, sizeof(temp), 0);
            if (n <= 0) break;
            buffer.append(temp, n);

            size_t pos;
            while ((pos = buffer.find('\n')) != std::string::npos) {
                QString cmd = QString::fromStdString(buffer.substr(0, pos)).trimmed();
                buffer.erase(0, pos + 1);
                if (!cmd.isEmpty()) emit inkomendCommando(cmd);
            }
        }
        QString rest = QString::fromStdString(buffer).trimmed();
        if (!rest.isEmpty()) emit inkomendCommando(rest);

        ::close(clientFd);
    }
}

bool SocketCommunicatieRPIBUS::zorgVoorSensorVerbinding() {
    std::lock_guard<std::mutex> lock(sensorMutex);
    if (sensorFd >= 0) return true;

    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) return false;

    struct sockaddr_in serv;
    std::memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(Config::POORT_WEMOS_DATA);

    if (inet_pton(AF_INET, Config::RPI_WEMOS_IP.toUtf8().constData(), &serv.sin_addr) <= 0) {
        ::close(fd); return false;
    }
    if (::connect(fd, (struct sockaddr*)&serv, sizeof(serv)) < 0) {
        ::close(fd); return false;
    }
    sensorFd = fd;
    return true;
}

void SocketCommunicatieRPIBUS::verzendData(const QString &bericht) {
    QByteArray data = bericht.toUtf8();

    {
        std::lock_guard<std::mutex> lock(sensorMutex);
        if (sensorFd >= 0) {
            if (::send(sensorFd, data.constData(), data.size(), MSG_NOSIGNAL) >= 0) return;
            ::close(sensorFd); sensorFd = -1;
        }
    }

    if (zorgVoorSensorVerbinding()) {
        std::lock_guard<std::mutex> lock(sensorMutex);
        if (sensorFd >= 0) {
            ::send(sensorFd, data.constData(), data.size(), MSG_NOSIGNAL);
        }
    }
}

void SocketCommunicatieRPIBUS::verzendHeartbeat() {
    verzendData("HEARTBEAT\n");
}
