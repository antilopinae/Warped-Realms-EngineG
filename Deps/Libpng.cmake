if(NOT DEFINED CUSTOM_DEP_HANDLER_INCLUDED)
    message(FATAL_ERROR "NOT DEFINED CUSTOM_DEP_HANDLER_INCLUDED")
endif()

if(CUSTOM_DEP_HANDLER_INCLUDED)
    find_or_clone_dependency(libpng_fc
        "https://github.com/glennrp/libpng.git"
        "v1.6.43"
        CMAKE_ARGS
            -DPNG_SHARED=ON
            -DPNG_STATIC=OFF
            -DPNG_TESTS=OFF
            -DCMAKE_POSITION_INDEPENDENT_CODE=ON
        EXPECTED_TARGET PNG::PNG
    )
    if(TARGET PNG::PNG)
        message(STATUS "PNG::PNG target is available.")
    else()
        message(WARNING "PNG::PNG target NOT found.")
    endif()
endif()
