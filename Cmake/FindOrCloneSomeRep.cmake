cmake_minimum_required(VERSION 3.20.0)

include(FetchContent)
include(CMakePrintHelpers)

# Find target, if it's not available, clones git repository.
function(FindOrCloneRep target repository tag NEED_TO_FIND)
if( ${NEED_TO_FIND})
  find_package(${target} QUIET)
endif()

if (NOT ${target}_FOUND)
  message("${target} not found, fetching git repository")

  set(source_dir ${CMAKE_CURRENT_LIST_DIR}/Extern/extern-${target})
  set(bin_dir ${CMAKE_BINARY_DIR}/Extern/extern-${target}-build)

  string(TOUPPER ${target} target_UPPER)

  set(FETCHCONTENT_UPDATES_DISCONNECTED_${target_UPPER} TRUE)

  FetchContent_Declare(${target}
    GIT_REPOSITORY ${repository}
    GIT_TAG ${tag}
    OVERRIDE_FIND_PACKAGE
    SOURCE_DIR ${source_dir}
    BINARY_DIR ${bin_dir}
    CMAKE_ARGS
      -D${target_UPPER}_ENABLE_INSTALL=OFF
      -D${target_UPPER}_BUILD_EXAMPLES=OFF
      -D${target_UPPER}_BUILD_DOCS=OFF
  )

  if(EXISTS ${source_dir})
    set(FETCHCONTENT_SOURCE_DIR_${target_UPPER} ${source_dir})
    message("${target} downloaded")
  endif()

  option(${target_UPPER}_ENABLE_INSTALL "Enable install" OFF)
  set(${target_UPPER}_ENABLE_INSTALL OFF CACHE BOOL "Enable install" FORCE)

  option(${target_UPPER}_BUILD_EXAMPLES "Build examples" OFF)
  set(${target_UPPER}_BUILD_EXAMPLES OFF CACHE BOOL "Build examples" FORCE)

  option(${target_UPPER}_BUILD_DOCS "Build docs" OFF)
  set(${target_UPPER}_BUILD_DOCS OFF CACHE BOOL "Build docs" FORCE)


  FetchContent_MakeAvailable(${target})

else()
  message("${target} found!")

endif()

  cmake_print_properties(TARGETS ${target}
                       PROPERTIES TYPE SOURCE_DIR)

  message("ENABLE_INSTALL=${${target_UPPER}_ENABLE_INSTALL}")
  message("BUILD_EXAMPLES=${${target_UPPER}_BUILD_EXAMPLES}")
  message("BUILD_DOCS=${${target_UPPER}_BUILD_DOCS}")

endfunction()
