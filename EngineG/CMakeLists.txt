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

# FetchContent_GetProperties(cereal)
# target_include_directories(EngineG PRIVATE
#   ${cereal_SOURCE_DIR}/include
# )

include(GenerateExportHeader)
generate_export_header(EngineG)
