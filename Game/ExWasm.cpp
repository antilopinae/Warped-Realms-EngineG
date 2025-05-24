#include "WasmEngine/HostApi.hpp"
#include "WasmEngine/ActorController.hpp"
#include "WasmEngine/GameEngine.hpp"

#include <iostream>
#include <vector>
#include <stdexcept>
#include <thread>
#include <chrono>
#include <Game.hpp>
#include <memory>

void ExWasm() {
    std::unique_ptr<WasmHost::GameEngine> gameEngine;
    try {
        gameEngine = std::make_unique<WasmHost::GameEngine>();
    } catch (const std::exception& e) {
        std::cerr << "Main FATAL: Failed to initialize GameEngine: " << e.what() << std::endl;
        return;
    }

    WasmHost::ActorController actor(gameEngine.get());

    try {
      actor.AttachComponent("Player", "scriptB", ASSETS_DIR "moduleB_c.wasm");
      actor.AttachComponent("Player", "scriptA", ASSETS_DIR "moduleA_c.wasm");

      for (int i = 0; i < 2; ++i) {
            WasmHost::LogMessage("Main loop frame " + std::to_string(i));
            if (i == 0) {
                actor.SendMessage("host_main", "Player.scriptA", "MSG_FROM_HOST_TO_B_1");
            }
            gameEngine->Update();
        }

        actor.AttachComponent("Player", "scriptB", ASSETS_DIR "moduleC_c.wasm");

        WasmHost::LogMessage("Main: === Game loop after moduleC replaced moduleB ===");
        for (int i = 0; i < 2; ++i) {
            WasmHost::LogMessage("Main loop frame (2 + " + std::to_string(i) + ")");
            if (i == 0) {
                 actor.SendMessage("host_main", "Player.scriptB", "MSG_FROM_HOST_TO_B_2");
            }
            gameEngine->Update();
        }

        actor.UnloadComponentFromSlot("Player", "scriptB");
        actor.UnloadComponentFromSlot("Player", "scriptA");
    } catch (const std::exception& e) {
        WasmHost::LogMessage("Main: Unhandled exception in main logic: " + std::string(e.what()));
        return;
    }
}