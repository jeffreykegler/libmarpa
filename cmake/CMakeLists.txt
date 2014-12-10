# Copyright 2014 Jeffrey Kegler
# This file is part of Libmarpa.  Libmarpa is free software: you can
# redistribute it and/or modify it under the terms of the GNU Lesser
# General Public License as published by the Free Software Foundation,
# either version 3 of the License, or (at your option) any later version.
#
# Libmarpa is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser
# General Public License along with Libmarpa.  If not, see
# http://www.gnu.org/licenses/.

CMAKE_MINIMUM_REQUIRED(VERSION 3.0.2)

PROJECT(libmarpa C)

# ------------------
# Module search path
# ------------------
SET (CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/modules")
ENABLE_TESTING()

if("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
   message(SEND_ERROR "In-source builds are not allowed.")
   endif("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")

# Version bumping utility relies on the exact
# format of those next 3 lines
SET(MARPA_LIB_MAJOR_VERSION 7)
SET(MARPA_LIB_MINOR_VERSION 4)
SET(MARPA_LIB_PATCH_VERSION 0)

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

# Add PIC later
# Required on Unix OS family to be able to be linked into shared libraries.
# set_target_properties(${PROJECT_NAME}
#                      PROPERTIES POSITION_INDEPENDENT_CODE ON)

# Expose B's public includes (including Boost transitively) to other
# subprojects through cache variable.
set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

include_directories(
    "${PROJECT_BINARY_DIR}" \  # for config.h
    "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/include")
ADD_LIBRARY(marpa STATIC ${libmarpa_src})

# vim: expandtab shiftwidth=4:
