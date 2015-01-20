CMAKE_MINIMUM_REQUIRED(VERSION 3.0.2)

project(rule1 C CXX)

include_directories(${LIBMARPA_INCLUDE} "${CMAKE_SOURCE_DIR}/tap")

add_executable(${PROJECT_NAME} rule1.c)

target_link_libraries(${PROJECT_NAME} ${LIBMARPA_STATIC} ${LIBTAP})

add_test(rule1 rule1)

# vim: expandtab shiftwidth=4:
