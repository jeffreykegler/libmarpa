CMAKE_MINIMUM_REQUIRED(VERSION 3.0.2)

PROJECT(LIBMARPA C)

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

FOREACH(KEYWORD "inline" "__inline__" "__inline")
   IF(NOT DEFINED C_INLINE)
     TRY_COMPILE(C_HAS_${KEYWORD} "${LIBMARPA_BINARY_DIR}"
       "${LIBMARPA_SOURCE_DIR}/test_inline.c"
       COMPILE_DEFINITIONS "-Dinline=${KEYWORD}")
     IF(C_HAS_${KEYWORD})
       SET(C_INLINE TRUE)
       ADD_DEFINITIONS("-Dinline=${KEYWORD}")
     ENDIF(C_HAS_${KEYWORD})
   ENDIF(NOT DEFINED C_INLINE)
ENDFOREACH(KEYWORD)
IF(NOT DEFINED C_INLINE)
   ADD_DEFINITIONS("-Dinline=")
ENDIF(NOT DEFINED C_INLINE)

CONFIGURE_FILE(config.h.cmake config.h)
CONFIGURE_FILE(LIB_VERSION.cmake LIB_VERSION)

SET(libmarpa_src
    marpa_ami.h marpa_codes.c marpa.c marpa_tavl.c marpa.h marpa_slif.h marpa_tavl.h
    marpa_ami.c marpa_obs.c marpa_slif.c marpa_avl.c marpa_avl.h marpa_obs.h)

# Add PIC later

INCLUDE_DIRECTORIES("${CMAKE_BINARY_DIR}")
ADD_LIBRARY(libmarpa STATIC ${libmarpa_src})

# vim: expandtab shiftwidth=4:
