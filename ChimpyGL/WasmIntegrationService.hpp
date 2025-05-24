#pragma once

#include "WasmHostShared.hpp" // Теперь используем его
#include <memory>
#include <string>
#include <vector>

// Forward-declaration
namespace WasmHost { class GameEngine; }

class WasmIntegrationService {
public:
    static WasmIntegrationService& GetInstance();

    WasmIntegrationService(const WasmIntegrationService&) = delete;
    WasmIntegrationService& operator=(const WasmIntegrationService&) = delete;

    bool Initialize();
    void Shutdown();
    void Update();

    // API для EngineG
    // ownerId: Уникальный ID владельца из EngineG (например, Actor ID)
    // scriptSlotName: Имя слота на этом владельце (например, "controller", "renderer_hook")
    // wasmModulePath: Путь к .wasm файлу
    // specificHostFunctions: Вектор функций, которые владелец (EngineG::Component) хочет предоставить этому WASM
    bool AttachWasmComponent(
        const std::string& ownerId,
        const std::string& scriptSlotName,
        const std::string& wasmModulePath,
        const std::vector<HostFunctionBinding>& specificHostFunctions // Этот параметр остается!
    );

    bool DetachWasmComponent(
        const std::string& ownerId,
        const std::string& scriptSlotName
    );

    bool SendMessageToWasm(
        const std::string& hostSenderId,
        const std::string& targetOwnerId,
        const std::string& targetScriptSlotName,
        const std::string& messageContent
    );

private:
    WasmIntegrationService();
    ~WasmIntegrationService();

    std::unique_ptr<WasmHost::GameEngine> m_wasmGameEngine;
    bool m_isInitialized = false;
};
