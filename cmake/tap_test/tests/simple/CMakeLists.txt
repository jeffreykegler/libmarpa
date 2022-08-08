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

project(simple C CXX)

include(CMakePrintHelpers)
cmake_print_variables(CMAKE_BUILD_TYPE)
if (CMAKE_BUILD_TYPE MATCHES Asan)
    add_compile_options( -fsanitize=leak -fsanitize=address )
    add_link_options( -fsanitize=leak -fsanitize=address -static-libasan )
ENDIF()

include_directories(${LIBMARPA_INCLUDE} ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/tap_test ${CMAKE_SOURCE_DIR}/tap_test/tests)

add_executable(issue92a issue92a.c marpa_m_test.c)
target_link_libraries(issue92a ${LIBMARPA_STATIC} ${LIBTAP})

add_executable(issue92b issue92b.c marpa_m_test.c)
target_link_libraries(issue92b ${LIBMARPA_STATIC} ${LIBTAP})

add_executable(issue92c issue92c.c marpa_m_test.c)
target_link_libraries(issue92c ${LIBMARPA_STATIC} ${LIBTAP})

add_executable(nits nits.c marpa_m_test.c)
target_link_libraries(nits ${LIBMARPA_STATIC} ${LIBTAP})

add_executable(rule1 rule1.c)
target_link_libraries(rule1 ${LIBMARPA_STATIC} ${LIBTAP})

add_executable(trivial trivial.c)
target_link_libraries(trivial ${LIBMARPA_STATIC} ${LIBTAP})

add_executable(trivial1 trivial1.c marpa_m_test.c)
target_link_libraries(trivial1 ${LIBMARPA_STATIC} ${LIBTAP})

add_executable(depr_unvalued depr_unvalued.c marpa_m_test.c)
target_link_libraries(depr_unvalued ${LIBMARPA_STATIC} ${LIBTAP})

# vim: expandtab shiftwidth=4:
