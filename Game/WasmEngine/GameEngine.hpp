#pragma once

#include "WasmComponent.hpp"
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <wasm.h>
#include <wasmtime.h>

namespace WasmHost {

struct Entity {
    std::string mName;
    std::map<std::string, std::string> mComponentSlots; // slotName -> componentId
};

class GameEngine {
public:
    GameEngine();
    ~GameEngine();

    GameEngine(const GameEngine&) = delete;
    GameEngine& operator=(const GameEngine&) = delete;
    GameEngine(GameEngine&&) = default;
    GameEngine& operator=(GameEngine&&) = default;

    bool AttachComponent(const std::string& entityName,
                         const std::string& slotName,
                         const std::string& wasmModulePath);
    bool UnloadComponentFromSlot(const std::string& entityName, const std::string& slotName);

    void SendMessage(const std::string& fromComponentId,
                     const std::string& toEntityAndSlotName,
                     const std::string& messageContent);
    void Update();

    void HandleWasmTrap(const std::string& funcName, wasm_trap_t* trap, const std::string& componentId);

    bool ReadFileToVector(const std::string& path, std::vector<byte_t>& outBytes);
    wasm_engine_t* GetWasmEngine() { return mWasmEngine; }

private:
    std::map<std::string, Entity> mEntities;
    std::map<std::string, std::unique_ptr<WasmComponent>> mLoadedComponents;
    std::vector<MessagePayload> mMessageQueue;

    wasm_engine_t* mWasmEngine = nullptr;

    std::string GenerateComponentId(const std::string& entityName, const std::string& slotName, const std::string& path);
    void UnloadComponentInternal(const std::string& componentId);

    void ProcessMessageQueue();
    void DeliverInboxMessages();
};

} // namespace WasmHost