cmake_minimum_required(VERSION 3.20.0)

set(ENGINE_WASM_PATH ".")
file(GLOB_RECURSE ENGINE_WASM_SOURCES ${ENGINE_WASM_PATH}/*.cpp)

add_library(EngineWasm STATIC ${ENGINE_WASM_SOURCES})
target_include_directories(EngineWasm PUBLIC .)

target_link_libraries(EngineWasm PUBLIC EngineGDeps)

include(GenerateExportHeader)
generate_export_header(EngineWasm)
