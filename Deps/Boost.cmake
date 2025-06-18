cmake_minimum_required(VERSION 3.26.0)

# --- Dependencies ---

add_library(zstd::libzstd_shared INTERFACE IMPORTED)
add_library(zstd::libzstd INTERFACE IMPORTED)
add_definitions(-DBOOST_IOSTREAMS_NO_ZSTD)

# Boost
if(CUSTOM_DEP_HANDLER_INCLUDED)
    find_or_clone_dependency(Boost
        "https://github.com/boostorg/boost.git"
        "boost-1.88.0"
        CMAKE_ARGS
            -DBOOST_IOSTREAMS_NO_ZSTD=ON
    )
else()
    message(FATAL_ERROR "boost target not created.")
endif()

if(TARGET boost_cobalt) # Проверяем, существует ли цель boost_cobalt
    if(WIN32 OR (CMAKE_CROSSCOMPILING AND CMAKE_SYSTEM_NAME STREQUAL "Windows"))
        message(STATUS "Добавление ws2_32 к цели boost_cobalt.")
        # Для MinGW мы линкуем с ws2_32 (это имя библиотеки без префикса lib и суффикса .a)
        # CMake автоматически найдет libws2_32.a в путях MinGW.
        target_link_libraries(boost_cobalt INTERFACE ws2_32)
        # Используем INTERFACE, так как boost_cobalt - это SHARED_LIBRARY,
        # и если что-то линкуется с boost_cobalt, оно может транзитивно нуждаться в символах,
        # которые ws2_32 предоставляет через boost_cobalt.
        # Можно попробовать PRIVATE, если это только внутренняя зависимость boost_cobalt.
    endif()
else()
    message(WARNING "Цель boost_cobalt не найдена после конфигурации Boost. Не могу добавить ws2_32.")
endif()
