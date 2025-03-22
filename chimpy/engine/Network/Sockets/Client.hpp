#pragma once

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

namespace Net {

class Client {
private:
    char buffer[1024] = {0};
    std::string host;
    int port;
    int server_fd;
    sockaddr_in server_addr{};

public:
    Client(int port, const std::string& host);
    ~Client();
    void start();
    void stop();
private:
    void send();
    void receive();
    void init();
};

Client::Client(int port, const std::string& host) : host(host), port(port) {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "Error creating socket\n";
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, host.c_str(), &server_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address\n";
        exit(EXIT_FAILURE);
    }
}

Client::~Client() {
    stop();
}

void Client::init() {
    if (connect(server_fd, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection failed\n";
        exit(EXIT_FAILURE);
    }
    std::cout << "Connected to server\n";
}

void Client::start() {
    init();
    send();
    receive();
}

void Client::stop() {
    close(server_fd);
    std::cout << "Client stopped.\n";
}

void Client::send() {
    std::string msg = "Hello, server!";
    if (::send(server_fd, msg.c_str(), msg.length(), 0) < 0) {
        std::cerr << "Error sending message\n";
    }
}

void Client::receive() {
    int bytes_received = recv(server_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        std::cout << "Message from server: " << buffer << "\n";
    } else {
        std::cerr << "Error receiving message\n";
    }
}


}
