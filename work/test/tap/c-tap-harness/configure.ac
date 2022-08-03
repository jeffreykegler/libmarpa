dnl Autoconf configuration for C TAP Harness
dnl
dnl The C TAP harness is written in maximally portable C, so Autoconf is used
dnl here mostly for consistency in build process and to handle Automake, not
dnl for any portability of the code itself.
dnl
dnl Copyright 2008-2011, 2013-2015, 2017, 2020 Russ Allbery <eagle@eyrie.org>
dnl
dnl SPDX-License-Identifier: MIT

AC_PREREQ([2.64])
AC_INIT([C TAP Harness], [4.8], [eagle@eyrie.org], [c-tap-harness])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([1.11 check-news dist-xz foreign silent-rules subdir-objects
    -Wall -Werror])
AM_MAINTAINER_MODE

dnl Detect unexpanded macros.
m4_pattern_forbid([^PKG_])
m4_pattern_forbid([^_?RRA_])

AC_PROG_CC
RRA_PROG_CC_WARNINGS_FLAGS
AC_SYS_LARGEFILE
AM_PROG_CC_C_O
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])
AC_PROG_INSTALL
AC_PROG_RANLIB
AC_PROG_LN_S

dnl Only used for the test suite.
AC_ARG_VAR([PATH_VALGRIND], [Path to valgrind for the test suite])
AC_PATH_PROG([PATH_VALGRIND], [valgrind])

dnl Add some additional warnings since this package is incredibly strict.
AS_IF([test x"$CLANG" = xyes],
    [WARNINGS_CFLAGS="${WARNINGS_CFLAGS} -pedantic-errors"],
    [WARNINGS_CFLAGS="-ansi -pedantic ${WARNINGS_CFLAGS}"])

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([tests/harness/env/env.t], [chmod +x tests/harness/env/env.t])
AC_CONFIG_FILES([tests/harness/search-t],  [chmod +x tests/harness/search-t])
AC_OUTPUT
