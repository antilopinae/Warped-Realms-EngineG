#pragma once

#include "NetCommon.hpp"
#include "NetTSQueue.hpp"
#include "NetMessage.hpp"
#include "NetConnection.hpp"

#include <algorithm>

namespace EngineG::Network
{

template<typename T>
class ServerInterface
{
public:
    ServerInterface(uint16_t port): mAcceptor(mIoContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
    {

    }

    virtual ~ServerInterface()
    {
        Stop();
    }

    bool Start()
    {
        try
        {
            WaitForClientConnection();

            mThreadContext = std::thread([this]() { mIoContext.run(); });
        } catch (std::exception& e)
        {
            std::cerr <<"[SERVER] Exception: " << e.what() << std::endl;
            return false;
        }

        std::cout << "[SERVER] Started" << std::endl;
        return true;
    }

    void Stop()
    {
        mIoContext.stop();

        if (mThreadContext.joinable()) mThreadContext.join();

        // Inform someone, anybody, if they care
        std::cout << "[SERVER] Stopped" << std::endl;
    }

    // Async - instruct asio to wait for connection
    void WaitForClientConnection()
    {
        mAcceptor.async_accept([this](std::error_code ec, asio::ip::tcp::socket socket)
        {
            if (!ec)
            {
                std::cout << "[SERVER] New connection: " << socket.remote_endpoint() << std::endl;

                std::shared_ptr<Connection<T>> newConnection = std::make_shared<Connection<T>>(
                    Connection<T>::Owner::Server,
                    mIoContext,
                    std::move(socket),
                    mQMessagesIn
                    );

                // Give the user server a chance to deny connection
                if (OnClientConnect(newConnection))
                {
                    // Connection allowed
                    mDeqConnections.push_back(std::move(newConnection));
                    mDeqConnections.back() -> ConnectToClient(nIDCounter++);

                    std::cout << "[" << mDeqConnections.back() -> GetID() << "] Connection Approved" << std::endl;
                }
                else
                {
                    std::cout << "[-----] Connection Denied" << std::endl;
                }
            }
            else
            {
                std::cerr << "[SERVER] New Connection Error: " << ec.message() << std::endl;
            }

            WaitForClientConnection();
        });
    }

    // Send a message to a specific client
    void SendMessageClient(std::shared_ptr<Connection<T>> client, const Message<T>& message)
    {
        if (client && client->isConnected())
        {
            client->Send(message);
        }
        else
        {
            OnClientDisconnect(client);
            client.reset();
            mDeqConnections.erase(
                std::remove(mDeqConnections.begin(),
                mDeqConnections.end(), client),
                mDeqConnections.end()
                );
        }
    }

    // Send message to all clients
    void SendMessageAllClients(const Message<T>& message, std::shared_ptr<Connection<T>> pIgnoreClient = nullptr)
    {
        bool bInvalidClientExists = false;

        for (auto& client : mDeqConnections)
        {
            if (client && client->IsConnected())
            {
                if (client != pIgnoreClient)
                    client->Send(message);
            }
            else
            {
                // The client couldnt be contacted, so assume it has disconnected
                OnClientDisconnect(client);
                client.reset();
                bInvalidClientExists = true;
            }
        }

        if (bInvalidClientExists)
        {
            mDeqConnections.erase(
                std::remove(mDeqConnections.begin(), mDeqConnections.end(), nullptr),
                mDeqConnections.end()
                );
        }
    }

    void Update(size_t nMaxMessages = -1, bool bWait = false)
    {
        if (bWait) mQMessagesIn.Wait();

        size_t nMessageCount = 0;
        while (nMessageCount < nMaxMessages && !mQMessagesIn.empty())
        {
            auto message = mQMessagesIn.pop_front();
            OnMessage(message.remote, message.message);
            nMessageCount++;
        }
    }

protected:
    // Called when a client connects, you can veto the connection by returning false
    virtual bool OnClientConnect(std::shared_ptr<Connection<T>> client)
    {
        return false;
    }

    // Called when a client appears to have disconnected
    virtual void OnClientDisconnect(std::shared_ptr<Connection<T>> client)
    {

    }

    // Called when a message arrives
    virtual void OnMessage(std::shared_ptr<Connection<T>> client, const Message<T>& message)
    {

    }

protected:
    // Thread safe queue for incoming message packets
    TSQueue<OwnedMessage<T>> mQMessagesIn;

    // Container of active validated connections
    std::deque<std::shared_ptr<Connection<T>>> mDeqConnections;

    asio::io_context mIoContext;
    std::thread mThreadContext;

    asio::ip::tcp::acceptor mAcceptor;
    //Clients will be identified in the "wider system" via an ID;
    uint32_t nIDCounter = 10000;
};

}

