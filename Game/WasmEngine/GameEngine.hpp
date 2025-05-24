#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>
#include <wasmtime.hh>  // Use Wasmtime C++ API

namespace WasmHost {

class WasmComponent;

struct MessagePayload {
  std::string mFromId;
  std::string mToId;  // Target entity.slot or componentId
  std::string mContent;
};

struct Entity {
  std::string mName;
  std::map<std::string, std::string> mComponentSlots;  // slotName -> componentId
};

struct DeferredWasmUpdate {
  std::string targetComponentId;
};

class GameEngine {
 public:
  GameEngine();
  ~GameEngine();

  GameEngine(const GameEngine&) = delete;
  GameEngine& operator=(const GameEngine&) = delete;
  GameEngine(GameEngine&&) = default;
  GameEngine& operator=(GameEngine&&) = default;

  bool AttachComponent(const std::string& entityName, const std::string& slotName, const std::string& wasmModulePath);
  bool UnloadComponentFromSlot(const std::string& entityName, const std::string& slotName);

  void SendMessage(const std::string& fromComponentId,      // Can be "host" or a component ID
                   const std::string& toEntityAndSlotName,  // "EntityName.SlotName"
                   const std::string& messageContent);
  void Update();

  void LogWasmException(const std::string& operationName, const std::exception& e, const std::string& componentId);
  void LogWasmException(const std::string& operationName, const wasmtime::Error& e, const std::string& componentId);
  void LogWasmException(const std::string& operationName, const wasmtime::Trap& e, const std::string& componentId);

  bool ReadFileToVector(const std::string& path, std::vector<uint8_t>& outBytes);

  wasmtime::Engine& GetWasmEngine() { return mEngine; }
  wasmtime::Linker& GetLinker() { return mLinker; }

 private:
  std::map<std::string, Entity> mEntities;
  std::map<std::string, std::unique_ptr<WasmComponent>> mLoadedComponents;
  std::vector<MessagePayload> mMessageQueueInternal;
  std::vector<DeferredWasmUpdate> m_deferredWasmUpdates;

  wasmtime::Engine mEngine;
  wasmtime::Linker mLinker;

  std::string GenerateComponentId(const std::string& entityName, const std::string& slotName, const std::string& path);
  void UnloadComponentInternal(const std::string& componentId);

  void ProcessMessageQueue();
  void DeliverInboxMessages();
  void ProcessDeferredWasmUpdates();
  void ScheduleDeferredWasmUpdate(const std::string& componentId);
};

}  // namespace WasmHost