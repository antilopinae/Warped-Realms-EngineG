#pragma once

#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <wasmtime.hh> // Use Wasmtime C++ API
#include "GameEngine.hpp" // For MessagePayload (could be moved to a common types header)

namespace WasmHost {

class GameEngine; // Forward declaration

class WasmComponent {
public:
    WasmComponent(std::string componentId, std::string wasmPath, std::string entityName, std::string slotName,
                  GameEngine* ownerEngine, wasmtime::Engine& engine); // Pass engine by reference
    ~WasmComponent();

    WasmComponent(const WasmComponent&) = delete;
    WasmComponent& operator=(const WasmComponent&) = delete;
    WasmComponent(WasmComponent&&) = default;
    WasmComponent& operator=(WasmComponent&&) = default;

    bool Initialize(); // Can throw wasmtime::Error
    bool TriggerWasmUpdate(); // Can throw wasmtime::Error or wasmtime::Trap
    bool DeliverMessage(const std::string& fromId, const std::string& message); // Can throw
    void Terminate(); // Can throw

    uint32_t AllocateInGuest(uint32_t size); // Can throw
    void DeallocateInGuest(uint32_t ptr, uint32_t size); // Can throw
    void WriteStringToGuestMemory(uint32_t guestPtr, const std::string& str); // Can throw
    std::string ReadStringFromGuestMemory(uint32_t guestPtr, uint32_t len); // Can throw

    const std::string& GetId() const { return mId; }
    bool IsInitialized() const { return mIsInitialized; }

    // Expose StoreContextMut for advanced scenarios if needed, but prefer specific methods.
    // wasmtime::StoreContextMut GetStoreContextMut() { return mStore.context(); }
    // Expose Memory for direct access by GameEngine or trusted systems if needed.
    // std::optional<wasmtime::Memory>& GetMemory() { return mMemoryOpt; }

    std::vector<MessagePayload> mInbox; // Public for GameEngine to push to, component processes internally

private:
    bool LoadAndInstantiateModule(); // Can throw
    bool ExtractExports(); // Can throw

    std::string mId;
    std::string mPath;
    GameEngine* mOwnerEngine; // Still needed for logging, file reading etc.
    wasmtime::Engine& mEngineRef; // Reference to the GameEngine's Wasmtime engine

    std::optional<wasmtime::Store> mStoreOpt;
    std::optional<wasmtime::Instance> mInstanceOpt;
    std::optional<wasmtime::Memory> mMemoryOpt;
    std::vector<uint8_t> mWasmBinaryData;

    // Exported functions
    std::optional<wasmtime::Func> mAllocateFuncOpt;
    std::optional<wasmtime::Func> mDeallocateFuncOpt;
    std::optional<wasmtime::Func> mInitializeFuncOpt;
    std::optional<wasmtime::Func> mUpdateFuncOpt;
    std::optional<wasmtime::Func> mOnMessageFuncOpt;
    std::optional<wasmtime::Func> mTerminateFuncOpt;

    bool mIsInitialized = false;
};

} // namespace WasmHost