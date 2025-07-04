#!/bin/bash

find . -type d \( -name build -o -name cmake-build-debug -o -name Deps -o -name Instruments \) -prune -false -o -type f \( -iname "*.cpp" -o -iname "*.h" -o -iname "*.hpp" -o -iname "*.c" \) | while read -r file; do echo "Formatting $file"; clang-format -i -style=file "$file"; done

