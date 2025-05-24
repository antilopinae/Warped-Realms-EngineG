#pragma once

#include <string>
#include <vector>
#include <wasm.h>
#include <wasmtime.h>

namespace WasmHost {

class GameEngine;

struct MessagePayload {
    std::string mFromId;
    std::string mToEntitySlot;
    std::string mContent;
};

class WasmComponent {
public:
    WasmComponent(std::string componentId, std::string wasmPath, std::string entityName, std::string slotName,
                  GameEngine* ownerEngine, wasm_engine_t* engine);
    ~WasmComponent();

    WasmComponent(const WasmComponent&) = delete;
    WasmComponent& operator=(const WasmComponent&) = delete;
    WasmComponent(WasmComponent&&) = default;
    WasmComponent& operator=(WasmComponent&&) = default;

    bool Initialize();
    bool Update();
    bool DeliverMessage(const std::string& fromId, const std::string& message);
    bool Terminate();

    uint32_t AllocateInGuest(uint32_t size);
    void DeallocateInGuest(uint32_t ptr, uint32_t size);

    void WriteStringToGuestMemory(uint32_t guestPtr, const std::string& str);
    std::string ReadStringFromGuestMemory(uint32_t guestPtr, uint32_t len);

    byte_t* GetGuestMemoryDataPtr();
    size_t GetGuestMemoryDataSize();

    const std::string& GetId() const { return mId; }
    const std::string& GetPath() const { return mPath; }
    GameEngine* GetOwnerEngine() const { return mOwnerEngine; }
    bool IsInitialized() const { return mIsInitialized; }

    std::vector<MessagePayload> mInbox;

private:
    bool LoadModuleAndInstantiate(wasm_engine_t* engine);
    bool LinkImports();

    bool CallVoidExport(wasm_func_t* func, const char* funcNameForLog);

    std::string mId;
    std::string mPath;
    std::string mEntityName;
    std::string mSlotName;
    GameEngine* mOwnerEngine;

    wasm_store_t* mStore = nullptr;
    wasm_module_t* mModule = nullptr;
    wasm_instance_t* mInstance = nullptr;
    wasm_memory_t* mMemory = nullptr;
    std::vector<byte_t> mWasmBinaryData;

    // Экспортируемые функции из WASM
    wasm_func_t* mAllocateFunc = nullptr;
    wasm_func_t* mDeallocateFunc = nullptr;
    wasm_func_t* mInitializeFuncFromWasm = nullptr;
    wasm_func_t* mUpdateFuncFromWasm = nullptr;
    wasm_func_t* mOnMessageFuncFromWasm = nullptr;
    wasm_func_t* mTerminateFuncFromWasm = nullptr;

    bool mIsInitialized = false;
};

} // namespace WasmHost