#pragma once

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

namespace Net {

class Server {
private:
    std::string host;
    int port;
    int server_fd;
    sockaddr_in server_addr{};

public:
    Server(int port, const std::string& host);
    ~Server();
    void start();
    void stop();
private:
    void receive(int client_fd);
    void process(int client_fd);
    void send(int client_fd);
    void init();
};

Server::Server(int port, const std::string& host) : host(host), port(port) {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "Error creating socket\n";
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(host.c_str());
    server_addr.sin_port = htons(port);
}

Server::~Server() {
    stop();
}

void Server::init() {
    if (bind(server_fd, reinterpret_cast<sockaddr *>(&server_addr), sizeof(server_addr)) < 0) {
        std::cerr << "Error binding socket\n";
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 5) < 0) {
        std::cerr << "Error listening on socket\n";
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    std::cout << "Server listening on " << host << ":" << port << "\n";
}

void Server::start() {
    init();
    while (true) {
        int client_fd = accept(server_fd, nullptr, nullptr);
        if (client_fd < 0) {
            std::cerr << "Error accepting connection\n";
            continue;
        }
        std::thread(&Server::receive, this, client_fd).detach();
    }
}

void Server::stop() {
    close(server_fd);
    std::cout << "Server stopped.\n";
}

void Server::receive(int client_fd) {
    char buffer[1024] = {0};
    int bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        std::cout << "Received: " << buffer << "\n";
        process(client_fd);
    } else {
        std::cerr << "Error receiving message\n";
    }
    close(client_fd);
}

void Server::process(int client_fd) {
    std::cout << "Processing data...\n";
    send(client_fd);
}

void Server::send(int client_fd) {
    std::string msg = "Hello from server!";
    if (::send(client_fd, msg.c_str(), msg.length(), 0) < 0) {
        std::cerr << "Error sending message\n";
    }
}


}