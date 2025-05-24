#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>
#include <wasmtime.hh>

#include "GameEngine.hpp"

namespace WasmHost {

class GameEngine;

class WasmComponent {
 public:
  WasmComponent(std::string componentId, std::string wasmPath, std::string entityName, std::string slotName,
                GameEngine* ownerEngine, wasmtime::Engine& engine);
  ~WasmComponent();

  WasmComponent(const WasmComponent&) = delete;
  WasmComponent& operator=(const WasmComponent&) = delete;
  WasmComponent(WasmComponent&&) = default;
  WasmComponent& operator=(WasmComponent&&) = default;

  bool Initialize();
  bool TriggerWasmUpdate();
  bool DeliverMessage(const std::string& fromId, const std::string& message);
  void Terminate();

  uint32_t AllocateInGuest(uint32_t size);
  void DeallocateInGuest(uint32_t ptr, uint32_t size);
  void WriteStringToGuestMemory(uint32_t guestPtr, const std::string& str);
  std::string ReadStringFromGuestMemory(uint32_t guestPtr, uint32_t len);

  const std::string& GetId() const { return mId; }
  bool IsInitialized() const { return mIsInitialized; }

  std::vector<MessagePayload> mInbox;

 private:
  bool LoadAndInstantiateModule();
  bool ExtractExports();

  std::string mId;
  std::string mPath;
  GameEngine* mOwnerEngine;
  wasmtime::Engine& mEngineRef;

  std::optional<wasmtime::Store> mStoreOpt;
  std::optional<wasmtime::Instance> mInstanceOpt;
  std::optional<wasmtime::Memory> mMemoryOpt;
  std::vector<uint8_t> mWasmBinaryData;

  std::optional<wasmtime::Func> mAllocateFuncOpt;
  std::optional<wasmtime::Func> mDeallocateFuncOpt;
  std::optional<wasmtime::Func> mInitializeFuncOpt;
  std::optional<wasmtime::Func> mUpdateFuncOpt;
  std::optional<wasmtime::Func> mOnMessageFuncOpt;
  std::optional<wasmtime::Func> mTerminateFuncOpt;

  bool mIsInitialized = false;
};

}  // namespace WasmHost