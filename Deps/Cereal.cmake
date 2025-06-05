cmake_minimum_required(VERSION 3.26.0)

# dependencies
find_package(Boost REQUIRED)

option(BUILD_SANDBOX "Disable cereal sandbox" OFF)
set(BUILD_SANDBOX OFF CACHE BOOL "Disable cereal sandbox" FORCE)

FindOrCloneRep("cereal" "https://github.com/USCiLab/cereal" "v1.3.2" OFF)

set_property(TARGET cereal PROPERTY CXX_STANDARD 17)
set_property(TARGET cereal PROPERTY CXX_STANDARD_REQUIRED ON)

add_library(cereal-c17 OBJECT cereal-c17.cpp)
target_compile_features(cereal-c17 PRIVATE cxx_std_17)
target_compile_options(cereal-c17 PRIVATE -std=c++17)
target_link_libraries(cereal-c17 PRIVATE cereal)
target_include_directories(cereal-c17 PRIVATE Extern/extern-cereal/include)
