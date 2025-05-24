#pragma once

#include "WasmHostShared.hpp"
#include <asio.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace EngineG {
class Game;
namespace Wasm {

using json = nlohmann::json;

class WasmNetworkManager {
public:
    WasmNetworkManager(EngineG::Game* game_context);

    bool RequestAndApplyPackage(const std::string& serverHost,
                                const std::string& serverPort,
                                const std::string& packageName,
                                const std::string& targetOwnerActorId);

private:
    asio::io_context mIoContext;
    EngineG::Game* mGameContext;

    bool DownloadPayload(asio::ip::tcp::socket& socket, std::vector<char>& out_buffer, const std::string& fileDescription);
    bool SaveDownloadedFile(const std::string& localPath, const std::vector<char>& dataBuffer);
    json ParseManifestData(const std::vector<char>& manifestData);

    bool ApplyDownloadedWasm(const json& manifest, const std::string& localWasmPath, const std::string& targetOwnerActorId);
};

}  // namespace Wasm
}  // namespace EngineG