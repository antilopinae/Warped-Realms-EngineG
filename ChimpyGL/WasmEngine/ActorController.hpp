#pragma once

#include <string>

namespace WasmHost {

class GameEngine;

class ActorController {
 public:
  ActorController(GameEngine* gameEngine);

  void AttachComponent(const std::string& entityName, const std::string& slotName, const std::string& wasmModulePath);

  void UnloadComponentFromSlot(const std::string& entityName, const std::string& slotName);

  void SendMessage(const std::string& fromComponentFullId,  // e.g. "host_main" or "Player.scriptA[moduleA_c]"
                   const std::string& toEntityAndSlotName,  // e.g. "Player.scriptA"
                   const std::string& message);

 private:
  GameEngine* mGameEngine;
};

}  // namespace WasmHost
