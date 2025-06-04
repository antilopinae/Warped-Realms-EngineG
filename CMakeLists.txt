cmake_minimum_required(VERSION 3.26.0)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/Cmake")

include(NoInSourceBuilds)
include(CMakePrintHelpers)
include(CCache)
include(FindOrCloneSomeRep)

project(ChimpyGL VERSION 1.0.0 LANGUAGES CXX C)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

set(CMAKE_CXX_SCAN_FOR_MODULES NO)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")

set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(CMAKE_EXPERIMENTAL_CXX_MODULE_MAP_FORMAT "clang")
  set(CMAKE_EXE_LINKER_FLAGS -lstdc++)
endif()

set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_definitions(DEBUG)
endif()

add_subdirectory(Deps)
add_subdirectory(ChimpyGL)
add_subdirectory(WasmModules)
add_subdirectory(Game)
