cmake_minimum_required(VERSION 3.26.0)

find_program(EMCC emcc)
if(NOT EMCC)
    message(FATAL_ERROR "emcc not found!")
endif()

set(WASM_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/Assets")

set(EMCC_FLAGS
        -Os
        -s WASM=1
        -s ALLOW_MEMORY_GROWTH=1
        -s NO_FILESYSTEM=1
        "-sEXPORTED_FUNCTIONS=[\"_initialize\",\"_update\",\"_terminate\",\"_allocate\",\"_deallocate\"]"
#        "-sEXPORTED_RUNTIME_METHODS=[]"
        --no-entry
)

function(add_wasm_module MODULE_NAME SOURCE_FILE)
    set(OUTPUT_WASM "${WASM_OUTPUT_DIR}/${MODULE_NAME}.wasm")
    add_custom_command(
            OUTPUT ${OUTPUT_WASM}
            COMMAND ${EMCC} ${SOURCE_FILE} -o ${OUTPUT_WASM} ${EMCC_FLAGS}
            DEPENDS ${SOURCE_FILE}
            COMMENT "Compiling ${SOURCE_FILE} to ${OUTPUT_WASM}"
            VERBATIM
    )
    add_custom_target(${MODULE_NAME}_wasm ALL DEPENDS ${OUTPUT_WASM})
endfunction()

# wasms
add_wasm_module(moduleA_c "moduleA_c.c")
add_wasm_module(moduleB_c "moduleB_c.c")
add_wasm_module(moduleC_c "moduleC_c.c")
