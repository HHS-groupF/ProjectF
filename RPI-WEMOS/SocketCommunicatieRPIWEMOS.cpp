#include "SocketCommunicatieRPIWEMOS.h"
#include "SysteemConfig.h"

#include <QDateTime>
#include <cstring>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

SocketCommunicatieRPIWEMOS::SocketCommunicatieRPIWEMOS(QObject *parent) : QObject(parent) {}

SocketCommunicatieRPIWEMOS::~SocketCommunicatieRPIWEMOS() {
    gestopt = true;

    // Luister-socket sluiten breekt accept() af in de thread.
    if (serverFd >= 0) {
        ::shutdown(serverFd, SHUT_RDWR);
        ::close(serverFd);
        serverFd = -1;
    }
    if (luisterThread.joinable()) luisterThread.join();

    std::lock_guard<std::mutex> lock(stuurMutex);
    if (stuurFd >= 0) { ::close(stuurFd); stuurFd = -1; }
}

bool SocketCommunicatieRPIWEMOS::verbind() {
    // 1. Luister-socket voor binnenkomende sensordata.
    serverFd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd < 0) return false;

    int opt = 1;
    ::setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in address;
    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // luistert op alle interfaces (RJ45 én WiFi)
    address.sin_port = htons(Config::POORT_WEMOS_DATA);

    if (::bind(serverFd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        ::close(serverFd); serverFd = -1; return false;
    }
    if (::listen(serverFd, 4) < 0) {
        ::close(serverFd); serverFd = -1; return false;
    }

    luisterThread = std::thread(&SocketCommunicatieRPIWEMOS::luisterLus, this);

    // 2. Probeer alvast de stuur-verbinding op te zetten (mag falen; wordt
    //    bij de eerste verzendData opnieuw geprobeerd).
    zorgVoorStuurVerbinding();
    return true;
}

void SocketCommunicatieRPIWEMOS::luisterLus() {
    while (!gestopt) {
        struct sockaddr_in clientAddr;
        socklen_t len = sizeof(clientAddr);
        int clientFd = ::accept(serverFd, (struct sockaddr*)&clientAddr, &len);
        if (clientFd < 0) {
            if (gestopt) break;
            continue;
        }

        emit nieuwLogBericht(QString("RPI-BUS verbonden (%1)").arg(inet_ntoa(clientAddr.sin_addr)));

        // Lees tot de client sluit; alles wordt aan laatsteData toegevoegd.
        char temp[1024];
        while (!gestopt) {
            ssize_t n = ::recv(clientFd, temp, sizeof(temp), 0);
            if (n <= 0) break;
            {
                std::lock_guard<std::mutex> lock(dataMutex);
                laatsteData += QString::fromUtf8(temp, (int)n);
            }
            laatsteOntvangstTijd = QDateTime::currentMSecsSinceEpoch();
        }
        ::close(clientFd);
    }
}

bool SocketCommunicatieRPIWEMOS::zorgVoorStuurVerbinding() {
    std::lock_guard<std::mutex> lock(stuurMutex);
    if (stuurFd >= 0) return true;

    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) return false;

    struct sockaddr_in serv;
    std::memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(Config::POORT_RPIBUS_COMMANDS);

    if (inet_pton(AF_INET, Config::RPI_BUS_IP.toUtf8().constData(), &serv.sin_addr) <= 0) {
        ::close(fd); return false;
    }
    if (::connect(fd, (struct sockaddr*)&serv, sizeof(serv)) < 0) {
        ::close(fd); return false;
    }
    stuurFd = fd;
    return true;
}

void SocketCommunicatieRPIWEMOS::verzendData(const QString &bericht) {
    QByteArray data = bericht.toUtf8();

    {
        std::lock_guard<std::mutex> lock(stuurMutex);
        if (stuurFd >= 0) {
            if (::send(stuurFd, data.constData(), data.size(), MSG_NOSIGNAL) >= 0) return;
            // Verzenden mislukt → sluiten en hieronder opnieuw verbinden.
            ::close(stuurFd); stuurFd = -1;
        }
    }

    // Niet (meer) verbonden: opnieuw verbinden en alsnog versturen.
    if (zorgVoorStuurVerbinding()) {
        std::lock_guard<std::mutex> lock(stuurMutex);
        if (stuurFd >= 0) {
            ::send(stuurFd, data.constData(), data.size(), MSG_NOSIGNAL);
        }
    }
}

QString SocketCommunicatieRPIWEMOS::ontvangData() {
    std::lock_guard<std::mutex> lock(dataMutex);
    QString data = laatsteData;
    laatsteData.clear();
    return data;
}

bool SocketCommunicatieRPIWEMOS::checkConnectieStatus() {
    qint64 t = laatsteOntvangstTijd.load();
    if (t == 0) return false;
    qint64 nu = QDateTime::currentMSecsSinceEpoch();
    return (nu - t) < 5000;
}
