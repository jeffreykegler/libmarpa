CMAKE_MINIMUM_REQUIRED(VERSION 3.0.2)

PROJECT(libmarpa C)

if("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
   message(SEND_ERROR "In-source builds are not allowed.")
   endif("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")

SET(MARPA_LIB_MAJOR 7)
SET(MARPA_LIB_MINOR 3)
SET(MARPA_LIB_PATCH 0)
SET(MARPA_LIB_VERSION ${MARPA_LIB_MAJOR}.${MARPA_LIB_MINOR}.${MARPA_LIB_PATCH})

CONFIGURE_FILE(config.h.cmake config.h)

SET(libmarpa_src roof.c
    marpa_ami.h marpa_codes.c marpa.c marpa_tavl.c marpa.h marpa_slif.h marpa_tavl.h
    marpa_ami.c marpa_obs.c marpa_slif.c marpa_avl.c marpa_avl.h marpa_obs.h)

# MARPA_DEBUG_FLAG ?
# -Wextra ?
# -Wdeclaration-after-statement ?
ADD_DEFINITIONS(-g -O2 -Wall -Wpointer-arith -Wstrict-prototypes -Wwrite-strings -Wshadow -Wmissing-declarations -Wconversion -ansi -pedantic)
ADD_DEFINITIONS(-Wundef -Wendif-labels)

# LIB_VERSION ?

# Add PIC later

ADD_LIBRARY(libmarpa STATIC ${libmarpa_src})

# vim: expandtab shiftwidth=4:
