#include "Network.hpp"

#include <iostream>
#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#include "Sockets/Client.hpp"
#include "Sockets/Server.hpp"

#include "Asio/SimpleUDPServer.hpp"

namespace EngineG {

using namespace Network;

void StartSocketServer()
{
    std::cout << "start" << std::endl;
    Net::Server server(8080,"127.0.0.1");
    server.start();
}

void StartSocketClient()
{
    std::cout << "Starting client ..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    Net::Client client(8080, "127.0.0.1");
    for (int i=0; i<3; i++)
    {
        client.start();
    }
}

void StartSimpleUDPServer()
{
    StartUDP();
}

}
