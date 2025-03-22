// WASMModuleBindings.h
#ifndef WASM_MODULE_BINDINGS_H
#define WASM_MODULE_BINDINGS_H

#include <emscripten/bind.h>
#include "engine/Game.hpp"
#include "core/service_locator/ServiceLocator.hpp"
#include "core/actor/Actor.hpp"
#include <SDL_rect.h>

using Rect = SDL_Rect; // Define alias for SDL_Rect for convenience

// Forward declarations of module-specific update and create_actor functions
extern "C" void ship_module_update(int entityId);
extern "C" int ship_module_create_actor(EngineG::Game* engine, int entityId);
extern "C" void wall_module_update(int entityId);
extern "C" int wall_module_create_actor(EngineG::Game* engine, int entityId);
extern "C" void player_module_update(int entityId);
extern "C" int player_module_create_actor(EngineG::Game* engine, int entityId);


// Generic EMSCRIPTEN_BINDINGS block that will dispatch to specific module functions
EMSCRIPTEN_BINDINGS(WASM_Module_Bindings) {
    // Ship Module Bindings
    #ifdef SHIP_MODULE
    emscripten::function("UpdateActor", &ship_module_update);
    emscripten::function("CreateActor", &ship_module_create_actor);

    // Wall Module Bindings
    #elif defined(WALL_MODULE)
    emscripten::function("UpdateActor", &wall_module_update);
    emscripten::function("CreateActor", &wall_module_create_actor);

    // Player Module Bindings
    #elif defined(PLAYER_MODULE)
    emscripten::function("UpdateActor", &player_module_update);
    emscripten::function("CreateActor", &player_module_create_actor);

    #else
    #error "No module type defined for WASMModuleBindings. Define SHIP_MODULE, WALL_MODULE, or PLAYER_MODULE."
    #endif

    emscripten::class_<EngineG::Game>("Game")
        .smart_ptr<std::shared_ptr<EngineG::Game>>("Game") // Add smart_ptr support
        ;

    emscripten::class_<EngineG::Actor::Vector2>("Vector2")
        .constructor<float, float>()
        .property("x", &EngineG::Actor::Vector2::x)
        .property("y", &EngineG::Actor::Vector2::y)
        ;


    emscripten::class_<EngineG::Actor>("Actor")
        .constructor<EngineG::Game*>()
        .function("SetPosition", &EngineG::Actor::SetPosition)
        .function("GetPosition", &EngineG::Actor::GetPosition)
        .function("DrawTexture", emscripten::optional_override([](EngineG::Actor& actor, const std::string& textureName, Rect dstRect, int drawOrder){
            actor.DrawTexture(textureName, dstRect, drawOrder);
        }))
        ;

    emscripten::value_object<Rect>("Rect")
        .field("x", &Rect::x)
        .field("y", &Rect::y)
        .field("w", &Rect::w)
        .field("h", &Rect::h)
        ;

    emscripten::function("resolveGame", emscripten::optional_override([]() {
        return EngineG::ServiceLocator::get().resolveService<EngineG::Game>();
    }));
}

#endif // WASM_MODULE_BINDINGS_H