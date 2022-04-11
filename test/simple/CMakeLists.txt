CMAKE_MINIMUM_REQUIRED(VERSION 3.0.2)

project(simple C CXX)

include_directories(${LIBMARPA_INCLUDE} "${CMAKE_SOURCE_DIR}/tap")

IF (CMAKE_COMPILER_IS_GNUCC)
    set_source_files_properties(issue92a.c PROPERTIES COMPILE_OPTIONS
        "-g;-O2;-Wall;-Wpointer-arith;-Wstrict-prototypes;-Wwrite-strings;-Wshadow;-Wmissing-declarations;\
-Wconversion;-ansi;-pedantic;-Wundef;-Wendif-labels")
    # MARPA_DEBUG_FLAG ?
    # -Wextra ?
    # -Wdeclaration-after-statement ?
ENDIF (CMAKE_COMPILER_IS_GNUCC)
add_executable(issue92a issue92a.c marpa_m_test.c)
target_link_libraries(issue92a ${LIBMARPA_STATIC} ${LIBTAP})

IF (CMAKE_COMPILER_IS_GNUCC)
    set_source_files_properties(issue92b.c PROPERTIES COMPILE_OPTIONS
        "-g;-O2;-Wall;-Wpointer-arith;-Wstrict-prototypes;-Wwrite-strings;-Wshadow;-Wmissing-declarations;\
-Wconversion;-ansi;-pedantic;-Wundef;-Wendif-labels")
ENDIF (CMAKE_COMPILER_IS_GNUCC)
add_executable(issue92b issue92b.c marpa_m_test.c)
target_link_libraries(issue92b ${LIBMARPA_STATIC} ${LIBTAP})

IF (CMAKE_COMPILER_IS_GNUCC)
    set_source_files_properties(issue92c.c PROPERTIES COMPILE_OPTIONS
        "-g;-O2;-Wall;-Wpointer-arith;-Wstrict-prototypes;-Wwrite-strings;-Wshadow;-Wmissing-declarations;\
-Wconversion;-ansi;-pedantic;-Wundef;-Wendif-labels")
ENDIF (CMAKE_COMPILER_IS_GNUCC)
add_executable(issue92c issue92c.c marpa_m_test.c)
target_link_libraries(issue92c ${LIBMARPA_STATIC} ${LIBTAP})

add_executable(nits nits.c marpa_m_test.c)
target_link_libraries(nits ${LIBMARPA_STATIC} ${LIBTAP})

add_executable(rule1 rule1.c)
target_link_libraries(rule1 ${LIBMARPA_STATIC} ${LIBTAP})

add_executable(trivial trivial.c)
target_link_libraries(trivial ${LIBMARPA_STATIC} ${LIBTAP})

IF (CMAKE_COMPILER_IS_GNUCC)
    set_source_files_properties(trivial1.c PROPERTIES COMPILE_OPTIONS
        "-g;-O2;-Wall;-Wpointer-arith;-Wstrict-prototypes;-Wwrite-strings;-Wshadow;-Wmissing-declarations;\
-Wconversion;-ansi;-pedantic;-Wundef;-Wendif-labels")
ENDIF (CMAKE_COMPILER_IS_GNUCC)
add_executable(trivial1 trivial1.c marpa_m_test.c)
target_link_libraries(trivial1 ${LIBMARPA_STATIC} ${LIBTAP})

add_test(issue92a issue92a)
add_test(issue92b issue92b)
add_test(issue92c issue92c)
add_test(nits nits)
add_test(rule1 rule1)
add_test(trivial trivial)
add_test(trivial1 trivial1)

# vim: expandtab shiftwidth=4:
