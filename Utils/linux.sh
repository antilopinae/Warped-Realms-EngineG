#!/bin/bash
set -e

cmake -B build-lin -S .
cmake --build build-lin
cmake -B build-lin -S .

read -r -p "Continue?: " BUILD_WINDOWS_ANSWER

cd build-lin
cmake --install . --prefix ../install-lin
cd ..
