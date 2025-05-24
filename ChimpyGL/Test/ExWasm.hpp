#pragma one

#include "../Game.hpp"
#include "../WasmEngine/WasmController.hpp"
#include "../WasmEngine/WasmEngine.hpp"
#include "../WasmEngine/WasmHostApi.hpp"
#include <chrono>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <thread>
#include <vector>

namespace EngineG {

void ExWasm() {
    std::unique_ptr<Wasm::WasmEngine> gameEngine;
    try {
        gameEngine = std::make_unique<Wasm::WasmEngine>();
    } catch (const std::exception& e) {
        std::cerr << "Main FATAL: Failed to initialize WasmEngine: " << e.what() << std::endl;
        return;
    }

    Wasm::WasmController actor(gameEngine.get());

    try {
        actor.AttachComponent("Player", "scriptB", ASSETS_DIR "moduleB_c.wasm");
        actor.AttachComponent("Player", "scriptA", ASSETS_DIR "moduleA_c.wasm");

        for (int i = 0; i < 2; ++i) {
            Wasm::LogMessage("Main loop frame " + std::to_string(i));
            if (i == 0) {
                actor.SendMessage("host_main", "Player.scriptA", "MSG_FROM_HOST_TO_B_1");
            }
            gameEngine->Update();
        }

        actor.AttachComponent("Player", "scriptB", ASSETS_DIR "moduleC_c.wasm");

        Wasm::LogMessage("Main: === Game loop after moduleC replaced moduleB ===");
        for (int i = 0; i < 2; ++i) {
            Wasm::LogMessage("Main loop frame (2 + " + std::to_string(i) + ")");
            if (i == 0) {
                actor.SendMessage("host_main", "Player.scriptB", "MSG_FROM_HOST_TO_B_2");
            }
            gameEngine->Update();
        }

        actor.UnloadComponentFromSlot("Player", "scriptB");
        actor.UnloadComponentFromSlot("Player", "scriptA");
    } catch (const std::exception& e) {
        Wasm::LogMessage("Main: Unhandled exception in main logic: " + std::string(e.what()));
        return;
    }
}

}  // namespace EngineG
