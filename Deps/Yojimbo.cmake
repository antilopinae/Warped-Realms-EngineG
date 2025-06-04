set(YOJIMBO_DIR Extern/extern-yojimbo)

file(GLOB_RECURSE YOJIMBO_SRC_FILES
${YOJIMBO_DIR}/source/.cpp
${YOJIMBO_DIR}/sodium/.c
${YOJIMBO_DIR}/netcode/netcode.c
${YOJIMBO_DIR}/reliable/reliable.c
${YOJIMBO_DIR}/tlsf/tlsf.c
CMAKE_CONFIGURE_DEPENDS)

add_library(yojimbo STATIC ${YOJIMBO_SRC_FILES})

target_include_directories(yojimbo PUBLIC "${YOJIMBO_DIR}/include/")
target_include_directories(yojimbo PUBLIC "${YOJIMBO_DIR}/sodium/")
target_include_directories(yojimbo PUBLIC "${YOJIMBO_DIR}/tlsf/")
target_include_directories(yojimbo PUBLIC "${YOJIMBO_DIR}/netcode/")
target_include_directories(yojimbo PUBLIC "${YOJIMBO_DIR}/reliable/")
target_include_directories(yojimbo PUBLIC "${YOJIMBO_DIR}/serialize/")

target_compile_definitions (yojimbo PUBLIC
SERIALIZE_ENABLE_TESTS
RELIABLE_ENABLE_TESTS
NETCODE_ENABLE_TESTS
)
