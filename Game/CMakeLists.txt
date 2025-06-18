set(GAME_PATH ".")
file(GLOB_RECURSE GAME_SOURCES ${GAME_PATH}/*.cpp)

add_executable(Game ${GAME_SOURCES})

target_link_libraries(Game PUBLIC EngineG)

if(MINGW)
    target_link_libraries(Game PRIVATE ws2_32 mswsock)
    # target_link_options(Game PRIVATE -static-libgcc -static-libstdc++)
endif()

add_dependencies(Game moduleA_c_wasm moduleB_c_wasm moduleC_c_wasm)

add_subdirectory(WasmModules)

include(TargetGame)
include(CopyRuntimeArtifacts)
copy_target_runtime_dependencies(Game)

include(InstallRules)
setup_target_installation_rules(Game)
