function(copy_target_runtime_dependencies TARGET_NAME)
    if(NOT TARGET ${TARGET_NAME})
        message(WARNING "CopyRuntimeArtifacts: Target '${TARGET_NAME}' not found")
        return()
    endif()

    set(DESTINATION_DIR "$<TARGET_FILE_DIR:${TARGET_NAME}>")

    set(SHARED_DEPS_TARGETS
        SDL2::SDL2
        SDL2_image::SDL2_image
        # PNG::PNG
        # ZLIB::ZLIB

        # Corrade
        Corrade::Utility
        Corrade::PluginManager

        # Magnum
        Magnum::GL
        Magnum::Sdl2Application
        Magnum::Text
        Magnum::TextureTools
        Magnum::MeshTools
        Magnum::Primitives
        Magnum::SceneGraph
        Magnum::Shaders
        Magnum::Trade
        Magnum::Magnum

        # UI
        ImGui::ImGui
        ImNodes
        MagnumIntegration::ImGui

        # Другие
        box2d
        # sodium # Цель от robinlinden/libsodium-cmake
        wasmtime
        wasmtime-cpp
        glfw
    )

    foreach(dep_target IN LISTS SHARED_DEPS_TARGETS)
        if(TARGET ${dep_target})
            get_target_property(target_type ${dep_target} TYPE)

            if(target_type STREQUAL "INTERFACE_LIBRARY" OR target_type STREQUAL "STATIC_LIBRARY" OR target_type STREQUAL "OBJECT_LIBRARY")
                continue()
            endif()

            add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    $<TARGET_FILE:${dep_target}>
                    "${DESTINATION_DIR}/$<TARGET_FILE_NAME:${dep_target}>"
                COMMENT "Copying ${dep_target} -> ${DESTINATION_DIR}/$<TARGET_FILE_NAME:${dep_target}>"
                VERBATIM
            )

            if(UNIX AND NOT APPLE AND NOT ANDROID)
                if(NOT "$<TARGET_SONAME_FILE_NAME:${dep_target}>" STREQUAL "$<TARGET_FILE_NAME:${dep_target}>" AND NOT "$<TARGET_SONAME_FILE_NAME:${dep_target}>" STREQUAL "")
                    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E create_symlink
                            $<TARGET_FILE_NAME:${dep_target}>
                            "${DESTINATION_DIR}/$<TARGET_SONAME_FILE_NAME:${dep_target}>"
                        WORKING_DIRECTORY "${DESTINATION_DIR}"
                        COMMENT "Create SONAME link for ${dep_target}: $<TARGET_SONAME_FILE_NAME:${dep_target}> -> $<TARGET_FILE_NAME:${dep_target}>"
                        VERBATIM
                    )
                endif()

                if(NOT "$<TARGET_LINKER_FILE_NAME:${dep_target}>" STREQUAL "$<TARGET_SONAME_FILE_NAME:${dep_target}>" AND NOT "$<TARGET_LINKER_FILE_NAME:${dep_target}>" STREQUAL "")
                     add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E create_symlink
                            $<TARGET_SONAME_FILE_NAME:${dep_target}>
                            "${DESTINATION_DIR}/$<TARGET_LINKER_FILE_NAME:${dep_target}>"
                        WORKING_DIRECTORY "${DESTINATION_DIR}"
                        COMMENT "Create linker name link for ${dep_target}: $<TARGET_LINKER_FILE_NAME:${dep_target}> -> $<TARGET_SONAME_FILE_NAME:${dep_target}>"
                        VERBATIM
                    )
                endif()
            elseif(WIN32)
                if(MINGW AND EXISTS "$<TARGET_LINKER_FILE:${dep_target}>" AND NOT "$<TARGET_LINKER_FILE_NAME:${dep_target}>" STREQUAL "$<TARGET_FILE_NAME:${dep_target}>")
                    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                            $<TARGET_LINKER_FILE:${dep_target}>
                            "${DESTINATION_DIR}/$<TARGET_LINKER_FILE_NAME:${dep_target}>"
                        COMMENT "Copying MinGW import lib ${dep_target} -> ${DESTINATION_DIR}/$<TARGET_LINKER_FILE_NAME:${dep_target}>"
                        VERBATIM
                    )
                endif()
            endif()
        else()
            message(WARNING "CopyRuntimeArtifacts: Target ${dep_target} in list SHARED_DEPS_TARGETS not found")
        endif()
    endforeach()
endfunction()
