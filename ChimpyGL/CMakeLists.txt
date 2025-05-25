cmake_minimum_required(VERSION 3.20.0)

set(ENGINE_PATH ".")
file(GLOB_RECURSE ENGINE_SOURCES ${ENGINE_PATH}/*.cpp)

add_library(ChimpyGL STATIC ${ENGINE_SOURCES})

target_include_directories(ChimpyGL PUBLIC .)

include(GenerateExportHeader)
generate_export_header(ChimpyGL)

message(WARNING "You must install Wasmtime yourself and ensure the following paths are correct!")

set(WASMTIME_INCLUDE_DIR "/usr/local/include")
set(WASMTIME_LIB_DIR "/usr/local/lib")

include_directories(${WASMTIME_INCLUDE_DIR})
link_directories(${WASMTIME_LIB_DIR})

message(STATUS "Wasmtime include path: ${WASMTIME_INCLUDE_DIR}")
message(STATUS "Wasmtime lib path:    ${WASMTIME_LIB_DIR}")


target_link_libraries(ChimpyGL PUBLIC
        SDL2::SDL2
        SDL2_image::SDL2_image
        box2d
        asio
        wasmtime-cpp
        nlohmann_json::nlohmann_json
)
