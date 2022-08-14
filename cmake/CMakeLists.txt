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

CMAKE_MINIMUM_REQUIRED(VERSION 3.16.3)

include(version.cmake)

SET(MARPA_LIB_VERSION
    ${MARPA_LIB_MAJOR_VERSION}.${MARPA_LIB_MINOR_VERSION}.${MARPA_LIB_MICRO_VERSION})

PROJECT(libmarpa
  LANGUAGES C
  HOMEPAGE_URL https://jeffreykegler.github.io/Marpa-web-site/libmarpa.html
  DESCRIPTION "C language library that implements the parse engine at the core of Marpa"
  VERSION ${MARPA_LIB_VERSION})

include(CTest)

# ------------------
# Module search path
# ------------------
SET (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/modules")

if("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
   message(SEND_ERROR "In-source builds are not allowed.")
   endif("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")

# ----------------------------------------------------------------------------
# Enable all warnings depending on compiler
# C.f. http://stackoverflow.com/questions/14231292/cmake-and-compiler-warnings
# ----------------------------------------------------------------------------
IF (CMAKE_COMPILER_IS_GNUCC)
    # MARPA_DEBUG_FLAG ?
    # -Wextra ?
    # -Wdeclaration-after-statement ?
    add_compile_options(-g -O2)
    set(LIBMARPA_COMPILE_OPTIONS
        -Wall -Wpointer-arith -Wstrict-prototypes -Wwrite-strings
        -Wshadow -Wmissing-declarations -Wconversion -ansi -pedantic
        -Wundef -Wendif-labels)
ENDIF (CMAKE_COMPILER_IS_GNUCC)
IF (MSVC)
    set(LIBMARPA_COMPILE_OPTIONS /W3)
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

SET(libmarpa_src
    marpa.c marpa_tavl.c
    marpa_ami.c marpa_obs.c marpa_avl.c )

include_directories(
    "${PROJECT_BINARY_DIR}" \  # for config.h
    "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/include")

ADD_LIBRARY(marpa SHARED ${libmarpa_src})
target_compile_options(marpa PUBLIC ${LIBMARPA_COMPILE_OPTIONS})
ADD_LIBRARY(marpa_s STATIC ${libmarpa_src})
target_compile_options(marpa_s PUBLIC ${LIBMARPA_COMPILE_OPTIONS})

# set_target_properties(marpa_s PROPERTIES POSITION_INDEPENDENT_CODE ON)

install(TARGETS marpa marpa_s
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(FILES "${PROJECT_SOURCE_DIR}/include/marpa.h" DESTINATION include)

configure_file(libmarpa.pc.in ${PROJECT_BINARY_DIR}/libmarpa.pc @ONLY)
install(FILES "${PROJECT_BINARY_DIR}/libmarpa.pc" DESTINATION "lib/pkgconfig/")

# Remove temp directory after copy?
configure_file(tap_test/do_tap_tests.sh.in temp/do_tap_tests.sh @ONLY)
file(COPY ${PROJECT_BINARY_DIR}/temp/do_tap_tests.sh
    DESTINATION ${PROJECT_BINARY_DIR}/tap_test/
    FILE_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ)

add_subdirectory(tap_test/tests)
add_test(tap_test tap_test/do_tap_tests.sh)

add_subdirectory(marpalua)

# vim: expandtab shiftwidth=4:
