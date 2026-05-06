#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <chrono>

#define PORT 50001

const std::string TARGET_IP = "10.0.0.2";

void start_server() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = { 0 };

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 3);

    while (true) {
        new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (new_socket >= 0) {
            memset(buffer, 0, 1024);
            read(new_socket, buffer, 1024);
            std::cout << "\n[Ontvangen]: " << buffer << std::endl;
            std::cout << "Bericht verzenden: " << std::flush;
            close(new_socket);
        }
    }
}

void stuur_bericht(std::string ip, std::string tekst) {
    int sock = 0;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) return;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr) <= 0) {
        close(sock);
        return;
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) >= 0) {
        send(sock, tekst.c_str(), tekst.length(), 0);
    }
    close(sock);
}

void start_heartbeat(std::string ip) {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(10));
        stuur_bericht(ip, "Automatische Heartbeat: Pi is nog online.");
    }
}

int main() {
    std::thread ontvanger(start_server);
    ontvanger.detach();

    std::thread timer_bericht(start_heartbeat, TARGET_IP);
    timer_bericht.detach();

    while (true) {
        std::string input;
        std::cout << "Bericht verzenden: ";
        std::getline(std::cin, input);

        if (input == "stop") break;
        if (input.empty()) continue;

        stuur_bericht(TARGET_IP, input);
    }

    return 0;
}