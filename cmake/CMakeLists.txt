# Copyright 2018 Jeffrey Kegler
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

CMAKE_MINIMUM_REQUIRED(VERSION 3.0.2)

PROJECT(libmarpa C)

# ------------------
# Module search path
# ------------------
SET (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/modules")
ENABLE_TESTING()

if("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
   message(SEND_ERROR "In-source builds are not allowed.")
   endif("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")

# Version bumping utility relies on the exact
# format of those next 3 lines
SET(MARPA_LIB_MAJOR_VERSION 8)
SET(MARPA_LIB_MINOR_VERSION 6)
SET(MARPA_LIB_PATCH_VERSION 2)

SET(MARPA_LIB_VERSION
    ${MARPA_LIB_MAJOR_VERSION}.${MARPA_LIB_MINOR_VERSION}.${MARPA_LIB_PATCH_VERSION})

# ----------------------------------------------------------------------------
# Enable all warnings depending on compiler
# C.f. http://stackoverflow.com/questions/14231292/cmake-and-compiler-warnings
# ----------------------------------------------------------------------------
IF (CMAKE_COMPILER_IS_GNUCC)
    # MARPA_DEBUG_FLAG ?
    # -Wextra ?
    # -Wdeclaration-after-statement ?
    ADD_DEFINITIONS(-g -O2 -Wall -Wpointer-arith -Wstrict-prototypes -Wwrite-strings -Wshadow -Wmissing-declarations -Wconversion -ansi -pedantic)
    ADD_DEFINITIONS(-Wundef -Wendif-labels)
ENDIF (CMAKE_COMPILER_IS_GNUCC)
IF (MSVC)
    ADD_DEFINITIONS(/W3)
ENDIF (MSVC)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions( -DMARPA_DEBUG=1 )
ENDIF()

# --------
# config.h
# --------
FIND_PACKAGE(Inline)
FIND_PACKAGE(NullIsZeroes)
INCLUDE(CheckIncludeFile)
CHECK_INCLUDE_FILE("stdint.h" HAVE_STDINT_H)
CHECK_INCLUDE_FILE("inttypes.h" HAVE_INTTYPES_H)
ADD_DEFINITIONS(-DHAVE_CONFIG_H=1)
CONFIGURE_FILE(config.h.cmake config.h)

CONFIGURE_FILE(LIB_VERSION.cmake LIB_VERSION)

SET(libmarpa_src
    marpa.c marpa_tavl.c
    marpa_ami.c marpa_obs.c marpa_avl.c )

# Expose B's public includes (including Boost transitively) to other
# subprojects through cache variable.
set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

set(${PROJECT_NAME}_ERROR_CODES ${PROJECT_SOURCE_DIR}/error_codes.table
    CACHE INTERNAL "${PROJECT_NAME}: Error codes table" FORCE)

include_directories(
    "${PROJECT_BINARY_DIR}" \  # for config.h
    "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/include")

ADD_LIBRARY(marpa SHARED ${libmarpa_src})
ADD_LIBRARY(marpa_s STATIC ${libmarpa_src})

set_target_properties(marpa_s
                      PROPERTIES POSITION_INDEPENDENT_CODE ON)

install(TARGETS marpa marpa_s
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(FILES "${PROJECT_SOURCE_DIR}/include/marpa.h" DESTINATION inc)

# vim: expandtab shiftwidth=4:
