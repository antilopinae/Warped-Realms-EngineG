#pragma once

#include <asio.hpp>

namespace EngineG::Network::UDP {

class Server {
public:
    Server(asio::io_context& ioContext, std::uint16_t port);

    void Start();
    void Terminate();

    void SetOnRegister(std::function<void(class User& curUser, class Session& curSession)> onRegister);
    void SetOnAuthorization(std::function<void(class User& curUser, class Session& curSession)> onAuthorization);

    void SetOnDisconnect(std::function<void(class Session& curSession)> onDisconnect);
    void SetOnConnect(std::function<void(class Session& curSession)> onConnect);

    void SetOnUserConnect(std::function<void(class User& curUser, class Session& curSession)> onUserConnect);
    void SetOnUserDisconnect(std::function<void(class User& curUser, class Session& curSession)> onUserDisconnect);

    void SetOnReceive(std::function<void(class Message& message, class Session& curSession)> onReceive);
    void SetOnSend(std::function<class Message&(class Message& message, class Session& curSession)> onSend);

    std::vector<class Message>& GetCriticalMessages();

    std::vector<class Message>& GetSendingLayer(ulong layer);
    std::vector<class Message>& GetReceivingLayer(ulong layer);
    void SendLayer(ulong layer);

    void SetTimeLayerHeight(float timeHeight);

    void SetMaxLayerMemoryBuffer(int maxSizeLayer);
    void SetMaxInputMemoryBuffer(int maxSizeInput);
private:
    asio::io_context& mIOContext;
    asio::ip::tcp::acceptor mAcceptor;
    std::optional<asio::ip::tcp::socket> mSocket;
};

}
