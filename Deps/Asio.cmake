cmake_minimum_required(VERSION 3.26.0)

# --- Dependencies ---

# Asio
if(CUSTOM_DEP_HANDLER_INCLUDED)
    find_or_clone_dependency(asio
        "https://github.com/chriskohlhoff/asio.git"
        "asio-1-31-0"
    )
else()
    message(FATAL_ERROR "Asio target not created.")
endif()

FetchContent_GetProperties(asio)

if(NOT asio_POPULATED)
  message(STATUS "Fetching asio ${ASIO_CMAKE_ASIO_TAG}")

  if(EXISTS "${ASIO_CMAKE_ASIO_DEP_DIR}/asio")
    set(FETCHCONTENT_SOURCE_DIR_ASIO ${ASIO_CMAKE_ASIO_DEP_DIR})
    message(STATUS "asio ${ASIO_CMAKE_ASIO_TAG} source found at ${ASIO_CMAKE_ASIO_DEP_DIR}; skip fetching.")
  endif()

  FetchContent_MakeAvailable(asio)
  message(STATUS "Populating asio ${ASIO_CMAKE_ASIO_TAG} done; src at ${asio_SOURCE_DIR}")
endif()

add_library(asio INTERFACE)

target_include_directories(asio
        INTERFACE ${asio_SOURCE_DIR}/asio/include
)

target_compile_definitions(asio
        INTERFACE
        ASIO_STANDALONE
        ASIO_NO_DEPRECATED
)

if(TARGET_LINUX)
  target_link_libraries(asio
          INTERFACE pthread
  )
elseif(TARGET_WINDOWS)
  # macro see @ https://stackoverflow.com/a/40217291/1746503
  macro(get_WIN32_WINNT version)
    if (CMAKE_SYSTEM_VERSION)
      set(ver ${CMAKE_SYSTEM_VERSION})
      string(REGEX MATCH "^([0-9]+).([0-9])" ver ${ver})
      string(REGEX MATCH "^([0-9]+)" verMajor ${ver})
      # Check for Windows 10, b/c we'll need to convert to hex 'A'.
      if ("${verMajor}" MATCHES "10")
        set(verMajor "A")
        string(REGEX REPLACE "^([0-9]+)" ${verMajor} ver ${ver})
      endif ("${verMajor}" MATCHES "10")
      # Remove all remaining '.' characters.
      string(REPLACE "." "" ver ${ver})
      # Prepend each digit with a zero.
      string(REGEX REPLACE "([0-9A-Z])" "0\\1" ver ${ver})
      set(${version} "0x${ver}")
    endif(CMAKE_SYSTEM_VERSION)
  endmacro(get_WIN32_WINNT)

  if(NOT DEFINED _WIN32_WINNT)
    get_WIN32_WINNT(ver)
    set(_WIN32_WINNT ${ver})
  endif()

  message(STATUS "Set _WIN32_WINNET=${_WIN32_WINNT}")

  target_compile_definitions(asio
          INTERFACE
          _WIN32_WINNT=${_WIN32_WINNT}
          WIN32_LEAN_AND_MEAN
  )
endif()
