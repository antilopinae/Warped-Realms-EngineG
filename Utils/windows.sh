#!/bin/bash
set -e

cmake -DCMAKE_TOOLCHAIN_FILE=Toolchain-mingw-x86_64.cmake -B build-win -S . -DCMAKE_CROSSCOMPILING_EMULATOR="wine"

cmake --build build-win

cmake -DCMAKE_TOOLCHAIN_FILE=Toolchain-mingw-x86_64.cmake -B build-win -S . -DCMAKE_CROSSCOMPILING_EMULATOR="wine"

read -r -p "Continue?: " BUILD_WINDOWS_ANSWER

cd build-win
cmake --install . --prefix ../install-win
cd ..
