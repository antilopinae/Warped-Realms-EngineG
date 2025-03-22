cmake_minimum_required(VERSION 3.20.0)

add_executable(Bootstrap Bootstrap.cpp)

target_link_libraries(Bootstrap PRIVATE ChimpyGL)
