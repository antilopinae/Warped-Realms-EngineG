cmake_minimum_required(VERSION 3.26.0)

# dependencies
FindOrCloneRep("wasmtime-cpp" "https://github.com/bytecodealliance/wasmtime-cpp" "main" OFF)

message(STATUS "You must install Wasmtime yourself and ensure the following paths are correct!")

set(WASMTIME_INCLUDE_DIR "/usr/local/include")
set(WASMTIME_LIB_DIR "/usr/local/lib")

include_directories(${WASMTIME_INCLUDE_DIR})
link_directories(${WASMTIME_LIB_DIR})

message(STATUS "Wasmtime include path: ${WASMTIME_INCLUDE_DIR}")
message(STATUS "Wasmtime lib path:    ${WASMTIME_LIB_DIR}")
