// ship_module.cpp
#include <emscripten/bind.h>

#include <iostream>
#include <string>

#include "SDL_rect.h"
#include "core/actor/Actor.hpp"
#include "engine/Game.hpp"

#define SHIP_MODULE
#include "WASMModuleBindings.h"

class Ship : public EngineG::Actor {
 public:
  Ship(EngineG::Game* game) : EngineG::Actor(game) { std::cout << "Ship WASM class instance created" << std::endl; }
  ~Ship() override = default;

 private:
  void ActorUpdate(float deltaTime) override {
    SDL_Rect rect = {100, 100, 64, 64};
    DrawTexture("assets/ship.png", rect, 1);
    EngineG::Actor::Vector2 pos = GetPosition();
    pos.x += 1.0f;
    if (pos.x > 1024.0f) pos.x = -64.0f;
    SetPosition(pos);
  }
};

extern "C" int ship_module_create_actor(EngineG::Game* engine, int entityId) {
  std::cout << "ship_module_create_actor called in WASM, entityId: " << entityId << std::endl;
  Ship* shipActor = new Ship(engine);
  return reinterpret_cast<int>(shipActor);  // Возвращаем указатель на Actor как int
}

extern "C" void ship_module_update(int entityId) {
  auto gamePtr = EngineG::ServiceLocator::get().resolveService<EngineG::Game>();
  if (!gamePtr) return;
  EngineG::Game* engine = gamePtr.get();
  // Получаем указатель на Ship Actor (приведение обратно к Ship*)
  EngineG::Actor* actorPtr = engine->mActorsWASM["ship"];
  if (actorPtr) {
    actorPtr->Update(0.0f);  // Directly call update, deltaTime is not used in this simple example
  }
}