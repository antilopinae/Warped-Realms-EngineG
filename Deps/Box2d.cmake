cmake_minimum_required(VERSION 3.26.0)

# --- Dependencies ---

# box2d
if(CUSTOM_DEP_HANDLER_INCLUDED)
    find_or_clone_dependency(box2d
        "https://github.com/erincatto/box2d.git"
        "v3.0.0"
    )
else()
    message(FATAL_ERROR "box2d target not created.")
endif()
