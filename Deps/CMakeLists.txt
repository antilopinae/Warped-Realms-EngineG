cmake_minimum_required(VERSION 3.26.0)

# --- Dependencies ---
find_package(OpenGL REQUIRED)

include(Nlohmann_json.cmake)
# include(Cereal.cmake)

include(EnTT.cmake)
include(Box2cpp.cmake)
# include(Sodium.cmake)
include(Spine-cpp.cmake)

include(Asio.cmake)
include(Yojimbo.cmake)
include(Wasmtime-cpp.cmake)

include(SDL2.cmake)
include(SDL2_image.cmake)

include(Magnum.cmake)
include(MagnumIntegration.cmake)

include(ImNodes.cmake)

add_library(EngineGDeps INTERFACE)
target_compile_definitions(EngineGDeps INTERFACE SDL_MAIN_HANDLED)

target_link_libraries(EngineGDeps INTERFACE
    nlohmann_json::nlohmann_json
    # cereal-c17

    EnTT::EnTT
    box2cpp
    # sodium
    spine-cpp

    asio
    yojimbo
    wasmtime-cpp

    SDL2::SDL2
    SDL2_image::SDL2_image
    # PNG::PNG
    # ZLIB::ZLIB

    Corrade::Utility
    Corrade::PluginManager

    # Magnum::Application
    Magnum::GL
    Magnum::Shaders
    Magnum::Text
    Magnum::TextureTools
    Magnum::MeshTools
    Magnum::Primitives
    Magnum::SceneGraph
    Magnum::Sdl2Application

    ImGui::ImGui
    MagnumIntegration::ImGui

    ImNodes
)
