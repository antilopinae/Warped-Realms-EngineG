#include "UDPSession.hpp"

#include <iostream>
#include <functional>
#include "asio/error.hpp"

namespace EngineG::Network::UDP {

using MessageHandler = std::function<void(std::string)>;

Session::Session(asio::ip::tcp::socket&& socket)
:mSocket(std::move(socket))
,mStreamBuf(65536)
{
}

void Session::Start(messageHandler&& onMessage, errorHandler&& onError) {
  mOnMessage = std::move(onMessage);
  mOnError = std::move(onError);

  asio::async_read_until(
      mSocket,
      mStreamBuf,
      '.',
      [self = shared_from_this()](
          asio::error_code error,
          std::size_t bytes_transferred)
      {
          if (!error)
          {
              std::cout << std::istream(&self->mStreamBuf).rdbuf();

              asio::ip::tcp::endpoint endpoint;
              asio::error_code errorRemoteSocket;
              asio::error_code errorLocalSocket;
              // IP:the port on the remote side of the connection
              auto remoteEndPoint = (&self->mSocket)->local_endpoint(errorRemoteSocket);
              auto localEndPoint = (&self->mSocket)->remote_endpoint(errorLocalSocket);

              if (!errorRemoteSocket && !errorLocalSocket) {
                  std::cout << "Remote endpoint: " << remoteEndPoint << "\n";
                  std::cout << "Local endpoint: " << localEndPoint << "\n";
              }
              else
              {
                  std::cerr << errorLocalSocket.message() << "\n";
                  std::cerr << errorRemoteSocket.message() << "\n";
              }
          }
          else if (error == asio::error::eof)
          {
              // The connection was terminated.
              // The received data is still stored in the buffer,
              // numerically equal to `bytes_transferred' (in bytes)
          }
          else
          {
              std::cerr << error.message() << "\n";
          }
      });
}

void Session::AsyncRead() {
  using namespace std::placeholders;
  asio::async_read_until(
      mSocket,
      mStreamBuf,
      "\n",
      std::bind(&Session::OnRead, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
}

void Session::AsyncWrite() {
  using namespace std::placeholders;
  asio::async_write(
      mSocket,
      asio::buffer(mOutgoing.front()),
      std::bind(&Session::OnWrite, shared_from_this(), _1, _2));
}

void Session::Post(const std::string& message)
{
  bool idle = mOutgoing.empty();
  mOutgoing.push_back(message);

  if (idle) {
    AsyncWrite();
  }
}

void Session::OnRead(asio::error_code error, std::size_t bytesTransferred)
{
    if (!error) {
      std::stringstream message;
      message << mSocket.remote_endpoint(error) << ": "
              << std::istream(&mStreamBuf).rdbuf();
      mStreamBuf.consume(bytesTransferred);
      mOnMessage(message.str());
      AsyncRead();
    } else {
      mSocket.close(error);
      mOnError(error);
    }
}

void Session::OnWrite(asio::error_code error, std::size_t bytesTransferred) {
  if (!error) {
    mOutgoing.pop_back();

    if (!mOutgoing.empty()) {
      AsyncWrite();
    }
  } else {
    mSocket.close(error);
    mOnError(error);
  }
}

}
