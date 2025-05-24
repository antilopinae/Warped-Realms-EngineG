add_executable(Game main.cpp ExWasm.cpp
)
target_link_libraries(Game PUBLIC ChimpyGL)

add_dependencies(Game moduleA_c_wasm moduleB_c_wasm moduleC_c_wasm)
