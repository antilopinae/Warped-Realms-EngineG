// wall_module.cpp
#include <iostream>
#include <string>
#include <emscripten/bind.h>
#include "core/actor/Actor.hpp"
#include "engine/Game.hpp"
#include "SDL_rect.h"

#define WALL_MODULE
#include "WASMModuleBindings.h"

class Wall : public EngineG::Actor {
public:
    Wall(EngineG::Game* game) : EngineG::Actor(game) {
        std::cout << "Wall WASM class instance created" << std::endl;
    }
    ~Wall() override = default;

private:
    void ActorUpdate(float deltaTime) override {
        SDL_Rect rect = {400, 400, 64, 64};
        DrawTexture("assets/wall.png", rect, 2);
    }
};


extern "C" int wall_module_create_actor(EngineG::Game* engine, int entityId) {
    std::cout << "wall_module_create_actor called in WASM, entityId: " << entityId << std::endl;
    Wall* wallActor = new Wall(engine);
    return reinterpret_cast<int>(wallActor); // Возвращаем указатель на Actor как int
}


extern "C" void wall_module_update(int entityId) {
    auto gamePtr = EngineG::ServiceLocator::get().resolveService<EngineG::Game>();
    if (!gamePtr) return;
    EngineG::Game* engine = gamePtr.get();
    // Получаем указатель на Wall Actor (приведение обратно к Wall*)
    EngineG::Actor* actorPtr = engine->mActorsWASM["wall"];
    if (actorPtr) {
        actorPtr->Update(0.0f); // Directly call update, deltaTime is not used in this simple example
    }
}