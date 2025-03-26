#include "Network.hpp"

#include <iostream>
#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#include "SimpleUDPServer.hpp"

namespace EngineG {

using namespace Network;

void StartSocketServer()
{
}

void StartSocketClient()
{
}

void StartSimpleUDPServer()
{
    StartUDP();
}

}
