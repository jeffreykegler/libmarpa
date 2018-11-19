# Copyright 2018 Jeffrey Kegler
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

dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)

# Making releases:
#   TEST_MICRO_VERSION += 1;
#   TEST_INTERFACE_AGE += 1;
#   TEST_BINARY_AGE += 1;
# if any functions have been added, set TEST_INTERFACE_AGE to 0.
# if backwards compatibility has been broken,
# set TEST_BINARY_AGE and TEST_INTERFACE_AGE to 0.

m4_define([test_major_version], [2])
m4_define([test_minor_version], [91])
m4_define([test_micro_version], [8])
m4_define([test_interface_age], [0])
m4_define([test_binary_age],
          [m4_eval(100 * test_minor_version + test_micro_version)])
m4_define([test_version],
          [test_major_version.test_minor_version.test_micro_version])
# This is the X.Y used in -lgtk-FOO-X.Y
m4_define([test_api_version], [3.0])

AC_PREREQ([2.62])
AC_INIT([testleak], [test_version])

AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([foreign])

dnl declare --enable-* args and collect ac_help strings
AC_ARG_ENABLE(debug,
              AS_HELP_STRING([--enable-debug=@<:@no/minimum/yes@:>@],
                             [turn on debugging @<:@default=marpa_debug_default@:>@]),,
              enable_debug=marpa_debug_default)


LT_INIT()

# Checks for programs.
AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_LN_S

# Checks for header files.
AC_CHECK_HEADERS([inttypes.h stddef.h stdint.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

if test x"$GCC" = xyes; then
  WARNING_CFLAGS="-Wall \
    -Wextra \
    -Wpointer-arith \
    -Wstrict-prototypes \
    -Wwrite-strings \
    -Wdeclaration-after-statement \
    -Wshadow \
    -Winline \
    -Wmissing-declarations \
    -Wconversion \
    -ansi -pedantic"
fi

AC_SUBST(WARNING_CFLAGS)

PKG_PROG_PKG_CONFIG([0.22])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

