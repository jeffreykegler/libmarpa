CMAKE_MINIMUM_REQUIRED(VERSION 3.0.2)

PROJECT(tap C CXX)

ADD_EXECUTABLE(runtests runtests.c)

ADD_LIBRARY(${PROJECT_NAME} basic.c float.c)

set(LIBTAP ${PROJECT_BINARY_DIR}/libtap.a
    CACHE INTERNAL "${PROJECT_NAME}: Libtap library" FORCE)

# vim: expandtab shiftwidth=4:
