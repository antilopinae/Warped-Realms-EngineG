# EngineG - Игровой движок с интеграцией WebAssembly

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

**EngineG** - это 2D игровой движок на C++, созданный для гибкой разработки игр с помощью WebAssembly (Wasm). Используйте мощь C++ для ядра движка и пишите игровую логику на языках, компилируемых в Wasm (C, C++, Rust), которая безопасно исполняется благодаря рантайму Wasmtime.

## ✨ Ключевые Возможности

<details>
  <summary><strong>🚀 Продвинутая Интеграция WebAssembly</strong></summary>

  *   **Гибкий Скриптинг:** Разрабатывайте поведение игровых объектов и компонентов на C/C++/Rust, компилируя их в Wasm.
  *   **Безопасность Превыше Всего:** Wasm-модули выполняются в изолированной среде Wasmtime, защищая основной движок.
  *   **Настраиваемый API Хоста:** C++ компоненты могут предоставлять кастомные функции (host functions) для Wasm, обеспечивая контролируемое и мощное взаимодействие.
  *   **Динамическая Загрузка Модулей:** Поддержка загрузки Wasm-пакетов (модуль + манифест) по сети для обновлений "на лету" или расширений.
  *   **Полный Жизненный Цикл:** Стандартизированные функции (`initialize`, `update(deltaTime)`, `onMessage`, `terminate`) для управления Wasm-компонентами.
  *   **Система Сообщений:** Механизм для обмена данными между C++ и Wasm, а также между разными Wasm-модулями.
</details>

<details>
  <summary><strong>⚙️ Архитектура Движка</strong></summary>

  *   **Entity-Component Система:** В основе лежат Актеры (`Actor`) и Компоненты (`Component`), позволяющие создавать сложные игровые сущности.
  *   **Рендеринг на SDL2:** Использование проверенной библиотеки SDL2 для 2D-графики, обработки ввода и звука.
  *   **Сетевые Функции:** Включает базовую клиент-серверную архитектуру (ASIO) и систему для загрузки Wasm-пакетов по сети.
</details>

<details>
  <summary><strong>🛠️ Технологии</strong></summary>

  *   **C++20 / 17**
  *   **Wasmtime**
  *   **SDL2 & SDL2_image**
  *   **ASIO**
  *   **nlohmann/json**
  *   **CMake**
</details>

## 🚀 Как Начать

<details>
  <summary><strong>Установка и Запуск</strong></summary>

  Для сборки проекта вам понадобятся:
  *   Компилятор C++ (GCC, Clang, MSVC).
  *   CMake.

  **Общие шаги сборки:**
  1.  Клонируйте репозиторий.
  2.  Создайте директорию `build` и перейдите в нее.
  3.  Выполните `cmake ..` (возможно, с указанием `-DCMAKE_PREFIX_PATH` для библиотек в нестандартных местах).
  4.  Соберите проект: `cmake --build .` или `make`.
  5.  Запустите исполняемый файл из директории сборки. Убедитесь, что директория `Assets/` доступна.

  *Более подробные инструкции по зависимостям и сборке будут добавлены позже.*
</details>

## 💡 Принцип Работы Wasm-Интеграции

<details>
  <summary><strong>Обзор Механизма</strong></summary>

  Интеграция Wasm в EngineG позволяет отделить игровую логику от основного кода движка. Вот как это работает:

  1.  **C++ Компоненты:** Базовые строительные блоки (например, `SpriteComponent`, `MoveComponent`) реализуются на C++.
  2.  **Wasm-Скрипты:** Более высокоуровневая логика, ИИ, или специфическое поведение могут быть написаны на C/C++/Rust и скомпилированы в `.wasm` файлы.
  3.  **Привязка:** C++ компонент может запросить привязку Wasm-скрипта к определенному "слоту" на своем `Actor`-владельце.
  4.  **Хост-Функции:** C++ компонент может предоставить Wasm-скрипту набор специальных функций (host functions), через которые Wasm сможет взаимодействовать с состоянием этого компонента или движком в целом (например, изменить текстуру спрайта, получить позицию игрока).
  5.  **WasmEngine & WasmComponent:** Эти C++ классы управляют всем процессом: загружают `.wasm` файлы, компилируют их с помощью Wasmtime, инстанцируют, связывают хост-функции и управляют жизненным циклом Wasm-модуля (`initialize`, `update`, `onMessage`, `terminate`).
  6.  **Исполнение:** В игровом цикле, C++ движок вызывает функцию `update(deltaTime)` у активных Wasm-модулей, передавая время кадра, что позволяет Wasm-логике выполняться синхронно с игрой.

  Эта архитектура обеспечивает гибкость, позволяя легко изменять или добавлять новую игровую логику без перекомпиляции всего движка, а также повышает безопасность за счет выполнения Wasm в песочнице.
