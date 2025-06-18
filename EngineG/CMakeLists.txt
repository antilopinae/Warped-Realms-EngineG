cmake_minimum_required(VERSION 3.20.0)

set(ENGINE_PATH ".")
file(GLOB_RECURSE ENGINE_SOURCES ${ENGINE_PATH}/*.cpp)

add_library(EngineG STATIC ${ENGINE_SOURCES})
target_include_directories(EngineG PUBLIC
  .
  /Actor
  /Component
  /Math
  /Network
  /Screen
  /Wasm
)

target_link_libraries(EngineG PUBLIC
  EngineGDeps
  EngineWasm
)

target_compile_definitions(EngineG PUBLIC SDL_MAIN_HANDLED _USE_MATH_DEFINES)

if(TARGET_WINDOWS)
    target_compile_definitions(EngineG PUBLIC
        _WIN32_WINNT=0x0A00
        _CRT_SECURE_NO_WARNINGS
    )
endif()

if(TARGET_LINUX OR TARGET_MACOS)
    target_compile_definitions(EngineG PUBLIC
        _GNU_SOURCE
    )
endif()

# FetchContent_GetProperties(cereal)
# target_include_directories(EngineG PRIVATE
#   ${cereal_SOURCE_DIR}/include
# )

include(GenerateExportHeader)
generate_export_header(EngineG)
