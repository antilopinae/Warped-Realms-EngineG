#pragma once

#include "NetCommon.hpp"
#include "NetMessage.hpp"
#include "NetTSQueue.hpp"
#include "NetConnection.hpp"

namespace EngineG::Network
{

template<typename T>
class ClientInterface
{
public:
    ClientInterface(): mSocket(mIOContext)
    {
    }

    virtual ~ClientInterface()
    {
        Disconnect();
    }

public:
    bool Connect(const std::string& host, const uint16_t port)
    {
        try
        {
            // Resolve hostname/ip-address into tangiable physical address
            asio::ip::tcp::resolver resolver(mIOContext);
            asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));

            // Create connection
            mConnection = std::make_unique<Connection<T>>(
                Connection<T>::Owner::Client,
                mIOContext,
                asio::ip::tcp::socket(mIOContext),
                mQMessagesIn
            );

            // Tell the connection object to connect to server
            mConnection->ConnectToServer(endpoints);

            // Start Context Thread
            mThreadContext = std::thread([this]() { mIOContext.run(); });
        }
        catch (const std::exception& e)
        {
            std::cerr << "Client Exception: " << e.what() << std::endl;
            return false;
        }

        return true;
    }

    void Send(const Message<T>& msg)
    {
        if (IsConnected())
            mConnection->Send(msg);
    }

    void Disconnect()
    {
        if (IsConnected())
        {
            mConnection->Disconnect();
        }

        mIOContext.stop();
        if (mThreadContext.joinable())
        {
            mThreadContext.join();
        }

        mConnection.release();
    }

    bool IsConnected() const
    {
        if (mConnection)
            return mConnection->IsConnected();
        return false;
    }

    TSQueue<OwnedMessage<T>>& IncomingMessages()
    {
        return mQMessagesIn;
    }

protected:
    asio::io_context mIOContext;
    std::thread mThreadContext;
    asio::ip::tcp::socket mSocket;
    std::unique_ptr<Connection<T>> mConnection;

private:
    // Thread safe queue of incoming messages from server
    TSQueue<OwnedMessage<T>> mQMessagesIn;
};

}