</details>

## 🔧 Пример Wasm-Модуля на C

<details>
  <summary><strong>`example_script.c`</strong></summary>

  ```c
  #include <stdio.h>
  #include <string.h>
  #include <stdlib.h>

  #ifndef EMSCRIPTEN_KEEPALIVE
  #define EMSCRIPTEN_KEEPALIVE __attribute__((used)) __attribute__((visibility("default")))
  #endif

  // --- Импорты из C++ хоста ---
  __attribute__((import_module("env"))) __attribute__((import_name("engine_log_wasm")))
  extern void engine_log_wasm(const char* ptr, int len);

  // Пример импорта специфичной хост-функции (если C++ компонент ее предоставляет)
  // __attribute__((import_module("env"))) __attribute__((import_name("component_set_speed")))
  // extern void component_set_speed(float new_speed);

  // --- Обязательные экспорты для аллокации памяти хостом ---
  EMSCRIPTEN_KEEPALIVE
  void* allocate(int size) {
      return malloc(size); // Или ваш кастомный аллокатор
  }

  EMSCRIPTEN_KEEPALIVE
  void deallocate(void* ptr, int size) {
      (void)size;
      free(ptr); // Или ваш кастомный деаллокатор
  }

  // --- Экспорты жизненного цикла ---
  EMSCRIPTEN_KEEPALIVE
  void initialize(void) {
      engine_log_wasm("WASM_EXAMPLE: Module Initialized!", strlen("WASM_EXAMPLE: Module Initialized!"));
  }

  EMSCRIPTEN_KEEPALIVE
  void update(float deltaTime) {
      char buffer[128];
      snprintf(buffer, sizeof(buffer), "WASM_EXAMPLE: Update called! DeltaTime: %f", deltaTime);
      engine_log_wasm(buffer, strlen(buffer));

      // Пример вызова хост-функции, если она была импортирована
      // component_set_speed(100.0f * deltaTime);
  }

  EMSCRIPTEN_KEEPALIVE
  void onMessage(const char* from_ptr, int from_len, const char* msg_ptr, int msg_len) {
      char log_buffer[512];
      int prefix_len = snprintf(log_buffer, sizeof(log_buffer), "WASM_EXAMPLE: Message from '%.*s': '", from_len, from_ptr);
      if (prefix_len < 0 || prefix_len >= sizeof(log_buffer)) { /* ошибка */ return; }

      // Копируем тело сообщения, если есть место
      int remaining_space = sizeof(log_buffer) - prefix_len;
      int to_copy = (msg_len < remaining_space - 2) ? msg_len : remaining_space - 2; // -2 for closing ' and \0
      if (to_copy > 0) {
        memcpy(log_buffer + prefix_len, msg_ptr, to_copy);
      }
      log_buffer[prefix_len + to_copy] = '\'';
      log_buffer[prefix_len + to_copy + 1] = '\0';

      engine_log_wasm(log_buffer, strlen(log_buffer));
  }

  EMSCRIPTEN_KEEPALIVE
  void terminate(void) {
      engine_log_wasm("WASM_EXAMPLE: Module Terminated.", strlen("WASM_EXAMPLE: Module Terminated."));
  }
  ```

</details>
🗺️ Дальнейшее Развитие
<details>
<summary><strong>Идеи и Планы</strong></summary>
    Улучшенный менеджер ресурсов.
    Полноценная система UI.
    Редактор уровней/сцен.
    Расширение API хост-функций для Wasm.
    Стабильная система уникальных ID для Actor-ов.
    Улучшенная документация по API для Wasm-разработчиков.
</details>
🤝 Вклад
<details>
<summary><strong>Присоединяйтесь к Разработке!</strong></summary>
    Мы всегда рады новым идеям и помощи! Если вы хотите внести свой вклад:
        Сделайте форк репозитория.
        Создайте ветку для ваших изменений.
        Внесите изменения и сделайте коммиты.
        Отправьте изменения в ваш форк.
        Создайте Pull Request с описанием ваших изменений.
        Будем благодарны за соблюдение стиля кода и добавление комментариев.
</details>
📝 Лицензия
Этот проект распространяется под лицензией MIT.
Подробнее см. в файле [LICENSE](LICENSE.md).

Разработано с использованием C++ и магии WebAssembly! ✨
