#include "WasmIntegrationService.hpp"

#include "WasmEngine/WasmEngine.hpp"
#include "WasmEngine/WasmHostApi.hpp"

namespace EngineG {

WasmIntegrationService::WasmIntegrationService() {
    try {
        mWasmEngine = std::make_unique<Wasm::WasmEngine>();
    } catch (const std::exception& e) {
        Wasm::LogMessage("WasmIntegrationService FATAL: Failed to create Wasm::WasmEngine: " + std::string(e.what()));
    }
}

WasmIntegrationService::~WasmIntegrationService() {
}

void WasmIntegrationService::Update() {
    if (!mWasmEngine) return;
    mWasmEngine->Update();
}

bool WasmIntegrationService::AttachWasmComponent(const std::string& ownerId,
                                                 const std::string& scriptSlotName,
                                                 const std::string& wasmModulePath,
                                                 const std::vector<Wasm::HostFunctionBinding>& specificHostFunctions) {
    if (!mWasmEngine) {
        Wasm::LogMessage("WasmIntegrationService ERROR: AttachWasmComponent called while not initialized.");
        return false;
    }
    Wasm::LogMessage("WasmIntegrationService: Forwarding AttachWasmComponent for Owner: " + ownerId +
                     ", Slot: " + scriptSlotName + ", Path: " + wasmModulePath + " with " +
                     std::to_string(specificHostFunctions.size()) + " specific bindings.");
    return mWasmEngine->AttachComponent(ownerId, scriptSlotName, wasmModulePath, specificHostFunctions);
}

bool WasmIntegrationService::DetachWasmComponent(const std::string& ownerId, const std::string& scriptSlotName) {
    if (!mWasmEngine) {
        return false;
    }
    Wasm::LogMessage("WasmIntegrationService: Forwarding DetachWasmComponent for Owner: " + ownerId +
                     ", Slot: " + scriptSlotName);
    return mWasmEngine->UnloadComponentFromSlot(ownerId, scriptSlotName);
}

bool WasmIntegrationService::SendMessageToWasm(const std::string& hostSenderId,
                                               const std::string& targetOwnerId,
                                               const std::string& targetScriptSlotName,
                                               const std::string& messageContent) {
    if (!mWasmEngine) {
        return false;
    }
    std::string targetWasmEntityAndSlot = targetOwnerId + "." + targetScriptSlotName;
    mWasmEngine->SendMessage(hostSenderId, targetWasmEntityAndSlot, messageContent);
    return true;
}

}  // namespace EngineG