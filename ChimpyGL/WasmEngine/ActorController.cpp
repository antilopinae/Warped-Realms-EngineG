#include "ActorController.hpp"

#include <stdexcept>  // For std::runtime_error

#include "GameEngine.hpp"  // For GameEngine methods
#include "HostApi.hpp"     // For WasmHost::LogMessage (used by ActorController itself)

namespace WasmHost {

ActorController::ActorController(GameEngine* gameEngine) : mGameEngine(gameEngine) {
  if (!mGameEngine) {
    throw std::runtime_error("ActorController initialized with null GameEngine pointer.");
  }
}

void ActorController::AttachComponent(const std::string& entityName, const std::string& slotName,
                                      const std::string& wasmModulePath) {
  if (!mGameEngine->AttachComponent(entityName, slotName, wasmModulePath)) {
    LogMessage("ActorController: AttachComponent call to GameEngine failed for " + entityName + "." + slotName);
  }
}

void ActorController::UnloadComponentFromSlot(const std::string& entityName, const std::string& slotName) {
  LogMessage("ActorController: Requesting UnloadComponentFromSlot for Entity: '" + entityName + "', Slot: '" + slotName + "'.");
  if (!mGameEngine->UnloadComponentFromSlot(entityName, slotName)) {
    LogMessage("ActorController: UnloadComponentFromSlot call to GameEngine failed for " + entityName + "." + slotName);
  }
}

void ActorController::SendMessage(const std::string& fromComponentFullId, const std::string& toEntityAndSlotName,
                                  const std::string& message) {
  mGameEngine->SendMessage(fromComponentFullId, toEntityAndSlotName, message);
}

}  // namespace WasmHost