#pragma once

#include "WasmEngine.hpp"
#include "WasmHostShared.hpp"
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include <wasmtime.hh>

namespace EngineG::Wasm {

class WasmEngine;

class WasmComponent {
public:
    WasmComponent(const std::string& componentId,
                  const std::string& wasmPath,
                  const std::string& entityName,
                  const std::string& slotName,
                  WasmEngine* ownerEngine,
                  wasmtime::Engine& engine,
                  const std::vector<HostFunctionBinding>& specificHostFunctions);
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
    void WriteBytesToGuestMemory(uint32_t guestPtr, const std::vector<uint8_t>& bytes);
    std::vector<uint8_t> ReadBytesFromGuestMemory(uint32_t guestPtr, uint32_t len);
    void WriteStringToGuestMemory(uint32_t guestPtr, const std::string& str);
    std::string ReadStringFromGuestMemory(uint32_t guestPtr, uint32_t len);

    const std::string& GetId() const { return mId; }
    bool IsInitialized() const { return mIsInitialized; }
    const std::string& GetEntityName() const { return mEntityName; }
    const std::string& GetSlotName() const { return mSlotName; }

    std::vector<WasmMessagePayload> mInbox;

private:
    bool LoadAndInstantiateModule();
    bool ExtractExports();

    std::string mId;
    std::string mPath;
    std::string mEntityName;
    std::string mSlotName;
    WasmEngine* mOwnerEngine;
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

    const std::vector<HostFunctionBinding> mSpecificHostFunctions;
    bool mIsInitialized = false;
};

}  // namespace EngineG::Wasm