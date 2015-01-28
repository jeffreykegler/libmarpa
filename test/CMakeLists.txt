CMAKE_MINIMUM_REQUIRED(VERSION 3.0.2)

enable_testing()

# You can tweak some common (for all subprojects) stuff here. For example:

SET (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)

SET(LIBMARPA_SHARED "${CMAKE_SOURCE_DIR}/usr/local/lib/libmarpa.so")
SET(LIBMARPA_STATIC "${CMAKE_SOURCE_DIR}/usr/local/lib/libmarpa_s.a")
SET(LIBMARPA_INCLUDE "${CMAKE_SOURCE_DIR}/usr/local/inc/")

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(SEND_ERROR "${CMAKE_SOURCE_DIR} vs. ${CMAKE_BINARY_DIR}: In-source builds are not allowed.")
endif ()

# set(CMAKE_VERBOSE_MAKEFILE ON)
# set(CMAKE_COLOR_MAKEFILE   ON)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

# Remove 'lib' prefix for shared libraries on Windows
if (WIN32)
  set(CMAKE_SHARED_LIBRARY_PREFIX "")
endif ()

# NOTE: The order matters! The most independent ones should go first.
add_subdirectory(tap)
add_subdirectory(simple)

# vim: expandtab shiftwidth=4:
