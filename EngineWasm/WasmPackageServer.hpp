#pragma once

#include <asio.hpp>
#include <atomic>
#include <string>
#include <thread>

namespace EngineG::Wasm {

class WasmPackageServer {
public:
    WasmPackageServer(const std::string& package_root_dir, uint16_t port);
    ~WasmPackageServer();

    void Start();
    void Stop();

private:
    void Run();
    void HandleClient(asio::ip::tcp::socket socket);

    std::string m_package_root_dir;
    uint16_t m_port;
    asio::io_context m_io_context;
    std::unique_ptr<asio::ip::tcp::acceptor> m_acceptor;
    std::thread m_server_thread;
    std::atomic<bool> m_running;
};

}  // namespace EngineG::Wasm