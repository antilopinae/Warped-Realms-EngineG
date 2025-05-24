#include "WasmController.hpp"

#include "WasmEngine.hpp"
#include "WasmHostApi.hpp"
#include <stdexcept>

namespace EngineG::Wasm {

WasmController::WasmController(WasmEngine* gameEngine) : mWasmEngine(gameEngine) {
    if (!mWasmEngine) {
        throw std::runtime_error("WasmController initialized with null WasmEngine pointer.");
    }
}

void WasmController::AttachComponent(const std::string& entityName,
                                     const std::string& slotName,
                                     const std::string& wasmModulePath) {
    if (!mWasmEngine->AttachComponent(entityName, slotName, wasmModulePath, {})) {
        LogMessage("WasmController: AttachComponent call to WasmEngine failed for " + entityName + "." + slotName);
    }
}

void WasmController::UnloadComponentFromSlot(const std::string& entityName, const std::string& slotName) {
    if (!mWasmEngine->UnloadComponentFromSlot(entityName, slotName)) {
        LogMessage("WasmController: UnloadComponentFromSlot call to WasmEngine failed for " + entityName + "." + slotName);
    }
}

void WasmController::SendMessage(const std::string& fromComponentFullId,
                                 const std::string& toEntityAndSlotName,
                                 const std::string& message) {
    mWasmEngine->SendMessage(fromComponentFullId, toEntityAndSlotName, message);
}

}  // namespace EngineG::Wasm