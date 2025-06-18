if(NOT DEFINED CUSTOM_DEP_HANDLER_INCLUDED)
    message(FATAL_ERROR "NOT DEFINED CUSTOM_DEP_HANDLER_INCLUDED")
endif()

if(CUSTOM_DEP_HANDLER_INCLUDED)
    find_or_clone_dependency(zlib_fc
        "https://github.com/madler/zlib.git"
        "v1.3.1"
        CMAKE_ARGS
            -DBUILD_SHARED_LIBS=ON
            -DCMAKE_POSITION_INDEPENDENT_CODE=ON
        EXPECTED_TARGET zlib
    )

    if(TARGET zlib AND NOT TARGET ZLIB::ZLIB)
        add_library(ZLIB::ZLIB ALIAS zlib)
    endif()
    if(TARGET ZLIB::ZLIB)
        message(STATUS "ZLIB::ZLIB target is available.")
    else()
        message(WARNING "ZLIB::ZLIB target NOT found.")
    endif()
endif()
