// player_module.cpp
#include <iostream>
#include <string>
#include <emscripten/bind.h>
#include "core/actor/Actor.hpp"
#include "engine/Game.hpp"
#include "SDL_rect.h"

#define PLAYER_MODULE
#include "WASMModuleBindings.h"

class Player : public EngineG::Actor {
public:
    Player(EngineG::Game* game) : EngineG::Actor(game) {
        std::cout << "Player WASM class instance created" << std::endl;
    }
    ~Player() override = default;

private:
    void ActorUpdate(float deltaTime) override {
        SDL_Rect rect = {200, 200, 64, 64};
        DrawTexture("assets/player.png", rect, 3); // Player отрисовывается выше всех (drawOrder = 3)
    }
};


extern "C" int player_module_create_actor(EngineG::Game* engine, int entityId) {
    std::cout << "player_module_create_actor called in WASM, entityId: " << entityId << std::endl;
    Player* playerActor = new Player(engine);
    return reinterpret_cast<int>(playerActor); // Возвращаем указатель на Actor как int
}


extern "C" void player_module_update(int entityId) {
    auto gamePtr = EngineG::ServiceLocator::get().resolveService<EngineG::Game>();
    if (!gamePtr) return;
    EngineG::Game* engine = gamePtr.get();
    // Получаем указатель на Player Actor (приведение обратно к Player*)
    EngineG::Actor* actorPtr = engine->mActorsWASM["player"];
    if (actorPtr) {
        actorPtr->Update(0.0f); // Directly call update, deltaTime is not used in this simple example
    }
}