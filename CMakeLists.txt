cmake_minimum_required(VERSION 3.20.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

project(ChimpyGL VERSION 1.0.0 LANGUAGES CXX)
enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

set(CMAKE_CXX_SCAN_FOR_MODULES NO)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(CMAKE_EXPERIMENTAL_CXX_MODULE_MAP_FORMAT "clang")
  set(CMAKE_EXE_LINKER_FLAGS -lstdc++)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)

include(NoInSourceBuilds)
include(CMakePrintHelpers)
include(CCache)

include(FindOrCloneSomeRep)

# dependencies
FindOrCloneRep("glfw" "https://github.com/glfw/glfw.git" "master" OFF)
FindOrCloneRep("box2d" "https://github.com/erincatto/box2d" "v3.0.0" OFF)
FindOrCloneRep("entt" "https://github.com/skypjack/entt.git" "master" OFF)
# FindOrCloneRep("SPIDERMONKEY" "https://github.com/mozilla-spidermonkey/spidermonkey.dev" "production" OFF)
FindOrCloneRep("quickjs" "https://github.com/bellard/quickjs" "master" OFF)

#add_library(quickjs STATIC
#        quickjs.c
#        quickjs-libc.c
#        quickjs.h
#        quickjs-libc.h
#)
#
#target_include_directories(quickjs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# dependencies
FindOrCloneRep("SDL3" "https://github.com/libsdl-org/SDL.git" "main" OFF)
include_directories(${SDL2_INCLUDE_DIRS})

# dependencies
FindOrCloneRep("box2cpp" "https://github.com/HolyBlackCat/box2cpp" "box2d-3.0.0" OFF)
set(BOX2CPP_PATH  "Extern/extern-box2cpp")
add_library(box2cpp INTERFACE)
target_include_directories(box2cpp INTERFACE ${BOX2CPP_PATH}/include)

# dependencies
FindOrCloneRep("imgui" "https://github.com/ocornut/imgui.git" "master" OFF)
set(IMGUI_PATH  "extern/extern-imgui")
file(GLOB IMGUI_SOURCES ${IMGUI_PATH}/*.cpp)
add_library(imgui STATIC ${IMGUI_SOURCES}
    ${IMGUI_PATH}/backends/imgui_impl_glfw.cpp
    ${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp
    # ${IMGUI_PATH}/backends/imgui_impl_opengl3_loader.cpp
)
target_include_directories(imgui PUBLIC "${CMAKE_CURRENT_LIST_DIR}/extern/extern-imgui")
target_include_directories(imgui PUBLIC "${CMAKE_CURRENT_LIST_DIR}/extern/extern-imgui/backends")
target_link_libraries(imgui PUBLIC glfw)

#dependencies
FindOrCloneRep("asio" "https://github.com/chriskohlhoff/asio.git" "asio-1-31-0" OFF)
FetchContent_GetProperties(asio)
if(NOT asio_POPULATED)
  message(STATUS "Fetching asio ${ASIO_CMAKE_ASIO_TAG}")

  if(EXISTS "${ASIO_CMAKE_ASIO_DEP_DIR}/asio")
    set(FETCHCONTENT_SOURCE_DIR_ASIO ${ASIO_CMAKE_ASIO_DEP_DIR})
    message(STATUS "asio ${ASIO_CMAKE_ASIO_TAG} source found at ${ASIO_CMAKE_ASIO_DEP_DIR}; skip fetching.")
  endif()

  FetchContent_MakeAvailable(asio)
  message(STATUS "Populating asio ${ASIO_CMAKE_ASIO_TAG} done; src at ${asio_SOURCE_DIR}")
endif()

add_library(asio INTERFACE)
target_include_directories(asio
        INTERFACE ${asio_SOURCE_DIR}/asio/include
)
target_compile_definitions(asio
        INTERFACE
        ASIO_STANDALONE
        ASIO_NO_DEPRECATED
)

if(UNIX)
  target_link_libraries(asio
          INTERFACE pthread
  )
elseif(WIN32)
  # macro see @ https://stackoverflow.com/a/40217291/1746503
  macro(get_WIN32_WINNT version)
    if (CMAKE_SYSTEM_VERSION)
      set(ver ${CMAKE_SYSTEM_VERSION})
      string(REGEX MATCH "^([0-9]+).([0-9])" ver ${ver})
      string(REGEX MATCH "^([0-9]+)" verMajor ${ver})
      # Check for Windows 10, b/c we'll need to convert to hex 'A'.
      if ("${verMajor}" MATCHES "10")
        set(verMajor "A")
        string(REGEX REPLACE "^([0-9]+)" ${verMajor} ver ${ver})
      endif ("${verMajor}" MATCHES "10")
      # Remove all remaining '.' characters.
      string(REPLACE "." "" ver ${ver})
      # Prepend each digit with a zero.
      string(REGEX REPLACE "([0-9A-Z])" "0\\1" ver ${ver})
      set(${version} "0x${ver}")
    endif(CMAKE_SYSTEM_VERSION)
  endmacro(get_WIN32_WINNT)

  if(NOT DEFINED _WIN32_WINNT)
    get_WIN32_WINNT(ver)
    set(_WIN32_WINNT ${ver})
  endif()

  message(STATUS "Set _WIN32_WINNET=${_WIN32_WINNT}")

  target_compile_definitions(asio
          INTERFACE
          _WIN32_WINNT=${_WIN32_WINNT}
          WIN32_LEAN_AND_MEAN
  )
endif()

#dependencies
find_package(glfw REQUIRED)
find_package(box2d REQUIRED)
find_package(SDL3 REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(entt REQUIRED)
# find_package(SPIDERMONKEY REQUIRED)
find_package(quickjs REQUIRED)
find_package(box2cpp REQUIRED)
find_package(imgui REQUIRED)
find_package(asio REQUIRED)

#dependencies
find_package(OpenGL REQUIRED)
find_package(spdlog REQUIRED)
find_package(docopt REQUIRED)
find_package(fmt REQUIRED)


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_definitions(DEBUG)
endif()

add_subdirectory(chimpy)
add_subdirectory(modules)
add_subdirectory(examples)
