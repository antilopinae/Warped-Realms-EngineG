#include "ActorController.hpp"
#include "GameEngine.hpp" // For GameEngine methods
#include "HostApi.hpp"    // For WasmHost::LogMessage (used by ActorController itself)

#include <stdexcept> // For std::runtime_error

namespace WasmHost {

ActorController::ActorController(GameEngine* gameEngine)
    : mGameEngine(gameEngine) {
  if (!mGameEngine) {
    // This is a programming error, throw immediately.
    throw std::runtime_error("ActorController initialized with null GameEngine pointer.");
  }
  LogMessage("ActorController: Initialized.");
}

void ActorController::AttachComponent(const std::string& entityName,
                                      const std::string& slotName,
                                      const std::string& wasmModulePath) {
  LogMessage("ActorController: Requesting AttachComponent for Entity: '" + entityName +
             "', Slot: '" + slotName + "', Path: '" + wasmModulePath + "'.");
  if (!mGameEngine->AttachComponent(entityName, slotName, wasmModulePath)) {
    // GameEngine's AttachComponent should do its own detailed logging.
    // ActorController can log a summary.
    LogMessage("ActorController: AttachComponent call to GameEngine failed for " +
               entityName + "." + slotName);
  } else {
    LogMessage("ActorController: AttachComponent call to GameEngine reported success for " +
               entityName + "." + slotName);
  }
}

void ActorController::UnloadComponentFromSlot(const std::string& entityName, const std::string& slotName) {
  LogMessage("ActorController: Requesting UnloadComponentFromSlot for Entity: '" + entityName +
             "', Slot: '" + slotName + "'.");
  if (!mGameEngine->UnloadComponentFromSlot(entityName, slotName)) {
    LogMessage("ActorController: UnloadComponentFromSlot call to GameEngine failed for " +
               entityName + "." + slotName);
    // Depending on semantics, this might not be an "error" if the slot was already empty.
    // GameEngine should clarify this with its logs.
  } else {
     LogMessage("ActorController: UnloadComponentFromSlot call to GameEngine reported success for " +
               entityName + "." + slotName);
  }
}

void ActorController::SendMessage(const std::string& fromComponentFullId,
                                  const std::string& toEntityAndSlotName,
                                  const std::string& message) {
  LogMessage("ActorController: Sending message from '" + fromComponentFullId +
             "' to '" + toEntityAndSlotName + "'.");
  mGameEngine->SendMessage(fromComponentFullId, toEntityAndSlotName, message);
}

} // namespace WasmHost