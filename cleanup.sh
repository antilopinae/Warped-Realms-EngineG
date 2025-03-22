#!/bin/bash

BUILD_DIR="../Assets/Plugins/build"

EXEC_EXTENSIONS=("out" "exe" "app" "bin" "run" "sh" "a" "so" "dylib" "dll" "lib")

is_executable_or_library() {
    local file=$1
    if [[ "$file" == *.* ]]; then
        for ext in "${EXEC_EXTENSIONS[@]}"; do
            if [[ "$file" == *.$ext ]]; then
                return 0
            fi
        done
    fi
    return 1
}

find "$BUILD_DIR" -type f | while read file; do
    if ! is_executable_or_library "$file"; then
        echo "Удаление: $file"
        rm "$file"
    fi
done

find "$BUILD_DIR" -type d -empty -delete

echo "Очистка завершена!"
