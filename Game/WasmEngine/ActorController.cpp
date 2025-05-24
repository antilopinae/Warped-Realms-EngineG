#include "ActorController.hpp"
#include "GameEngine.hpp"
#include "HostApi.hpp" // Для LogMessage

namespace WasmHost {

ActorController::ActorController(GameEngine* gameEngine)
    : mGameEngine(gameEngine) {
  if (!mGameEngine) {
    throw std::runtime_error("ActorController initialized with null GameEngine pointer.");
  }
}

void ActorController::AttachComponent(const std::string& entityName,
                                      const std::string& slotName,
                                      const std::string& wasmModulePath) {
  if (!mGameEngine->AttachComponent(entityName, slotName, wasmModulePath)) {
    LogMessage("ActorController: AttachComponent failed for " + entityName + "." + slotName);
  } else {
    LogMessage("ActorController: AttachComponent successful for " + entityName + "." + slotName);
  }
}

void ActorController::UnloadComponentFromSlot(const std::string& entityName, const std::string& slotName) {
  if (!mGameEngine->UnloadComponentFromSlot(entityName, slotName)) {
    LogMessage("ActorController: UnloadComponentFromSlot failed for " + entityName + "." + slotName);
  }
}

void ActorController::SendMessage(const std::string& fromComponentFullId,
                                  const std::string& toEntityAndSlotName,
                                  const std::string& message) {
  mGameEngine->SendMessage(fromComponentFullId, toEntityAndSlotName, message);
}

} // namespace WasmHost