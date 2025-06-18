cmake_minimum_required(VERSION 3.26.0)

# --- Dependencies ---

find_package(Rust QUIET)
if(NOT Rust_FOUND)
    message(WARNING "Rust toolchain (cargo) not found. Building Wasmtime C-API may be fail")
else()
    message(STATUS "Found Rust: ${Rust_CARGO_EXECUTABLE}")
endif()
#
#
# set(WASMTIME_C_API_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/wasmtime/crates/c-api)
# if(NOT EXISTS "${WASMTIME_C_API_SOURCE_DIR}/CMakeLists.txt")
#     message(FATAL_ERROR "CMakeLists.txt для Wasmtime C-API не найден в ${WASMTIME_C_API_SOURCE_DIR}")
# endif()
#
# set(BUILD_SHARED_LIBS ON CACHE BOOL "Сборка Wasmtime C-API как shared library" FORCE)
# set(WASMTIME_BUILD_TESTS OFF CACHE BOOL "Отключить тесты для Wasmtime C-API" FORCE)
#
# add_subdirectory(${WASMTIME_C_API_SOURCE_DIR} ${CMAKE_BINARY_DIR}/wasmtime-c-api-build)

# 1. Wasmtime C-API
set(WASMTIME_C_API_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/wasmtime/crates/c-api)
if(NOT EXISTS "${WASMTIME_C_API_SOURCE_DIR}/CMakeLists.txt")
    message(FATAL_ERROR "CMakeLists.txt для Wasmtime C-API не найден в ${WASMTIME_C_API_SOURCE_DIR}")
endif()

# Опции для Wasmtime C-API
set(BUILD_SHARED_LIBS ON CACHE BOOL "Сборка Wasmtime C-API как shared library" FORCE)
set(WASMTIME_BUILD_TESTS OFF CACHE BOOL "Отключить тесты для Wasmtime C-API" FORCE)

if(CMAKE_CROSSCOMPILING AND CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # Устанавливаем Rust target для MinGW
    set(WASMTIME_TARGET "x86_64-pc-windows-gnu" CACHE STRING "Rust target to build Wasmtime C-API for" FORCE)
    message(STATUS "Для Wasmtime C-API установлен Rust target: ${WASMTIME_TARGET}")
else()
    # Для нативной сборки можно оставить пустым, чтобы он определил хост
    set(WASMTIME_TARGET "" CACHE STRING "Rust target to build Wasmtime C-API for" FORCE)
endif()
# Если ты всегда собираешь только для Windows из этого CMake, можно жестко задать:
# set(WASMTIME_TARGET "x86_64-pc-windows-gnu" CACHE STRING "Rust target to build Wasmtime C-API for" FORCE)


add_subdirectory(${WASMTIME_C_API_SOURCE_DIR} ${CMAKE_BINARY_DIR}/wasmtime-c-api-build)
message(STATUS "Wasmtime C-API сконфигурирован. Цель 'wasmtime' должна быть доступна.")



# 2. Wasmtime-cpp (использует уже определенную цель 'wasmtime')
# Предполагается, что сабмодуль wasmtime-cpp находится в Deps/wasmtime-cpp
# set(WASMTIME_CPP_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/wasmtime-cpp)
# if(NOT EXISTS "${WASMTIME_CPP_SOURCE_DIR}/CMakeLists.txt")
#     message(FATAL_ERROR "CMakeLists.txt для wasmtime-cpp не найден в ${WASMTIME_CPP_SOURCE_DIR}")
# endif()
#
# # Опции для wasmtime-cpp (из его CMakeLists.txt)
# set(WASMTIME_CPP_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # Если есть такая опция
# set(WASMTIME_CPP_BUILD_TESTS OFF CACHE BOOL "" FORCE)    # Переименовал, чтобы не конфликтовать с C-API
# # add_subdirectory ожидает, что цель 'wasmtime' уже существует и найдет ее.
# add_subdirectory(${WASMTIME_CPP_SOURCE_DIR} ${CMAKE_BINARY_DIR}/wasmtime-cpp-build)
# # Эта команда создаст INTERFACE цель 'wasmtime-cpp'
# message(STATUS "wasmtime-cpp сконфигурирован. Цель 'wasmtime-cpp' должна быть доступна.")
