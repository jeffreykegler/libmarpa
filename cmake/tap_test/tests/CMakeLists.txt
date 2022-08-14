# Copyright 2022 Jeffrey Kegler
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
# OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
# ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.

# You can tweak some common (for all subprojects) stuff here. For example:

SET (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)

SET(LIBMARPA_SHARED "${CMAKE_BINARY_DIR}/libmarpa.so")
SET(LIBMARPA_STATIC "${CMAKE_BINARY_DIR}/libmarpa_s.a")
SET(LIBMARPA_INCLUDE "${CMAKE_BINARY_DIR}")

# if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
#  message(SEND_ERROR "${CMAKE_SOURCE_DIR} vs. ${CMAKE_BINARY_DIR}: In-source builds are not allowed.")
# endif ()

# set(CMAKE_VERBOSE_MAKEFILE ON)
# set(CMAKE_COLOR_MAKEFILE   ON)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

# Remove 'lib' prefix for shared libraries on Windows
if (WIN32)
  set(CMAKE_SHARED_LIBRARY_PREFIX "")
endif ()

ADD_EXECUTABLE(runtests runtests.c)
IF (CMAKE_COMPILER_IS_GNUCC)
    target_compile_options(runtests PUBLIC ${LIBMARPA_COMPILE_OPTIONS})
    target_compile_options(runtests PUBLIC "-Wno-sign-conversion")
ENDIF (CMAKE_COMPILER_IS_GNUCC)

# NOTE: The order matters! The most independent ones should go first.
add_subdirectory(tap)
add_subdirectory(simple)

# vim: expandtab shiftwidth=4:
