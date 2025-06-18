cmake_minimum_required(VERSION 3.26.0)

# --- Dependencies ---

include(ImGUI.cmake)

set(IMGUI_DIR ${CMAKE_CURRENT_LIST_DIR}/Extern/imgui CACHE STRING "" FORCE)

# MagnumIntegration
set(MAGNUM_WITH_IMGUI ON CACHE BOOL "" FORCE)
set(WITH_IMGUI ON CACHE BOOL "" FORCE)
set(MAGNUM_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(MAGNUM_WITH_BULLET OFF CACHE BOOL "" FORCE)
set(MAGNUM_WITH_EIGEN OFF CACHE BOOL "" FORCE)

add_subdirectory(magnum-integration EXCLUDE_FROM_ALL)

message(STATUS "Magnum-Integration was configured")

find_package(MagnumIntegration REQUIRED ImGui)
