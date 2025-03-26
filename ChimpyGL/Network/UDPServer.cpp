#include "UDPServer.hpp"

#include "Message.hpp"
#include "UDPSession.hpp"
#include <memory>
#include <iostream>

namespace EngineG::Network::UDP {
#if 0
Server::Server(asio::io_context& ioContext, std::uint16_t port)
:mIOContext(ioContext),
mAcceptor(
    ioContext,
    asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)
    )
{
}

void Server::AsyncAccept() {
    mSocket.emplace(mIOContext);

    mAcceptor.async_accept(*mSocket, [&](asio::error_code error) {
        if (!error){
            std::make_shared<Session>(std::move(*mSocket))->Start();
            AsyncAccept();
        }
        else
        {
            std::cerr << error.message() << "\n";
        }
    });
}

void Server::Terminate() {
    asio::error_code error;
    mSocket->close(error);
    if(error){
        std::cerr << error.message() << "\n";
    }
}
#endif
}
