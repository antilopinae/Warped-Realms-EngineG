#!/bin/bash
set -e

step() {
    echo
    echo "------------------------------------------------------------------------"
    echo "-- $1"
    echo "------------------------------------------------------------------------"
}

step "Начало процесса сборки для Linux"

step "Linux: Конфигурация (1-й проход)"
cmake -B build-lin -S .

step "Linux: Сборка"
cmake --build build-lin

step "Linux: Повторная конфигурация для установки (2-й проход)"
cmake -B build-lin -S .

step "Linux: Установка"
echo "Переход в директорию build-lin"
cd build-lin
echo "Текущая директория: $(pwd)"
echo "Выполнение: cmake --install . --prefix ../_InstallLin"
cmake --install . --prefix ../_InstallLin
echo "Возврат в корневую директорию проекта"
cd ..
echo "Текущая директория: $(pwd)"

step "Процесс сборки для Linux успешно завершен"
echo


read -r -p "Хотите выполнить сборку для Windows (требуется Toolchain-mingw-x86_64.cmake)? (y/N): " BUILD_WINDOWS_ANSWER

BUILD_WINDOWS_ANSWER_LOWER=$(echo "$BUILD_WINDOWS_ANSWER" | tr '[:upper:]' '[:lower:]')

if [[ "$BUILD_WINDOWS_ANSWER_LOWER" == "y" || "$BUILD_WINDOWS_ANSWER_LOWER" == "yes" ]]; then
    step "Начало процесса сборки для Windows"

    TOOLCHAIN_FILE="Toolchain-mingw-x86_64.cmake"

    if [ ! -f "$TOOLCHAIN_FILE" ]; then
        echo "ОШИБКА: Файл тулчейна для Windows '$TOOLCHAIN_FILE' не найден в текущей директории: $(pwd)"
        echo "Пожалуйста, убедитесь, что он существует в корне проекта. Пропуск сборки для Windows."
    else
        echo "Используется файл тулчейна: $(pwd)/$TOOLCHAIN_FILE"

        step "Windows: Конфигурация (1-й проход)"
        cmake -B build-win -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" -S .

        step "Windows: Сборка"
        cmake --build build-win

        step "Windows: Повторная конфигурация для установки (2-й проход)"
        cmake -B build-win -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" -S .

        step "Windows: Установка"
        echo "Переход в директорию build-win"
        cd build-win
        echo "Текущая директория: $(pwd)"
        echo "Выполнение: cmake --install . --prefix ../_InstallWin"
        cmake --install . --prefix ../_InstallWin
        echo "Возврат в корневую директорию проекта"
        cd ..
        echo "Текущая директория: $(pwd)"

        step "Процесс сборки для Windows успешно завершен"
    fi
else
    step "Пропуск сборки для Windows"
fi

echo
step "Все запрошенные процессы сборки завершены."
