CMAKE_MINIMUM_REQUIRED(VERSION 3.0.2)

project(simple C CXX)

include_directories(${LIBMARPA_INCLUDE} "${CMAKE_SOURCE_DIR}/tap")

add_executable(rule1 rule1.c)
target_link_libraries(rule1 ${LIBMARPA_STATIC} ${LIBTAP})

add_executable(trivial trivial.c)
target_link_libraries(trivial ${LIBMARPA_STATIC} ${LIBTAP})

add_executable(trivial1 trivial1.c marpa_m_test.c)
target_link_libraries(trivial1 ${LIBMARPA_STATIC} ${LIBTAP})

add_executable(nits nits.c marpa_m_test.c)
target_link_libraries(nits ${LIBMARPA_STATIC} ${LIBTAP})

add_test(rule1 rule1)
add_test(trivial trivial)
add_test(trivial1 trivial1)
add_test(nits nits)

# vim: expandtab shiftwidth=4:
