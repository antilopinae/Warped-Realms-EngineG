cmake_minimum_required(VERSION 3.26.0)

# --- Dependencies ---

# nlohmann_json
if(CUSTOM_DEP_HANDLER_INCLUDED)
    find_or_clone_dependency(nlohmann_json
        "https://github.com/nlohmann/json.git"
        "develop"
        EXPECTED_TARGET nlohmann_json::nlohmann_json
    )
else()
    message(FATAL_ERROR "nlohmann_json target not created.")
endif()
