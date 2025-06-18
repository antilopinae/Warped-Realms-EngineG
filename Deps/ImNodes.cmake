cmake_minimum_required(VERSION 3.26.0)

# --- Dependencies ---

# ImNodes
if(CUSTOM_DEP_HANDLER_INCLUDED)
    find_or_clone_dependency(imnodes
        "https://github.com/Nelarius/imnodes.git"
        "v0.5"
        EXPECTED_TARGET ImNodes
    )
else()
    message(FATAL_ERROR "imnodes target not created.")
endif()

add_library(ImNodes STATIC
    Extern/imnodes/imnodes.cpp
)

target_include_directories(ImNodes PUBLIC Extern/imnodes)
target_link_libraries(ImNodes PUBLIC imgui)
target_compile_definitions(ImNodes PRIVATE IMGUI_DEFINE_MATH_OPERATORS)
