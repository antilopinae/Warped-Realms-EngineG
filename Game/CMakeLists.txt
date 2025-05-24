add_executable(Game main.cpp ExQuickJs.cpp ExWasm.cpp
        WasmEngine/ActorController.cpp
        WasmEngine/HostApi.cpp
        WasmEngine/ActorController.cpp
        WasmEngine/WasmComponent.cpp
        WasmEngine/GameEngine.cpp
)
target_link_libraries(Game PUBLIC ChimpyGL)

add_dependencies(Game moduleA_c_wasm moduleB_c_wasm moduleC_c_wasm)

# include(Format)
# Format(Game ./WasmEngine)
