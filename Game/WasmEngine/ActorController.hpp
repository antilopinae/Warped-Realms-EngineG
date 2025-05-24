#pragma once
#include <string>
// No need for <vector> here

namespace WasmHost {

class GameEngine; // Forward declaration

class ActorController {
public:
    ActorController(GameEngine* gameEngine); // GameEngine pointer is fine

    void AttachComponent(const std::string& entityName,
                         const std::string& slotName,
                         const std::string& wasmModulePath);

    void UnloadComponentFromSlot(const std::string& entityName, const std::string& slotName);

    void SendMessage(const std::string& fromComponentFullId, // e.g. "host_main" or "Player.scriptA[moduleA_c]"
                     const std::string& toEntityAndSlotName, // e.g. "Player.scriptA"
                     const std::string& message);
private:
    GameEngine* mGameEngine; // Non-owning pointer to the GameEngine
};

} // namespace WasmHost