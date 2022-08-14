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

include(CMakePrintHelpers)
cmake_print_variables(CMAKE_BUILD_TYPE)
if (CMAKE_BUILD_TYPE MATCHES Asan)
    add_compile_options( -fsanitize=leak -fsanitize=address )
    add_link_options( -fsanitize=leak -fsanitize=address -static-libasan )
ENDIF()

include_directories(${LIBMARPA_INCLUDE} ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/tap_test ${CMAKE_SOURCE_DIR}/tap_test/tests)

add_executable(issue92a issue92a.c marpa_m_test.c)
target_link_libraries(issue92a ${LIBMARPA_STATIC} libtap)
target_compile_options(issue92a PUBLIC ${LIBMARPA_COMPILE_OPTIONS})

add_executable(issue92b issue92b.c marpa_m_test.c)
target_link_libraries(issue92b ${LIBMARPA_STATIC} libtap)
target_compile_options(issue92b PUBLIC ${LIBMARPA_COMPILE_OPTIONS})

add_executable(issue92c issue92c.c marpa_m_test.c)
target_link_libraries(issue92c ${LIBMARPA_STATIC} libtap)
target_compile_options(issue92c PUBLIC ${LIBMARPA_COMPILE_OPTIONS})

add_executable(nits nits.c marpa_m_test.c)
target_link_libraries(nits ${LIBMARPA_STATIC} libtap)
target_compile_options(nits PUBLIC ${LIBMARPA_COMPILE_OPTIONS})

add_executable(rule1 rule1.c)
target_link_libraries(rule1 ${LIBMARPA_STATIC} libtap)
target_compile_options(rule1 PUBLIC ${LIBMARPA_COMPILE_OPTIONS})

add_executable(trivial trivial.c)
target_link_libraries(trivial ${LIBMARPA_STATIC} libtap)
target_compile_options(trivial PUBLIC ${LIBMARPA_COMPILE_OPTIONS})

add_executable(trivial1 trivial1.c marpa_m_test.c)
target_link_libraries(trivial1 ${LIBMARPA_STATIC} libtap)
target_compile_options(trivial1 PUBLIC ${LIBMARPA_COMPILE_OPTIONS})

add_executable(depr_unvalued depr_unvalued.c marpa_m_test.c)
target_link_libraries(depr_unvalued ${LIBMARPA_STATIC} libtap)
target_compile_options(depr_unvalued PUBLIC ${LIBMARPA_COMPILE_OPTIONS})

# vim: expandtab shiftwidth=4:
