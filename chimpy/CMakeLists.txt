cmake_minimum_required(VERSION 3.20.0)

include_directories(include)

set(ENGINE_PATH  "engine")
#file(GLOB_RECURSE ENGINE_SOURCES ${ENGINE_PATH}/*.cpp)

add_library(ChimpyGL STATIC ChimpyGL.cpp ${ENGINE_SOURCES})

include_directories(${SPIDERMONKEY_SOURCE_DIR}/install/include)
link_directories(${SPIDERMONKEY_SOURCE_DIR}/install/lib)

include(GenerateExportHeader)
generate_export_header(ChimpyGL)

#add_subdirectory(console)

add_library(quickjs STATIC IMPORTED)
set_target_properties(quickjs PROPERTIES
        IMPORTED_LOCATION "/usr/local/lib/quickjs/libquickjs.a"
        INTERFACE_INCLUDE_DIRECTORIES "/usr/local/include/quickjs"
)

target_link_libraries(ChimpyGL PUBLIC
        quickjs
#        console
        SDL2::SDL2
        SDL2_image::SDL2_image
        spdlog::spdlog
        EnTT
#        mozjs-115
#        docopt
        fmt::fmt
        box2d
#        box2cpp
        asio
)


## Add executable
#add_executable(game main.cpp
#        core/actor/Actor.cpp
#        core/actor/Actor.hpp
#        core/actor/Ship.cpp
#        core/actor/Ship.hpp
#        core/actor/Wall.cpp
#        core/actor/Wall.hpp
#        core/actor/Player.cpp
#        core/actor/Player.hpp
#        core/command/Command.hpp
#        core/command/CommandQueue.cpp
#        core/command/CommandQueue.hpp
#        core/command/SetRenderableCommand.cpp
#        core/command/SetRenderableCommand.hpp
#        engine/Game.cpp
#        engine/Game.hpp
#        engine/SpiderMonkeyEngine.cpp
#        engine/SpiderMonkeyEngine.hpp
#        core/service_locator/ServiceLocator.hpp
#        SDL.hpp
#)




# target_include_directories(PhysicsEngineDLL PUBLIC engine)

# test SWIG
# find_package(SWIG REQUIRED)
# include(${SWIG_USE_FILE})
#
# set(SWIG_PROJECT_DIR "${CMAKE_CURRENT_BINARY_DIR}")
#
# # Add swig flags here
# set(CMAKE_SWIG_FLAGS "")
#
# set_property(SOURCE animal.i PROPERTY CPLUSPLUS ON)
# set_source_files_properties(animal.i PROPERTIES SWIG_FLAGS "-includeall")
#
# swig_add_library(animal
#  TYPE SHARED
#  LANGUAGE CSharp
#  SOURCES animal.i Animal.cpp
#  OUTPUT_DIR ${SWIG_PROJECT_DIR}/../../../Scripts/SWIG
#  OUTFILE_DIR ${SWIG_PROJECT_DIR}/../../../Scripts/SWIG
#  )
#
#  # for copying animal.dll to .Net project dir dir
# set_target_properties( animal
#    PROPERTIES
#    # These copy animal.dll on Windows to .Net project directory
#    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${SWIG_PROJECT_DIR}
#    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${SWIG_PROJECT_DIR}
#
#    # This copies animal.so on Linux to .Net project directory
#    LIBRARY_OUTPUT_DIRECTORY ${SWIG_PROJECT_DIR}
#
#    # Set address of C++ headers
#    INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}
# )
