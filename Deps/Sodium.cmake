cmake_minimum_required(VERSION 3.26.0)

# --- Dependencies ---

# Sodium
if(CUSTOM_DEP_HANDLER_INCLUDED)
    find_or_clone_dependency(sodium
        # "https://github.com/jedisct1/libsodium.git"
        "https://github.com/robinlinden/libsodium-cmake.git"
        "master"
    )
else()
    message(FATAL_ERROR "sodium target not created.")
endif()
