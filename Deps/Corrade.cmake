cmake_minimum_required(VERSION 3.26.0)

# --- Dependencies ---

# Corrade
set(CORRADE_WITH_UTILITY ON CACHE BOOL "" FORCE)
set(CORRADE_WITH_PLUGINMANAGER ON CACHE BOOL "" FORCE)
set(CORRADE_WITH_TESTSUITE OFF CACHE BOOL "" FORCE)
set(CORRADE_WITH_RC ON CACHE BOOL "Ensure corrade-rc is built" FORCE)

add_subdirectory(corrade EXCLUDE_FROM_ALL)

message(STATUS "Corrade was configured.")

find_package(Corrade REQUIRED Utility PluginManager)

# --- КОПИРОВАНИЕ MinGW DLL для corrade-rc через execute_process (НА ЭТАПЕ КОНФИГУРАЦИИ) ---
# Этот блок выполняется СРАЗУ после add_subdirectory для Corrade.
# Цель corrade-rc уже должна быть известна CMake, но сам .exe файл еще не собран.
# Мы копируем DLL в предполагаемую директорию вывода для corrade-rc.exe.
if(CMAKE_CROSSCOMPILING AND CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # Цель corrade-rc должна определять, куда будет помещен .exe файл.
    # Обычно это ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} этого подпроекта или глобальный.
    # Если Corrade наследует CMAKE_RUNTIME_OUTPUT_DIRECTORY из твоего главного проекта,
    # то это будет ${CMAKE_BINARY_DIR}/bin (т.е. build-win/bin).
    set(CORRADE_RC_EXPECTED_OUTPUT_DIR "${CMAKE_BINARY_DIR}/bin") # ПРОВЕРЬ ЭТОТ ПУТЬ!
                                                                # Он должен совпадать с тем, куда CMake положит corrade-rc.exe
    file(MAKE_DIRECTORY ${CORRADE_RC_EXPECTED_OUTPUT_DIR}) # Создаем директорию, если ее нет

    message(STATUS "[Deps] Копирование MinGW DLL для corrade-rc в ${CORRADE_RC_EXPECTED_OUTPUT_DIR} (через execute_process)")

    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        get_filename_component(MINGW_CXX_COMPILER_DIR_REAL ${CMAKE_CXX_COMPILER} DIRECTORY)
        get_filename_component(MINGW_TOOLCHAIN_BASE_DIR_REAL ${MINGW_CXX_COMPILER_DIR_REAL} DIRECTORY)

        set(DLL_SEARCH_HINTS_REAL
            "/usr/lib"
            "/usr/lib64"
            "/usr/x86_64-w64-mingw32/lib"
            "/usr/x86_64-w64-mingw32/bin"
        )
        if(CMAKE_CXX_COMPILER_VERSION)
            string(REGEX MATCH "^[0-9]+\\.[0-9]+\\.[0-9]+" GCC_VERSION_STRING_REAL "${CMAKE_CXX_COMPILER_VERSION}")
            if(GCC_VERSION_STRING_REAL)
                list(APPEND DLL_SEARCH_HINTS_REAL "${MINGW_TOOLCHAIN_BASE_DIR_REAL}/lib/gcc/x86_64-w64-mingw32/${GCC_VERSION_STRING_REAL}")
                list(APPEND DLL_SEARCH_HINTS_REAL "/usr/lib/gcc/x86_64-w64-mingw32/${GCC_VERSION_STRING_REAL}")
            endif()
        endif()
        list(REMOVE_DUPLICATES DLL_SEARCH_HINTS_REAL)
        message(STATUS "  Поиск MinGW DLL в (HINTS): ${DLL_SEARCH_HINTS_REAL}")

        find_file(MINGW_LIBGCC_DLL_PATH_REAL NAMES libgcc_s_seh-1.dll libgcc_s_dw2-1.dll HINTS ${DLL_SEARCH_HINTS_REAL} NO_DEFAULT_PATH)
        find_file(MINGW_LIBSTDCXX_DLL_PATH_REAL NAMES libstdc++-6.dll HINTS ${DLL_SEARCH_HINTS_REAL} NO_DEFAULT_PATH)
        find_file(MINGW_LIBPTHREAD_DLL_PATH_REAL NAMES libwinpthread-1.dll HINTS ${DLL_SEARCH_HINTS_REAL} NO_DEFAULT_PATH)

        set(COPIED_ANY_DLL_FOR_RC_EXEC FALSE)
        if(MINGW_LIBGCC_DLL_PATH_REAL)
            execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different
                            "${MINGW_LIBGCC_DLL_PATH_REAL}"
                            "${CMAKE_BINARY_DIR}/bin/"
                            RESULT_VARIABLE _copy_res)
            if(_copy_res EQUAL 0)
                message(STATUS "    + libgcc DLL (${MINGW_LIBGCC_DLL_PATH_REAL}) скопирована (или up-to-date) для corrade-rc.")
                set(COPIED_ANY_DLL_FOR_RC_EXEC TRUE)
            else()
                message(WARNING "    - ОШИБКА копирования libgcc DLL для corrade-rc. Код: ${_copy_res}")
            endif()
        else()
            message(WARNING "    - MinGW libgcc DLL НЕ НАЙДЕНА для corrade-rc.")
        endif()

        if(MINGW_LIBSTDCXX_DLL_PATH_REAL)
            execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different
                            "${MINGW_LIBSTDCXX_DLL_PATH_REAL}"
                            "${CMAKE_BINARY_DIR}/bin/"
                            RESULT_VARIABLE _copy_res)
            if(_copy_res EQUAL 0)
                message(STATUS "    + libstdc++ DLL (${MINGW_LIBSTDCXX_DLL_PATH_REAL}) скопирована (или up-to-date) для corrade-rc.")
                set(COPIED_ANY_DLL_FOR_RC_EXEC TRUE)
            else()
                message(WARNING "    - ОШИБКА копирования libstdc++ DLL для corrade-rc. Код: ${_copy_res}")
            endif()
        else()
            message(WARNING "    - MinGW libstdc++ DLL НЕ НАЙДЕНА для corrade-rc.")
        endif()

        if(MINGW_LIBPTHREAD_DLL_PATH_REAL)
                execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different
                                "${MINGW_LIBPTHREAD_DLL_PATH_REAL}"
                                "${CMAKE_BINARY_DIR}/bin/"
                                RESULT_VARIABLE _copy_res)
            if(_copy_res EQUAL 0)
                message(STATUS "    + libwinpthread DLL (${MINGW_LIBPTHREAD_DLL_PATH_REAL}) скопирована (или up-to-date) для corrade-rc.")
                set(COPIED_ANY_DLL_FOR_RC_EXEC TRUE)
            else()
                message(WARNING "    - ОШИБКА копирования libwinpthread DLL для corrade-rc. Код: ${_copy_res}")
            endif()
        else()
            message(STATUS "    - MinGW libwinpthread DLL не найдена (может быть не нужна).")
        endif()

        if(NOT COPIED_ANY_DLL_FOR_RC_EXEC)
            message(WARNING "[Deps] Ни одна из ключевых MinGW DLL не была скопирована для corrade-rc через execute_process.")
        endif()
    else()
        message(WARNING "[Deps] Не GNU компилятор для MinGW? Не удалось автоматически определить пути к DLL для копирования через execute_process.")
    endif()
endif() # if(CMAKE_CROSSCOMPILING ... )
# --- КОНЕЦ КОПИРОВАНИЯ MinGW DLL через execute_process ---
