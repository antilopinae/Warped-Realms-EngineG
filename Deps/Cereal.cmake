cmake_minimum_required(VERSION 3.26.0)

# --- Dependencies ---

include(Boost.cmake)

set(BUILD_DOC OFF CACHE BOOL "Disable cereal docs" FORCE)

# Wasmtime
if(CUSTOM_DEP_HANDLER_INCLUDED)
    find_or_clone_dependency(cereal
        "https://github.com/USCiLab/cereal.git"
        "v1.3.2"
    )
else()
    message(FATAL_ERROR "cereal target not created.")
endif()

set(BUILD_SANDBOX OFF CACHE BOOL "Disable cereal sandbox" FORCE)

set_property(TARGET cereal PROPERTY CXX_STANDARD 17)
set_property(TARGET cereal PROPERTY CXX_STANDARD_REQUIRED ON)

add_library(cereal-c17 OBJECT cereal-c17.cpp)
target_compile_features(cereal-c17 PRIVATE cxx_std_17)
target_compile_options(cereal-c17 PRIVATE -std=c++17)
target_link_libraries(cereal-c17 PRIVATE cereal)
