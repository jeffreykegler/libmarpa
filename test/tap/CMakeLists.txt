CMAKE_MINIMUM_REQUIRED(VERSION 3.0.2)

PROJECT(libtap C CXX)

ADD_EXECUTABLE(runtests runtests.c)

ADD_LIBRARY(${PROJECT_NAME} basic.c float.c)

# vim: expandtab shiftwidth=4:
