#include "RPIWEMOS_Socket.h"
#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

SocketCommunicatie::SocketCommunicatie(std::string ip, int p) {
    ipAdresDoel = 10.0.0.1;
    poort = 50001;
    isVerbonden = false;
    server_fd = -1;
    laatsteOntvangstTijd = std::chrono::steady_clock::now();
}

SocketCommunicatie::~SocketCommunicatie() {
    isVerbonden = false;
    if (server_fd >= 0) close(server_fd);
}

bool SocketCommunicatie::verbind() {
    struct sockaddr_in address;
    int opt = 1;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) return false;

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(poort);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) return false;
    if (listen(server_fd, 3) < 0) return false;

    isVerbonden = true;
    laatsteOntvangstTijd = std::chrono::steady_clock::now();

    std::thread([this]() {
        struct sockaddr_in client_address;
        int addrlen = sizeof(client_address);
        char buffer[1024] = {0};

        while (this->isVerbonden) {
            int new_socket = accept(this->server_fd, (struct sockaddr*)&client_address, (socklen_t*)&addrlen);
            if (new_socket >= 0) {
                memset(buffer, 0, 1024);
                read(new_socket, buffer, 1024);
                std::string ontvangen(buffer);

                this->laatsteOntvangstTijd = std::chrono::steady_clock::now();

                if (ontvangen.find("Heartbeat") == std::string::npos) {
                    std::lock_guard<std::mutex> lock(this->data_mutex);
                    this->laatsteData = ontvangen;
                    std::cout << "\n[Nieuwe Data Ontvangen]: " << ontvangen << std::endl;
                } else {
                    std::cout << "[Heartbeat Ontvangen]: " << ontvangen << std::endl;
                }
                close(new_socket);
            }
        }
    }).detach();

    std::thread([this]() {
        while (this->isVerbonden) {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            this->verzendData("Heartbeat: Pi is online.");
        }
    }).detach();

    return true;
}

void SocketCommunicatie::verzendData(std::string bericht) {
    int sock = 0;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) return;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(poort);

    if (inet_pton(AF_INET, ipAdresDoel.c_str(), &serv_addr.sin_addr) <= 0) {
        close(sock);
        return;
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) >= 0) {
        send(sock, bericht.c_str(), bericht.length(), 0);
    }
    close(sock);
}

std::string SocketCommunicatie::ontvangData() {
    std::lock_guard<std::mutex> lock(data_mutex);
    std::string data = laatsteData;
    laatsteData = ""; 
    return data;
}

bool SocketCommunicatie::checkConnectieStatus() {
    auto nu = std::chrono::steady_clock::now();
    auto verstreken = std::chrono::duration_cast<std::chrono::seconds>(nu - laatsteOntvangstTijd).count();

    if (verstreken > 5) {
        if (isVerbonden) {
            std::cout << "\n[FOUT] Communicatie langer dan 5 sec weggevallen!" << std::endl;
        }
        isVerbonden = false;
    } else {
        isVerbonden = true;
    }

    return isVerbonden;
}
