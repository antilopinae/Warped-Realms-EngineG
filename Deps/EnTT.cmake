cmake_minimum_required(VERSION 3.26.0)

# --- Dependencies ---

# EnTT::EnTT
if(CUSTOM_DEP_HANDLER_INCLUDED)
    find_or_clone_dependency(entt
        "https://github.com/skypjack/entt.git"
        "master"
    )
else()
    message(FATAL_ERROR "entt target not created.")
endif()
