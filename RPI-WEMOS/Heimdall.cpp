#include "Heimdall.h"

#include <algorithm>
#include <string>
#include <cstring>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

Heimdall::Heimdall(QObject *parent) : QObject(parent) {}

Heimdall::~Heimdall() {
    gestopt = true;

    // Sluit de luister-socket; dit breekt accept() af in de accept-thread.
    if (serverFd >= 0) {
        ::shutdown(serverFd, SHUT_RDWR);
        ::close(serverFd);
        serverFd = -1;
    }
    if (acceptThread.joinable()) acceptThread.join();

    std::lock_guard<std::mutex> lock(clientsMutex);
    for (int fd : clients) ::close(fd);
    clients.clear();
}

bool Heimdall::start(int p) {
    poort = p;

    serverFd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd < 0) return false;

    int opt = 1;
    ::setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in address;
    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // luister op alle interfaces (RJ45 én WiFi)
    address.sin_port = htons(poort);

    if (::bind(serverFd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        ::close(serverFd);
        serverFd = -1;
        return false;
    }
    if (::listen(serverFd, 8) < 0) {
        ::close(serverFd);
        serverFd = -1;
        return false;
    }

    acceptThread = std::thread(&Heimdall::accepteerLus, this);
    return true;
}

void Heimdall::accepteerLus() {
    while (!gestopt) {
        struct sockaddr_in clientAddr;
        socklen_t len = sizeof(clientAddr);
        int clientFd = ::accept(serverFd, (struct sockaddr*)&clientAddr, &len);

        if (clientFd < 0) {
            if (gestopt) break;
            continue;
        }

        {
            std::lock_guard<std::mutex> lock(clientsMutex);
            clients.push_back(clientFd);
        }
        emit logBericht(QString("Bifrost: nieuwe client verbonden (%1)")
                            .arg(inet_ntoa(clientAddr.sin_addr)));

        // Eén lees-thread per client (detached) — een Wemos die data stuurt.
        std::thread(&Heimdall::verwerkClient, this, clientFd).detach();
    }
}

void Heimdall::verwerkClient(int clientFd) {
    std::string buffer;
    char temp[512];

    while (!gestopt) {
        ssize_t n = ::recv(clientFd, temp, sizeof(temp), 0);
        if (n <= 0) break; // 0 = client weg, <0 = fout

        buffer.append(temp, n);

        // Verwerk volledige regels (Runes) uit de buffer.
        size_t pos;
        while ((pos = buffer.find('\n')) != std::string::npos) {
            std::string regel = buffer.substr(0, pos);
            buffer.erase(0, pos + 1);
            if (!regel.empty() && regel.back() == '\r') regel.pop_back();
            if (regel.empty()) continue;

            // Splits op de eerste spatie: topic + payload.
            size_t sp = regel.find(' ');
            QString topic, payload;
            if (sp == std::string::npos) {
                topic = QString::fromStdString(regel);
            } else {
                topic = QString::fromStdString(regel.substr(0, sp));
                payload = QString::fromStdString(regel.substr(sp + 1));
            }
            emit runeOntvangen(topic, payload); // queued naar de GUI-thread
        }
    }

    ::close(clientFd);
    std::lock_guard<std::mutex> lock(clientsMutex);
    clients.erase(std::remove(clients.begin(), clients.end(), clientFd), clients.end());
}

void Heimdall::publiceer(const QString &topic, const QString &payload) {
    std::string rune = topic.toStdString();
    rune += ' ';
    rune += payload.toStdString();
    rune += '\n';

    std::lock_guard<std::mutex> lock(clientsMutex);
    for (int fd : clients) {
        ::send(fd, rune.c_str(), rune.size(), MSG_NOSIGNAL);
    }
}
