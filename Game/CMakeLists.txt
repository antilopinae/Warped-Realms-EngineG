set(GAME_PATH ".")
file(GLOB_RECURSE GAME_SOURCES ${GAME_PATH}/*.cpp)

add_executable(Game ${GAME_SOURCES})

target_link_libraries(Game PUBLIC EngineG)

add_dependencies(Game moduleA_c_wasm moduleB_c_wasm moduleC_c_wasm)

add_subdirectory(WasmModules)

include(TargetGame)
include(CopyRuntimeArtifacts)
copy_target_runtime_dependencies(Game)

include(InstallRules)
setup_target_installation_rules(Game)
