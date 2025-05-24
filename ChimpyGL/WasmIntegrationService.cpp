#include "WasmIntegrationService.hpp"
#include "WasmEngine/GameEngine.hpp" // Полное определение
#include "WasmEngine/HostApi.hpp"   // Для WasmHost::LogMessage

// GetInstance, конструктор, деструктор, Initialize, Shutdown, Update - остаются как в вашем последнем варианте.
WasmIntegrationService& WasmIntegrationService::GetInstance() {
    static WasmIntegrationService instance;
    return instance;
}
WasmIntegrationService::WasmIntegrationService() { /* ... Log ... */ }
WasmIntegrationService::~WasmIntegrationService() { /* ... Log ... */ }
bool WasmIntegrationService::Initialize() { /* ... как раньше, создает m_wasmGameEngine ... */
    if (m_isInitialized) return true;
    WasmHost::LogMessage("WasmIntegrationService: Initializing...");
    try {
        m_wasmGameEngine = std::make_unique<WasmHost::GameEngine>();
    } catch (const std::exception& e) {
        WasmHost::LogMessage("WasmIntegrationService FATAL: Failed to create WasmHost::GameEngine: " + std::string(e.what()));
        return false;
    }
    m_isInitialized = true;
    WasmHost::LogMessage("WasmIntegrationService: Initialized successfully.");
    return true;
}
void WasmIntegrationService::Shutdown() { /* ... как раньше, m_wasmGameEngine.reset() ... */
    if (!m_isInitialized) return;
    WasmHost::LogMessage("WasmIntegrationService: Shutting down...");
    m_wasmGameEngine.reset();
    m_isInitialized = false;
    WasmHost::LogMessage("WasmIntegrationService: Shutdown complete.");
}
void WasmIntegrationService::Update() { /* ... как раньше, m_wasmGameEngine->Update() ... */
    if (!m_isInitialized || !m_wasmGameEngine) return;
    m_wasmGameEngine->Update();
}


// AttachWasmComponent просто передает все в WasmHost::GameEngine
bool WasmIntegrationService::AttachWasmComponent(
    const std::string& ownerId,
    const std::string& scriptSlotName,
    const std::string& wasmModulePath,
    const std::vector<HostFunctionBinding>& specificHostFunctions // Принимаем
) {
    if (!m_isInitialized || !m_wasmGameEngine) {
        WasmHost::LogMessage("WasmIntegrationService ERROR: AttachWasmComponent called while not initialized.");
        return false;
    }
    WasmHost::LogMessage("WasmIntegrationService: Forwarding AttachWasmComponent for Owner: " + ownerId +
    ", Slot: " + scriptSlotName + ", Path: " + wasmModulePath +
    " with " + std::to_string(specificHostFunctions.size()) + " specific bindings.");
    // ownerId становится entityName для WasmHost::GameEngine
    return m_wasmGameEngine->AttachComponent(ownerId, scriptSlotName, wasmModulePath, specificHostFunctions);
}

// DetachWasmComponent
bool WasmIntegrationService::DetachWasmComponent(
    const std::string& ownerId,
    const std::string& scriptSlotName
) {
    if (!m_isInitialized || !m_wasmGameEngine) { /* ... */ return false; }
    WasmHost::LogMessage("WasmIntegrationService: Forwarding DetachWasmComponent for Owner: " + ownerId + ", Slot: " + scriptSlotName);
    return m_wasmGameEngine->UnloadComponentFromSlot(ownerId, scriptSlotName);
}

// SendMessageToWasm
bool WasmIntegrationService::SendMessageToWasm(
    const std::string& hostSenderId,
    const std::string& targetOwnerId,
    const std::string& targetScriptSlotName,
    const std::string& messageContent
) {
    if (!m_isInitialized || !m_wasmGameEngine) { /* ... */ return false; }
    std::string targetWasmEntityAndSlot = targetOwnerId + "." + targetScriptSlotName;
    m_wasmGameEngine->SendMessage(hostSenderId, targetWasmEntityAndSlot, messageContent);
    return true;
}
