cmake_minimum_required(VERSION 3.26.0)

# dependencies
list(APPEND CMAKE_PREFIX_PATH "$ENV{HOME}/magnum-install")
list(APPEND CMAKE_PREFIX_PATH "$ENV{HOME}/magnum-integration-install")
list(APPEND CMAKE_PREFIX_PATH "$ENV{HOME}/corrade-install")

find_package(Corrade REQUIRED Utility)

find_package(Magnum REQUIRED
    GL
    Sdl2Application
    Shaders
    Primitives
    MeshTools
    SceneGraph
    TextureTools
)
find_package(MagnumIntegration REQUIRED ImGui)
