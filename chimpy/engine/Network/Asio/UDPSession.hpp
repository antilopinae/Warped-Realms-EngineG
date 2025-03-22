#pragma once

#include <asio.hpp>
#include <memory>

namespace EngineG::Network::UDP {

using messageHandler = std::function<void(std::string)>;
using errorHandler = std::function<void(asio::error_code error)>;

class Session: public std::enable_shared_from_this<Session> {
public:
    Session(asio::ip::tcp::socket&& socket);

    void Start(messageHandler&& onMessage, errorHandler&& onError);
    void Post(const std::string& message);
    void AsyncWrite();
    void AsyncRead();
private:
    void OnRead(asio::error_code error, std::size_t bytesTransferred);
    void OnWrite(asio::error_code error, std::size_t bytesTransferred);

    asio::ip::tcp::socket mSocket;
    asio::streambuf mStreamBuf;
    messageHandler mOnMessage;
    errorHandler mOnError;
    std::vector<std::string> mOutgoing;
};

}
