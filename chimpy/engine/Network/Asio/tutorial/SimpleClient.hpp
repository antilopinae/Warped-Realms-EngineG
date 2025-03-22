#pragma once

#include "NetCommon.hpp"
#include "NetMessage.hpp"
#include "NetTSQueue.hpp"
#include "NetConnection.hpp"
#include "NetClient.hpp"
#include "NetServer.hpp"

class CustomClient: public EngineG::Network::ClientInterface<EngineG::Network::CustomMsgTypes>
{
public:
    void PingServer()
    {
        EngineG::Network::Message<EngineG::Network::CustomMsgTypes> msg;
        msg.header.id = EngineG::Network::CustomMsgTypes::ServerPing;

        std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
        msg << timeNow;
        Send(msg);
    }

    void MessageAll()
    {
        EngineG::Network::Message<EngineG::Network::CustomMsgTypes> msg;
        msg.header.id = EngineG::Network::CustomMsgTypes::MessageAll;
        Send(msg);
    }

    // bool FireBullet(float x, float y)
    // {
    //     EngineG::Network::Message<CustomMsgType> msg;
    //     msg.header.id = CustomMsgType::FireBullet;
    //     msg << x << y;
    //     // Send(msg);
    //     return true;
    // }
};

void startCustomClient() {
    CustomClient client;
    client.Connect("127.0.0.1", 60000);

    bool bQuit = false;
    while (!bQuit)
    {
        int a;
        std::cin >> a;
        if (a==1)
        {
            client.PingServer();
        }
        else if (a==2)
        {
            client.MessageAll();
        }
        else if (a==3)
        {
            bQuit = true;
        }

        std::cout << "[CLIENT]: One tick" << std::endl;

        if (client.IsConnected())
        {
            if (!client.IncomingMessages().empty())
            {
                auto message = client.IncomingMessages().pop_front().message;

                switch (message.header.id)
                {
                    case EngineG::Network::CustomMsgTypes::ServerAccept:
                    {
                        // Server has responded to a ping request
                        std::cout << "[CLIENT]: Server Accepted Connection\n";
                    }
                    break;
                    case EngineG::Network::CustomMsgTypes::ServerPing:
                    {
                        std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
                        std::chrono::system_clock::time_point timePrev;
                        message >> timePrev;
                        std::cout << "[CLIENT]: Ping" << std::chrono::duration<double>(timeNow - timePrev).count() << std::endl;
                    }
                    break;

                    case EngineG::Network::CustomMsgTypes::ServerMessage:
                    {
                        uint32_t clientID;
                        message >> clientID;
                        std::cout << "[CLIENT]: ServerMessage Hello from: " << clientID << std::endl;
                    }
                    break;
                }
            }
        }
        else
        {
            std::cout << "Server down" << std::endl;
            bQuit = true;
        }
    }

#if 0
    client.FireBullet(5, 2);
    EngineG::Network::Message<CustomMsgType> message;
    message.header.id = CustomMsgType::FireBullet;

    int a = 1;
    bool b = true;
    float c = 3.1456f;

    struct{
    float x;
    float y;
    } d[5];

    message << a << b << c << d;

    a = 99;
    b = false;
    c = 99.0f;

    message >> d >> c >> b >> a;
#endif
}