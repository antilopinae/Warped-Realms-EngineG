#include "WasmPackageServer.hpp"

#include "WasmHostApi.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

namespace fs = std::filesystem;

namespace EngineG::Wasm {

static std::vector<char> ReadBinaryFile(const std::string& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) {
        LogMessage("Server ERROR: Failed to open file: " + path);
        return {};
    }
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<char> buffer(static_cast<size_t>(size));
    if (file.read(buffer.data(), size)) {
        return buffer;
    }
    LogMessage("Server ERROR: Failed to read file: " + path);
    return {};
}

WasmPackageServer::WasmPackageServer(const std::string& package_root_dir, uint16_t port)
    : m_package_root_dir(package_root_dir), m_port(port), m_running(false) {
    LogMessage("WasmPackageServer: Initializing for root '" + m_package_root_dir + "' on port " + std::to_string(m_port));
}

WasmPackageServer::~WasmPackageServer() {
    Stop();
}

void WasmPackageServer::Start() {
    if (m_running) {
        LogMessage("WasmPackageServer: Already running.");
        return;
    }
    LogMessage("WasmPackageServer: Starting...");
    m_running = true;
    try {
        m_acceptor =
            std::make_unique<asio::ip::tcp::acceptor>(m_io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), m_port));
    } catch (const std::exception& e) {
        LogMessage("WasmPackageServer ERROR: Failed to create acceptor: " + std::string(e.what()));
        m_running = false;
        return;
    }
    m_server_thread = std::thread(&WasmPackageServer::Run, this);
    LogMessage("WasmPackageServer: Started in a new thread.");
}

void WasmPackageServer::Stop() {
    if (!m_running) return;
    LogMessage("WasmPackageServer: Stopping...");
    m_running = false;
    if (m_acceptor && m_acceptor->is_open()) {
        m_acceptor->close();
    }
    m_io_context.stop();
    if (m_server_thread.joinable()) {
        m_server_thread.join();
    }
    LogMessage("WasmPackageServer: Stopped.");
}

void WasmPackageServer::Run() {
    LogMessage("WasmPackageServer: Run loop started on port " + std::to_string(m_port));
    try {
        while (m_running) {
            asio::ip::tcp::socket socket(m_io_context);
            m_acceptor->accept(socket);
            if (!m_running) break;

            LogMessage("WasmPackageServer: Client connected from " + socket.remote_endpoint().address().to_string());
            HandleClient(std::move(socket));
        }
    } catch (const std::exception& e) {
        if (m_running) {
            LogMessage("WasmPackageServer ERROR in Run loop: " + std::string(e.what()));
        }
    }
    LogMessage("WasmPackageServer: Run loop finished.");
    if (!m_running) {
        m_io_context.stop();
    }
}

void WasmPackageServer::HandleClient(asio::ip::tcp::socket socket) {
    try {
        char request_buf[256];
        asio::error_code error;

        // 1. Читаем имя запрашиваемого пакета
        size_t len = socket.read_some(asio::buffer(request_buf), error);
        if (error == asio::error::eof) {
            LogMessage("Server: Client disconnected (EOF) at package name request.");
            return;
        }
        if (error) throw asio::system_error(error);
        std::string requested_package_name(request_buf, len);
        LogMessage("Server: Client requested package: " + requested_package_name);

        fs::path package_dir = fs::path(m_package_root_dir) / requested_package_name;
        fs::path manifest_path = package_dir / "manifest.json";

        // 2. Читаем и отправляем манифест
        std::vector<char> manifest_data = ReadBinaryFile(manifest_path.string());
        if (manifest_data.empty()) {
            LogMessage("Server ERROR: Manifest file not found or empty: " + manifest_path.string());
            std::string err_msg = "MANIFEST_NOT_FOUND";
            uint32_t err_size = htonl(static_cast<uint32_t>(err_msg.length()));
            asio::write(socket, asio::buffer(&err_size, sizeof(err_size)));
            asio::write(socket, asio::buffer(err_msg));
            return;
        }

        uint32_t manifest_size_net = htonl(static_cast<uint32_t>(manifest_data.size()));
        asio::write(socket, asio::buffer(&manifest_size_net, sizeof(manifest_size_net)));
        asio::write(socket, asio::buffer(manifest_data));
        LogMessage("Server: Sent manifest for " + requested_package_name + " (" + std::to_string(manifest_data.size()) +
                   " bytes).");

        // 3. Читаем имя WASM файла, которое клиент извлек из манифеста
        len = socket.read_some(asio::buffer(request_buf), error);
        if (error == asio::error::eof) {
            LogMessage("Server: Client disconnected (EOF) at WASM file name request.");
            return;
        }
        if (error) throw asio::system_error(error);
        std::string requested_wasm_filename(request_buf, len);
        LogMessage("Server: Client requested WASM file from manifest: " + requested_wasm_filename);

        fs::path wasm_file_path_actual = package_dir / requested_wasm_filename;

        // 4. Читаем и отправляем WASM файл
        std::vector<char> wasm_data = ReadBinaryFile(wasm_file_path_actual.string());
        if (wasm_data.empty()) {
            LogMessage("Server ERROR: WASM file not found or empty: " + wasm_file_path_actual.string());
            std::string err_msg = "WASM_FILE_NOT_FOUND";
            uint32_t err_size = htonl(static_cast<uint32_t>(err_msg.length()));
            asio::write(socket, asio::buffer(&err_size, sizeof(err_size)));
            asio::write(socket, asio::buffer(err_msg));
            return;
        }

        uint32_t wasm_size_net = htonl(static_cast<uint32_t>(wasm_data.size()));
        asio::write(socket, asio::buffer(&wasm_size_net, sizeof(wasm_size_net)));
        asio::write(socket, asio::buffer(wasm_data));
        LogMessage("Server: Sent WASM file " + requested_wasm_filename + " (" + std::to_string(wasm_data.size()) + " bytes).");

    } catch (const std::exception& e) {
        LogMessage("Server ERROR in HandleClient: " + std::string(e.what()));
    }
    LogMessage("Server: Client handling finished for " + socket.remote_endpoint().address().to_string());
}

}  // namespace EngineG::Wasm