cmake_minimum_required(VERSION 3.20.0)

set(ENGINE_PATH ".")
file(GLOB_RECURSE ENGINE_SOURCES ${ENGINE_PATH}/*.cpp)

add_library(ChimpyGL STATIC ${ENGINE_SOURCES})
target_include_directories(ChimpyGL PUBLIC .)

target_link_libraries(ChimpyGL PUBLIC ChimpyDeps)

FetchContent_GetProperties(cereal)
target_include_directories(ChimpyGL PRIVATE
  ${cereal_SOURCE_DIR}/include
)

include(GenerateExportHeader)
generate_export_header(ChimpyGL)
