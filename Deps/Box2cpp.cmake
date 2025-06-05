cmake_minimum_required(VERSION 3.26.0)

# dependencies
FindOrCloneRep("box2d" "https://github.com/erincatto/box2d" "v3.0.0" OFF)

# dependencies
FindOrCloneRep("box2cpp" "https://github.com/HolyBlackCat/box2cpp" "box2d-3.0.0" OFF)

set(BOX2CPP_PATH  "Extern/extern-box2cpp")

add_library(box2cpp INTERFACE
#        ${BOX2CPP_PATH}/include/box2cpp/box2cpp.h
)

target_include_directories(box2cpp INTERFACE ${BOX2CPP_PATH}/include)
target_link_libraries(box2cpp INTERFACE box2d)
target_compile_features(box2cpp INTERFACE cxx_std_20)
