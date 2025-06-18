cmake_minimum_required(VERSION 3.26.0)

# --- Dependencies ---

include (GLFW.cmake)

# ImGui
if(CUSTOM_DEP_HANDLER_INCLUDED)
    find_or_clone_dependency(imgui
        "https://github.com/ocornut/imgui.git"
        "v1.89.9"
    )
else()
    message(FATAL_ERROR "imgui target not created.")
endif()

set(IMGUI_PATH  "Extern/imgui")
file(GLOB IMGUI_SOURCES ${IMGUI_PATH}/*.cpp)

set(IMGUI_BACKENDS
    ${IMGUI_PATH}/backends/imgui_impl_glfw.cpp
    ${IMGUI_PATH}/backends/imgui_impl_sdl2.cpp
    ${IMGUI_PATH}/backends/imgui_impl_sdlrenderer2.cpp
)

if(TARGET_WINDOWS)
    list(APPEND IMGUI_BACKENDS
        ${IMGUI_PATH}/backends/imgui_impl_win32.cpp
    )
endif()

add_library(imgui STATIC
    ${IMGUI_SOURCES}
    ${IMGUI_BACKENDS}
)

target_include_directories(imgui PUBLIC ${IMGUI_PATH})
target_include_directories(imgui PUBLIC ${IMGUI_PATH}/backends)
target_compile_definitions(imgui PUBLIC IMGUI_DEFINE_MATH_OPERATORS)
target_compile_options(imgui PRIVATE -fPIC)
target_compile_definitions(imgui PRIVATE IMGUI_DISABLE_WIN32_FUNCTIONS)

add_library(ImGui::ImGui ALIAS imgui)

target_include_directories(imgui PUBLIC
    ${IMGUI_SOURCE_DIR}
    ${IMGUI_SOURCE_DIR}/backends
)

add_library(ImGui::Sources ALIAS imgui)
target_link_libraries(imgui PUBLIC glfw)
target_link_libraries(imgui PUBLIC SDL2::SDL2)
