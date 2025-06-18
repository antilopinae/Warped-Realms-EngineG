cmake_minimum_required(VERSION 3.26.0)

# --- Dependencies ---

# SDL2
if(CUSTOM_DEP_HANDLER_INCLUDED)
    set(SDL_SHARED ON CACHE BOOL "" FORCE)
    set(SDL_STATIC OFF CACHE BOOL "" FORCE)

    find_or_clone_dependency(SDL2
        "https://github.com/libsdl-org/SDL.git"
        "release-2.32.8"
        CMAKE_ARGS
            -DSDL_STATIC=OFF
            -DSDL_SHARED=ON
            -DSDL_TEST=OFF
            -DCMAKE_POSITION_INDEPENDENT_CODE=ON
            -DBUILD_TESTS=OFF
            -DBUILD_TESTING=OFF

        EXPECTED_TARGET SDL2::SDL2
    )
else()
    message(FATAL_ERROR "SDL2 target not created.")
endif()
