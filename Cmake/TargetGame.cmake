cmake_minimum_required(VERSION 3.26.0)

if(TARGET_LINUX)
    set_target_properties(Game PROPERTIES
        INSTALL_RPATH "$ORIGIN"
        BUILD_RPATH   "$ORIGIN"
    )
endif()

set(LIBRARIES_TO_COPY_BESIDE_EXECUTABLE
    nlohmann_json::nlohmann_json
    # cereal-c17

    EnTT::EnTT
    box2cpp
    # sodium
    spine-cpp

    asio
    yojimbo
    wasmtime-cpp

    SDL2::SDL2
    SDL2_image::SDL2_image
    # PNG::PNG
    # ZLIB::ZLIB

    Corrade::Utility
    Corrade::PluginManager

    # Magnum::Application
    Magnum::GL
    Magnum::Shaders
    Magnum::Text
    Magnum::TextureTools
    Magnum::MeshTools
    Magnum::Primitives
    Magnum::SceneGraph
    Magnum::Sdl2Application

    ImGui::ImGui
    MagnumIntegration::ImGui

    ImNodes
)

foreach(lib_target_name IN LISTS LIBRARIES_TO_COPY_BESIDE_EXECUTABLE)
    if(TARGET ${lib_target_name})
        get_target_property(target_type ${lib_target_name} TYPE)

        if(target_type STREQUAL "SHARED_LIBRARY" OR target_type STREQUAL "MODULE_LIBRARY" OR target_type STREQUAL "UNKNOWN_LIBRARY")
            get_property(is_interface_lib TARGET ${lib_target_name} PROPERTY INTERFACE_LIBRARY)
            get_property(is_static_lib TARGET ${lib_target_name} PROPERTY TYPE)

            if(NOT is_interface_lib AND NOT (is_static_lib STREQUAL "STATIC_LIBRARY"))
                add_custom_command(TARGET Game POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                        $<TARGET_FILE:${lib_target_name}>
                        "$<TARGET_FILE_DIR:Game>/"
                    COMMENT "Copying ${lib_target_name} with Game"
                    VERBATIM
                )
            endif()
        endif()
    else()
        message(WARNING "Target ${lib_target_name} in list LIBRARIES_TO_COPY_BESIDE_EXECUTABLE not found")
    endif()
endforeach()
