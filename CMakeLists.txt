cmake_minimum_required(VERSION 3.26.0 FATAL_ERROR)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Cmake")

include(CCache OPTIONAL)
include(NoInSourceBuilds OPTIONAL)

project(SDLCompile VERSION 1.0.0 LANGUAGES CXX C)

cmake_policy(SET CMP0135 NEW)

include(ProjectSetup)
include(PlatformDetection)
include(DependencyManagement)

add_subdirectory(EngineWasm)
add_subdirectory(EngineG)
add_subdirectory(Game)
# add_subdirectory(Test) # not now
