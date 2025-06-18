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

add_library(imgui STATIC ${IMGUI_SOURCES}
    # ${IMGUI_PATH}/backends/imgui_impl_glfw.cpp
    # ${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp
    # ${IMGUI_PATH}/backends/imgui_impl_opengl3_loader.cpp
)
target_include_directories(imgui PUBLIC ${IMGUI_PATH})
target_include_directories(imgui PUBLIC ${IMGUI_PATH})
target_compile_definitions(imgui INTERFACE IMGUI_DEFINE_MATH_OPERATORS)
target_compile_options(imgui PRIVATE -fPIC)

add_library(ImGui::ImGui ALIAS imgui)

add_library(ImGuiSourcesInterface INTERFACE)
target_include_directories(ImGuiSourcesInterface INTERFACE
    ${IMGUI_SOURCE_DIR}
    ${IMGUI_SOURCE_DIR}/backends
)

add_library(ImGui::Sources ALIAS ImGuiSourcesInterface)
target_link_libraries(imgui PUBLIC glfw)
