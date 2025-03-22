cmake_minimum_required(VERSION 3.10)
project(math_wasm)

set(CMAKE_CXX_STANDARD 14)

# Configure Emscripten toolchain
set(CMAKE_TOOLCHAIN_FILE "/usr/lib/emscripten/emcc/cmake/Modules/Platform/Emscripten.cmake")  # Replace with your Emscripten path

# Specify the Emscripten compilers (Crucial!)
set(CMAKE_C_COMPILER "/usr/lib/emscripten/emcc") # Replace with your emcc path
set(CMAKE_CXX_COMPILER "/usr/lib/emscripten/em++") # Replace with your em++ path


add_library(math OBJECT math.cpp) # Create an object file

add_library(mathlib SHARED $<TARGET_OBJECTS:math>)
set_target_properties(mathlib
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
# install(TARGET ${math} OBJECTS DESTINATION "")
