cmake_minimum_required(VERSION 3.26.0)

# dependencies
add_library(ImNodes STATIC
    Extern/extern-imnodes/imnodes.cpp
)

target_include_directories(ImNodes PUBLIC Extern/extern-imnodes)
target_link_libraries(ImNodes PUBLIC imgui)
