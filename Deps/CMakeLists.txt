cmake_minimum_required(VERSION 3.26.0)

# dependencies
FindOrCloneRep("nlohmann_json" "https://github.com/nlohmann/json" "develop" OFF)
FindOrCloneRep("entt" "https://github.com/skypjack/entt" "master" OFF)
FindOrCloneRep("cereal" "https://github.com/USCiLab/cereal" "master" OFF)
FindOrCloneRep("sodium" "https://github.com/jedisct1/libsodium" "master" OFF)
FindOrCloneRep("yojimbo" "https://github.com/mas-bandwidth/yojimbo" "main" OFF)

add_subdirectory(Extern/spine-cpp)

include(Yojimbo.cmake)
include(Asio.cmake)
include(Wasmtime.cmake)
include(ImGUI.cmake)
include(MagnumCorrade.cmake)
include(ImNodes.cmake)

find_package(Boost REQUIRED)
find_package(box2d REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(asio REQUIRED)
find_package(OpenGL REQUIRED)

add_library(ChimpyDeps INTERFACE)

target_link_libraries(ChimpyDeps INTERFACE
      SDL2::SDL2
      SDL2_image::SDL2_image
      box2d
      asio
      wasmtime-cpp
      nlohmann_json::nlohmann_json
      EnTT::EnTT
      cereal
      sodium
      yojimbo
      box2d
      spine-cpp
      Corrade::Utility
      Magnum::Application
      Magnum::GL
      Magnum::Sdl2Application
      Magnum::SceneGraph
      Magnum::Shaders
      Magnum::Primitives
      Magnum::MeshTools
      Magnum::TextureTools
      MagnumIntegration::ImGui
      ImGui::ImGui
      ImNodes
)

target_include_directories(ChimpyDeps INTERFACE
    "$ENV{HOME}/magnum-integration-install/include"
)

