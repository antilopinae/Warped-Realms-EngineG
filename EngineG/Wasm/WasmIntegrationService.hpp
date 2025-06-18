#pragma once

#include "WasmHostShared.hpp"

#include <memory>
#include <string>
#include <vector>

namespace EngineG {
namespace Wasm {
class WasmEngine;
}

class WasmIntegrationService {
public:
    WasmIntegrationService();
    ~WasmIntegrationService();

    void Update(float deltaTime);

    bool AttachWasmComponent(const std::string& ownerId,
                             const std::string& scriptSlotName,
                             const std::string& wasmModulePath,
                             const std::vector<Wasm::HostFunctionBinding>& specificHostFunctions);

    bool DetachWasmComponent(const std::string& ownerId, const std::string& scriptSlotName);

    bool SendMessageToWasm(const std::string& hostSenderId,
                           const std::string& targetOwnerId,
                           const std::string& targetScriptSlotName,
                           const std::string& messageContent);

private:
    std::unique_ptr<Wasm::WasmEngine> mWasmEngine;
};

}  // namespace EngineG
