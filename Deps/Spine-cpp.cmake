cmake_minimum_required(VERSION 3.26.0)

# --- Dependencies ---

# Spine-cpp
if(CUSTOM_DEP_HANDLER_INCLUDED)
    find_or_clone_dependency(spinecpp_esoteric
        "https://github.com/EsotericSoftware/spine-runtimes.git"
        "4.1"
        SOURCE_SUBDIR "spine-cpp"
        CMAKE_ARGS
            -DSPINE_SHARED_LIB=ON
            -DCMAKE_POSITION_INDEPENDENT_CODE=ON
            -DSPINE_COMPILE_EXAMPLES=OFF
            -DSPINE_SFML_GRAPHICS=OFF
            -DSPINE_SDL_GRAPHICS=OFF
        EXPECTED_TARGET spine-cpp
    )
else()
    message(FATAL_ERROR "spine-cpp target not created.")
endif()
