cmake_minimum_required(VERSION 3.26.0)

# --- Dependencies ---

include(Box2d.cmake)

# box2cpp
if(CUSTOM_DEP_HANDLER_INCLUDED)
    find_or_clone_dependency(box2cpp
        "https://github.com/HolyBlackCat/box2cpp.git"
        "box2d-3.0.0"
    )
else()
    message(FATAL_ERROR "box2cpp target not created.")
endif()

set(BOX2CPP_PATH  "Extern/box2cpp")

add_library(box2cpp INTERFACE
#        ${BOX2CPP_PATH}/include/box2cpp/box2cpp.h
)

target_include_directories(box2cpp INTERFACE ${BOX2CPP_PATH}/include)
target_link_libraries(box2cpp INTERFACE box2d)
target_compile_features(box2cpp INTERFACE cxx_std_20)
