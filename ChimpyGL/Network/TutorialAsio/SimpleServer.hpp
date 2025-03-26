#pragma once

#include <iostream>
#include "NetServer.hpp"

class CustomServer: public EngineG::Network::ServerInterface<EngineG::Network::CustomMsgTypes>
{
public:
    CustomServer(uint16_t port): EngineG::Network::ServerInterface<EngineG::Network::CustomMsgTypes>(port)
    {
    }

protected:
    bool OnClientConnect(
        std::shared_ptr<EngineG::Network::Connection<EngineG::Network::CustomMsgTypes>> client
        ) override
    {
        EngineG::Network::Message<EngineG::Network::CustomMsgTypes> msg;
        msg.header.id = EngineG::Network::CustomMsgTypes::ServerAccept;
        client->Send(msg);
        return true;
    }

    // Called when a client appears to have disconnected
    void OnClientDisconnect(
        std::shared_ptr<EngineG::Network::Connection<EngineG::Network::CustomMsgTypes>> client
        ) override
    {
        std::cout << "Removing client [" << client->GetID() << "]\n";
    }

    // Called when a message arrives
    void OnMessage(
        std::shared_ptr<EngineG::Network::Connection<EngineG::Network::CustomMsgTypes>> client,
        const EngineG::Network::Message<EngineG::Network::CustomMsgTypes>& message
        ) override
    {
        switch (message.header.id)
        {
            case EngineG::Network::CustomMsgTypes::ServerPing:
            {
                std::cout << "[" << client->GetID() << "]: Server Ping" << std::endl;
                client->Send(message);
            }
            break;

            case EngineG::Network::CustomMsgTypes::MessageAll:
            {
                std::cout << "[" << client->GetID() << "]: Message All" << std::endl;
                EngineG::Network::Message<EngineG::Network::CustomMsgTypes> msg;
                msg.header.id = EngineG::Network::CustomMsgTypes::ServerMessage;
                auto id = client->GetID();
                msg << id;
                SendMessageAllClients(msg, client);
            }
            break;
        }
    }
};

void startCustomServer() {
    CustomServer server(60000);
    server.Start();

    while (true)
    {
        server.Update(1, true);
    }
}