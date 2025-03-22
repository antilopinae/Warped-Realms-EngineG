#include "SimpleUDPServer.hpp"

#include "UDPServer.hpp"
#include "tutorial//SimpleClient.hpp"
#include "tutorial//SimpleServer.hpp"
#include "UDPSession.hpp"

#include <asio.hpp>
#include <asio/basic_datagram_socket.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

#include <iostream>
#include <string>
#include <chrono>

namespace EngineG::Network {

void StartSimpleUDP() {
    std::uint16_t port = 15001;
    asio::io_context io_context;

    // Ip address and port
    // asio::ip::udp::v4() - Network Interface
    asio::ip::udp::endpoint receiver(asio::ip::udp::v4(), port);

    // Sockets descriptor, receiver - endpoint
    asio::ip::udp::socket socket(io_context, receiver);

    char buffer[65536];
    asio::ip::udp::endpoint sender;

#if 0
    while (true) {
        // Asio::buffer = Representation memory
        std::size_t bytes_transferred = socket.receive_from(asio::buffer(buffer), sender);

        socket.send_to(asio::buffer(buffer, bytes_transferred), sender);

        std::cout << std::string(buffer, bytes_transferred) << "\nReceived from: " << sender.address().to_string() << std::endl;
    }
#endif
    // This operation is non-blocking
    // Code execution will continue immediately after calling the function.
    socket.async_receive_from(
        asio::buffer(buffer),
        sender,
        [&](asio::error_code error, std::size_t bytes_transferred) {
            // This lambda function will be called after receiving the message.
            if (!error) {
                std::cout
                    << "Message is received with size: "
                    << bytes_transferred
                    << ": "
                    << std::string(buffer, bytes_transferred)
                    ;
            }
            else
            {
                std::cerr << error.message() << "\n";
            }
        });

    io_context.run();
}

void StartUDP() {
  std::uint16_t port = 15001;
  asio::io_context ioContext;

  using namespace UDP;
  auto server = std::thread([&]() {  startCustomServer(); });
  auto client = std::thread([&]() {  startCustomClient(); });
  client.join();
  server.join();
  // Server server{ioContext, port};
  // server.AsyncAccept();
  // ioContext.run();
  // server.Terminate();
}

}
