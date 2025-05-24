#include "WasmNetworkManager.hpp"

#include "../Actor.hpp"
#include "../Game.hpp"
#include "WasmHostApi.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>

namespace EngineG::Wasm {
namespace fs = std::filesystem;

WasmNetworkManager::WasmNetworkManager(EngineG::Game* gameContext) : mGameContext(gameContext) {
    if (!mGameContext) {
        throw std::runtime_error("WasmNetworkManager: EngineG::Game context cannot be null.");
    }
    LogMessage("WasmNetworkManager: Initialized.");
}

bool WasmNetworkManager::DownloadPayload(asio::ip::tcp::socket& socket,
                                         std::vector<char>& outBuffer,
                                         const std::string& fileDescription) {
    try {
        uint32_t data_size_net;
        asio::error_code error;
        size_t len_header = asio::read(socket, asio::buffer(&data_size_net, sizeof(data_size_net)), error);

        if (error == asio::error::eof) {
            LogMessage("NetworkMan ERROR: EOF while reading size for " + fileDescription);
            return false;
        } else if (error) {
            throw asio::system_error(error);
        }
        if (len_header != sizeof(data_size_net)) {
            LogMessage("NetworkMan ERROR: Incorrect size header received for " + fileDescription);
            return false;
        }

        uint32_t data_size_host = ntohl(data_size_net);
        LogMessage("NetworkMan: Expected " + fileDescription + " size: " + std::to_string(data_size_host) + " bytes.");

        if (data_size_host == 0 && fileDescription.find("error") == std::string::npos) {
            LogMessage("NetworkMan INFO: Received 0 bytes for " + fileDescription + ". Assuming empty payload.");
            outBuffer.clear();
            return true;
        }
        if (data_size_host > 1000 && data_size_host < (1024 * 1024 * 50) /*50MB max*/) {
        } else if (data_size_host <= 1000) {
            outBuffer.resize(data_size_host);
            size_t len_data = asio::read(socket, asio::buffer(outBuffer), error);
        } else {
            LogMessage("NetworkMan ERROR: Unreasonable payload size for " + fileDescription + ": " +
                       std::to_string(data_size_host));
            outBuffer.resize(std::min((uint32_t) 256, data_size_host));
            try {
                socket.read_some(asio::buffer(outBuffer), error);
            } catch (...) {
            }
            std::string potential_error(outBuffer.begin(), outBuffer.end());
            LogMessage("NetworkMan: Potential error string from server: " + potential_error);
            return false;
        }

        outBuffer.resize(data_size_host);
        if (data_size_host > 0) {
            size_t len_data = asio::read(socket, asio::buffer(outBuffer), asio::transfer_exactly(data_size_host), error);
            if (error == asio::error::eof && len_data < data_size_host) {
                LogMessage("NetworkMan ERROR: EOF before receiving all " + std::to_string(data_size_host) + " bytes for " +
                           fileDescription + ". Got " + std::to_string(len_data));
                return false;
            } else if (error) {
                throw asio::system_error(error);
            }
            if (len_data != data_size_host) {
                LogMessage("NetworkMan ERROR: Did not receive all bytes for " + fileDescription + ". Expected " +
                           std::to_string(data_size_host) + ", Got " + std::to_string(len_data));
                return false;
            }
        }
        LogMessage("NetworkMan: Successfully received " + std::to_string(outBuffer.size()) + " bytes for " + fileDescription +
                   ".");
        return true;
    } catch (const std::exception& e) {
        LogMessage("NetworkMan EXCEPTION during DownloadPayload for " + fileDescription + ": " + e.what());
        return false;
    }
}

bool WasmNetworkManager::SaveDownloadedFile(const std::string& localPath, const std::vector<char>& dataBuffer) {
    LogMessage("NetworkMan: Attempting to save " + std::to_string(dataBuffer.size()) + " bytes to " + localPath);
    try {
        fs::path path_obj(localPath);
        if (path_obj.has_parent_path()) {
            if (!fs::exists(path_obj.parent_path())) {
                if (!fs::create_directories(path_obj.parent_path())) {
                    LogMessage("NetworkMan ERROR: Could not create directory: " + path_obj.parent_path().string());
                    return false;
                }
            }
        }
        std::ofstream out_file(localPath, std::ios::binary | std::ios::trunc);
        if (!out_file) {
            LogMessage("NetworkMan ERROR: Failed to create/open local file: " + localPath);
            return false;
        }
        if (!dataBuffer.empty()) {
            out_file.write(dataBuffer.data(), dataBuffer.size());
        }
        out_file.close();
        if (!out_file) {
            LogMessage("NetworkMan ERROR: Failed to write/close local file: " + localPath);
            return false;
        }
        LogMessage("NetworkMan: File saved successfully to " + localPath);
        return true;
    } catch (const std::exception& e) {
        LogMessage("NetworkMan EXCEPTION during SaveDownloadedFile for " + localPath + ": " + e.what());
        return false;
    }
}

json WasmNetworkManager::ParseManifestData(const std::vector<char>& manifestData) {
    if (manifestData.empty()) {
        LogMessage("NetworkMan WARN: Manifest data is empty, cannot parse.");
        return json();
    }
    try {
        std::string potential_error_str(manifestData.begin(), manifestData.end());
        if (potential_error_str.rfind("ERROR:", 0) == 0 || potential_error_str.rfind("MANIFEST_NOT_FOUND", 0) == 0) {
            LogMessage("NetworkMan ERROR: Server sent error instead of manifest: " + potential_error_str);
            return json();
        }
        return json::parse(manifestData.begin(), manifestData.end());
    } catch (const json::parse_error& e) {
        LogMessage("NetworkMan ERROR: Failed to parse manifest JSON: " + std::string(e.what()) +
                   ". Data: " + std::string(manifestData.begin(), manifestData.end()).substr(0, 100));
        return json();
    }
}

bool WasmNetworkManager::ApplyDownloadedWasm(const json& manifest,
                                             const std::string& localWasmPath,
                                             const std::string& targetOwnerActorId) {
    if (!mGameContext) { /* ... */
        return false;
    }
    if (manifest.is_null() || manifest.empty() || !manifest.contains("target_script_slot_name")) {
        LogMessage("NetworkMan ERROR: Invalid or incomplete manifest for WASM: " + localWasmPath);
        return false;
    }

    std::string script_slot_name_on_target = manifest["target_script_slot_name"].get<std::string>();

    LogMessage("NetworkMan: Applying downloaded WASM - TargetActorID: " + targetOwnerActorId +
               ", Slot: " + script_slot_name_on_target + ", WasmPath: " + localWasmPath);

    EngineG::Actor* actor = mGameContext->FindActorById(targetOwnerActorId);
    if (!actor) { /* ... ошибка ... */
        return false;
    }

    LogMessage("NetworkMan: Requesting EngineG::Game to apply WASM to Actor: " + targetOwnerActorId +
               ", Slot: " + script_slot_name_on_target);

    bool success = mGameContext->ApplyWasmToActorComponent(targetOwnerActorId, script_slot_name_on_target, localWasmPath);

    if (success) {
        LogMessage("NetworkMan: Game successfully applied WASM.");
    } else {
        LogMessage("NetworkMan ERROR: Game failed to apply WASM.");
    }
    return success;
}

bool WasmNetworkManager::RequestAndApplyPackage(const std::string& server_host,
                                                const std::string& server_port,
                                                const std::string& packageName,
                                                const std::string& targetOwnerActorId) {
    LogMessage("NetworkMan: Requesting package '" + packageName + "' from " + server_host + ":" + server_port +
               " for OwnerID: " + targetOwnerActorId);

    try {
        asio::ip::tcp::resolver resolver(mIoContext);
        auto endpoints = resolver.resolve(server_host, server_port);
        asio::ip::tcp::socket socket(mIoContext);
        asio::connect(socket, endpoints);
        LogMessage("NetworkMan: Connected to server " + server_host + ":" + server_port);

        asio::write(socket, asio::buffer(packageName));
        LogMessage("NetworkMan: Sent package name request: " + packageName);

        std::vector<char> manifestData_buf;
        if (!DownloadPayload(socket, manifestData_buf, "manifest")) {
            LogMessage("NetworkMan ERROR: Failed to download manifest for package " + packageName);
            socket.close();
            return false;
        }

        json manifest_json = ParseManifestData(manifestData_buf);
        if (manifest_json.is_null() || manifest_json.empty()) {
            LogMessage("NetworkMan ERROR: Failed to parse manifest or manifest is empty for package " + packageName);
            socket.close();
            return false;
        }
        LogMessage("NetworkMan: Manifest parsed: " + manifest_json.dump(2).substr(0, 200) + "...");

        if (!manifest_json.contains("wasm_file_name") || !manifest_json["wasm_file_name"].is_string()) {
            LogMessage("NetworkMan ERROR: Manifest for " + packageName + " does not contain valid 'wasm_file_name'.");
            socket.close();
            return false;
        }
        std::string wasm_filename_on_server = manifest_json["wasm_file_name"].get<std::string>();
        if (wasm_filename_on_server.empty()) {
            LogMessage("NetworkMan ERROR: 'wasm_file_name' in manifest for " + packageName + " is empty.");
            socket.close();
            return false;
        }

        asio::write(socket, asio::buffer(wasm_filename_on_server));
        LogMessage("NetworkMan: Sent WASM file name request: " + wasm_filename_on_server);

        std::vector<char> wasm_data_buf;
        if (!DownloadPayload(socket, wasm_data_buf, "WASM file '" + wasm_filename_on_server + "'")) {
            LogMessage("NetworkMan ERROR: Failed to download WASM file " + wasm_filename_on_server);
            socket.close();
            return false;
        }
        if (wasm_data_buf.empty()) {
            LogMessage("NetworkMan ERROR: Downloaded WASM file " + wasm_filename_on_server + " is empty.");
            socket.close();
            return false;
        }

        socket.close();
        LogMessage("NetworkMan: Connection closed. Downloaded manifest and WASM file " + wasm_filename_on_server);

        std::string local_wasm_dir = ASSETS_DIR "/" + packageName + "/";
        fs::path local_wasm_fs_path = fs::path(local_wasm_dir) / wasm_filename_on_server;

        if (!SaveDownloadedFile(local_wasm_fs_path.string(), wasm_data_buf)) {
            LogMessage("NetworkMan ERROR: Failed to save downloaded WASM file to " + local_wasm_fs_path.string());
            return false;
        }
        LogMessage("NetworkMan: WASM file saved to " + local_wasm_fs_path.string());

        return ApplyDownloadedWasm(manifest_json, local_wasm_fs_path.string(), targetOwnerActorId);

    } catch (const std::exception& e) {
        LogMessage("NetworkMan EXCEPTION in RequestAndApplyPackage: " + std::string(e.what()));
        std::cerr << "NetworkMan EXCEPTION in RequestAndApplyPackage: " << e.what() << std::endl;
        return false;
    }
}

}  // namespace EngineG::Wasm