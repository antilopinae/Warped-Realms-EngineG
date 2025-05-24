#pragma once
#include <string>
#include <vector>

namespace WasmHost {

class GameEngine;

class ActorController {
public:
  ActorController(GameEngine* gameEngine);

  void AttachComponent(const std::string& entityName,
                       const std::string& slotName,
                       const std::string& wasmModulePath);

  void UnloadComponentFromSlot(const std::string& entityName, const std::string& slotName);

  void SendMessage(const std::string& fromComponentFullId,
                   const std::string& toEntityAndSlotName,
                   const std::string& message);
private:
  GameEngine* mGameEngine;
};

} // namespace WasmHost