#pragma once

#include "NetCommon.hpp"
#include "NetTSQueue.hpp"
#include "NetMessage.hpp"
#include "NetConnection.hpp"

#include <deque>
#include <cstddef>

namespace EngineG::Network {

template<typename T>
class ServerInterface {
public:
    ServerInterface(uint16_t port);
    virtual ~ServerInterface();

    bool Start();
    void Stop();

    void SendMessageClient(std::shared_ptr<Connection<T>> client, const Message<T>& message);
    void SendMessageAllClients(const Message<T>& message, std::shared_ptr<Connection<T>> pIgnoreClient = nullptr);
    void Update(size_t nMaxMessages = static_cast<size_t>(-1), bool bWait = false);

protected:
    virtual bool OnClientConnect(std::shared_ptr<Connection<T>> client);
    virtual void OnClientDisconnect(std::shared_ptr<Connection<T>> client);
    virtual void OnMessage(std::shared_ptr<Connection<T>> client, const Message<T>& message);

    void WaitForClientConnection();

    TSQueue<OwnedMessage<T>> mQMessagesIn;
    std::deque<std::shared_ptr<Connection<T>>> mDeqConnections;
    asio::io_context mIoContext;
    std::thread mThreadContext;
    asio::ip::tcp::acceptor mAcceptor;
    uint32_t nIDCounter = 10000;
};

template<typename T>
ServerInterface<T>::ServerInterface(uint16_t port)
    : mAcceptor(mIoContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
{
}

template<typename T>
ServerInterface<T>::~ServerInterface() {
    Stop();
}

template<typename T>
bool ServerInterface<T>::Start() {
    try {
        WaitForClientConnection();
        mThreadContext = std::thread([this]() {
             try {
                mIoContext.run();
             } catch (const std::exception& e) {
                 std::cerr << "Server ASIO Exception in context thread: " << e.what() << std::endl;
             }
        });
    } catch (const std::exception& e) {
        std::cerr << "[SERVER] Exception during Start(): " << e.what() << std::endl;
        return false;
    }
    std::cout << "[SERVER] Started successfully. Listening..." << std::endl;
    return true;
}

template<typename T>
void ServerInterface<T>::Stop() {
    mIoContext.stop();
    if (mThreadContext.joinable()) {
        mThreadContext.join();
    }
    std::cout << "[SERVER] Stopped." << std::endl;
}

template<typename T>
void ServerInterface<T>::WaitForClientConnection() {
    mAcceptor.async_accept(
        [this](std::error_code ec, asio::ip::tcp::socket socket) {
            if (!ec) {
                std::cout << "[SERVER] New connection accepted from: " << socket.remote_endpoint() << std::endl;
                std::shared_ptr<Connection<T>> newConnection =
                    std::make_shared<Connection<T>>(
                        Connection<T>::Owner::Server,
                        mIoContext,
                        std::move(socket),
                        mQMessagesIn
                    );

                if (OnClientConnect(newConnection)) {
                    mDeqConnections.push_back(std::move(newConnection));
                    uint32_t newClientID = nIDCounter++;
                    mDeqConnections.back()->ConnectToClient(newClientID);
                    std::cout << "[" << newClientID << "] Connection approved and initialized." << std::endl;
                } else {
                    std::cout << "[SERVER] Connection denied by OnClientConnect." << std::endl;
                }
            } else {
                std::cerr << "[SERVER] New Connection Error: " << ec.message() << std::endl;
            }

            if (mAcceptor.is_open()) {
                WaitForClientConnection();
            }
        }
    );
}

template<typename T>
void ServerInterface<T>::SendMessageClient(std::shared_ptr<Connection<T>> client, const Message<T>& message) {
    if (client && client->IsConnected()) {
        client->Send(message);
    } else {
        OnClientDisconnect(client);
        if(client) {
            mDeqConnections.erase(
                std::remove(mDeqConnections.begin(), mDeqConnections.end(), client),
                mDeqConnections.end()
            );
        }
    }
}

template<typename T>
void ServerInterface<T>::SendMessageAllClients(const Message<T>& message, std::shared_ptr<Connection<T>> pIgnoreClient) {
    bool bInvalidClientExists = false;
    for (auto& client : mDeqConnections) {
        if (client && client->IsConnected()) {
            if (client != pIgnoreClient) {
                client->Send(message);
            }
        } else {
            OnClientDisconnect(client);
            client.reset();
            bInvalidClientExists = true;
        }
    }

    if (bInvalidClientExists) {
        mDeqConnections.erase(
            std::remove(mDeqConnections.begin(), mDeqConnections.end(), nullptr),
            mDeqConnections.end()
        );
    }
}

template<typename T>
void ServerInterface<T>::Update(size_t nMaxMessages, bool bWait) {
    if (bWait) {
        mQMessagesIn.wait();
    }

    size_t nMessageCount = 0;
    while (nMessageCount < nMaxMessages && !mQMessagesIn.empty()) {
        auto msg = mQMessagesIn.pop_front();
        OnMessage(msg.remote, msg.message);
        nMessageCount++;
    }
}

template<typename T>
bool ServerInterface<T>::OnClientConnect(std::shared_ptr<Connection<T>> /*client*/) {
    return false; // Deny by default
}

template<typename T>
void ServerInterface<T>::OnClientDisconnect(std::shared_ptr<Connection<T>> /*client*/) {
}

template<typename T>
void ServerInterface<T>::OnMessage(std::shared_ptr<Connection<T>> /*client*/, const Message<T>& /*message*/) {
}

} // namespace EngineG::Network