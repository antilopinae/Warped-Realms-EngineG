add_library(console window.cpp cui.cpp)

set_target_properties(imgui PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_link_libraries(console PUBLIC
glfw
OpenGL::GL
imgui
)

target_include_directories(console PUBLIC #[["../../extern/extern-imgui/backends"]] imgui)

target_include_directories(console PUBLIC
"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

set_target_properties(console PROPERTIES
PUBLIC_HEADER WarpedRealms/console/include/cui.hpp
POSITION_INDEPENDENT_CODE 1
)
