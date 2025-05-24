#pragma once

#include "WasmHostShared.hpp"
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <wasmtime.hh>  // Use Wasmtime C++ API

namespace EngineG::Wasm {

class WasmComponent;

struct WasmMessagePayload {
    std::string mFromId;
    std::string mToId;  // Target entity.slot or componentId
    std::string mContent;
};

struct WasmEntity {
    std::string mName;
    std::map<std::string, std::string> mComponentSlots;  // slotName -> componentId
};

struct WasmDeferredUpdate {
    std::string targetComponentId;
};

class WasmEngine {
public:
    WasmEngine();
    ~WasmEngine();

    WasmEngine(const WasmEngine&) = delete;
    WasmEngine& operator=(const WasmEngine&) = delete;
    WasmEngine(WasmEngine&&) = default;
    WasmEngine& operator=(WasmEngine&&) = default;

    bool AttachComponent(const std::string& entityName,
                         const std::string& slotName,
                         const std::string& wasmModulePath,
                         const std::vector<HostFunctionBinding>& specificHostFunctions);
    bool UnloadComponentFromSlot(const std::string& entityName, const std::string& slotName);

    void SendMessage(const std::string& fromComponentId,      // Can be "host" or a component ID
                     const std::string& toEntityAndSlotName,  // "EntityName.SlotName"
                     const std::string& messageContent);
    void Update();

    void LogWasmException(const std::string& operationName, const std::exception& e, const std::string& componentId);
    void LogWasmException(const std::string& operationName, const wasmtime::Error& e, const std::string& componentId);
    void LogWasmException(const std::string& operationName, const wasmtime::Trap& e, const std::string& componentId);
    void LogWasmException(const std::string& operationName, const std::string& e, const std::string& componentId);

    bool ReadFileToVector(const std::string& path, std::vector<uint8_t>& outBytes);

    wasmtime::Engine& GetWasmEngine() { return mEngine; }

private:
    std::map<std::string, WasmEntity> mEntities;
    std::map<std::string, std::unique_ptr<WasmComponent>> mLoadedComponents;
    std::vector<WasmMessagePayload> mMessageQueueInternal;
    std::vector<WasmDeferredUpdate> mDeferredWasmUpdates;

    wasmtime::Engine mEngine;

    std::string GenerateComponentId(const std::string& entityName, const std::string& slotName, const std::string& path);
    void UnloadComponentInternal(const std::string& componentId);

    void ProcessMessageQueue();
    void DeliverInboxMessages();
    void ProcessDeferredWasmUpdates();
    void ScheduleDeferredWasmUpdate(const std::string& componentId);
};

}  // namespace EngineG::Wasm