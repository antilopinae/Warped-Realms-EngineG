cmake_minimum_required(VERSION 3.26.0)

# --- Dependencies ---

# GLFW
if(CUSTOM_DEP_HANDLER_INCLUDED)
    find_or_clone_dependency(glfw
        "https://github.com/glfw/glfw.git"
        "master"
    )
else()
    message(FATAL_ERROR "GLFW target not created.")
endif()
