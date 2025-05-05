#pragma once

#include "NetCommon.hpp"
#include "NetTSQueue.hpp"
#include "NetMessage.hpp"
#include "NetConnection.hpp"

#include <string>

namespace EngineG::Network {

template<typename T>
class ClientInterface {
public:
    ClientInterface();
    virtual ~ClientInterface();

public:
    bool Connect(const std::string& host, const uint16_t port);
    void Disconnect();
    [[nodiscard]] bool IsConnected() const;
    void Send(const Message<T>& msg);
    TSQueue<OwnedMessage<T>>& IncomingMessages();

protected:
    asio::io_context mIOContext;
    std::thread mThreadContext;
    asio::ip::tcp::socket mSocket;
    std::unique_ptr<Connection<T>> mConnection;

private:
    TSQueue<OwnedMessage<T>> mQMessagesIn;
};

template<typename T>
ClientInterface<T>::ClientInterface() : mSocket(mIOContext) {}

template<typename T>
ClientInterface<T>::~ClientInterface() {
    Disconnect();
}

template<typename T>
bool ClientInterface<T>::Connect(const std::string& host, const uint16_t port) {
    try {
        asio::ip::tcp::resolver resolver(mIOContext);
        asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));

        mConnection = std::make_unique<Connection<T>>(
            Connection<T>::Owner::Client,
            mIOContext,
            asio::ip::tcp::socket(mIOContext),
            mQMessagesIn
        );

        mConnection->ConnectToServer(endpoints);

        mThreadContext = std::thread([this]() {
            try {
                mIOContext.run();
            } catch (const std::exception& e) {
                 std::cerr << "Client ASIO Exception in context thread: " << e.what() << std::endl;
            }
        });
    } catch (const std::exception& e) {
        std::cerr << "Client Connection Exception: " << e.what() << std::endl;
         mConnection.reset();
         if (mThreadContext.joinable()) {
             mIOContext.stop();
             mThreadContext.join();
         }
        return false;
    }
    return true;
}

template<typename T>
void ClientInterface<T>::Disconnect() {
    if (IsConnected()) {
        mConnection->Disconnect();
    }

    mIOContext.stop();

    if (mThreadContext.joinable()) {
        mThreadContext.join();
    }

    mConnection.reset();
}

template<typename T>
bool ClientInterface<T>::IsConnected() const {
    return mConnection && mConnection->IsConnected();
}

template<typename T>
void ClientInterface<T>::Send(const Message<T>& msg) {
    if (IsConnected()) {
        mConnection->Send(msg);
    } else {
         std::cerr << "Client Error: Cannot send message, not connected." << std::endl;
    }
}

template<typename T>
TSQueue<OwnedMessage<T>>& ClientInterface<T>::IncomingMessages() {
    return mQMessagesIn;
}

} // namespace EngineG::Network