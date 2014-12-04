CMAKE_MINIMUM_REQUIRED(VERSION 3.0.2)

PROJECT(LIBMARPA C)

# ------------------
# Module search path
# ------------------
SET (CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/modules")
ENABLE_TESTING()

if("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
   message(SEND_ERROR "In-source builds are not allowed.")
   endif("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")

SET(MARPA_LIB_MAJOR 7)
SET(MARPA_LIB_MINOR 3)
SET(MARPA_LIB_PATCH 0)
SET(MARPA_LIB_VERSION ${MARPA_LIB_MAJOR}.${MARPA_LIB_MINOR}.${MARPA_LIB_PATCH})

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
CHECK_INCLUDE_FILE("sys/types.h" HAVE_SYS_TYPES_H)
CHECK_INCLUDE_FILE("unistd.h" HAVE_UNISTD_H)
CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/include/marpaWrapper_config.h.in" "${CMAKE_CURRENT_BINARY_DIR}/include/marpaWrapper_config.h")
CONFIGURE_FILE(config.h.cmake config.h)

CONFIGURE_FILE(LIB_VERSION.cmake LIB_VERSION)

SET(libmarpa_src
    marpa_ami.h marpa_codes.c marpa.c marpa_tavl.c marpa.h marpa_slif.h marpa_tavl.h
    marpa_ami.c marpa_obs.c marpa_slif.c marpa_avl.c marpa_avl.h marpa_obs.h)

# Add PIC later

INCLUDE_DIRECTORIES("${CMAKE_BINARY_DIR}")
ADD_LIBRARY(libmarpa STATIC ${libmarpa_src})

# vim: expandtab shiftwidth=4:
