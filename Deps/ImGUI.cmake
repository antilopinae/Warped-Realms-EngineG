cmake_minimum_required(VERSION 3.26.0)

# dependencies
FindOrCloneRep("imgui" "https://github.com/ocornut/imgui" "master" OFF)

set(IMGUI_PATH  "Extern/extern-imgui")
file(GLOB IMGUI_SOURCES ${IMGUI_PATH}/*.cpp)

add_library(imgui STATIC ${IMGUI_SOURCES}
    # ${IMGUI_PATH}/backends/imgui_impl_glfw.cpp
    # ${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp
    # ${IMGUI_PATH}/backends/imgui_impl_opengl3_loader.cpp
)
target_include_directories(imgui PUBLIC ${IMGUI_PATH})
target_include_directories(imgui PUBLIC ${IMGUI_PATH})

add_library(ImGui::ImGui ALIAS imgui)

# dependencies
FindOrCloneRep("glfw" "https://github.com/glfw/glfw" "master" OFF)
