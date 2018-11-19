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

AC_PREREQ([2.68])

dnl ***********************************
dnl *** include special marpa macros ***
dnl ***********************************

m4_define(marpa_configure_in)

m4_define([marpa_major_version], [8])
m4_define([marpa_minor_version], [6])
m4_define([marpa_micro_version], [1])
m4_define([marpa_version],
          [marpa_major_version.marpa_minor_version.marpa_micro_version])

AC_INIT([libmarpa-doc],[marpa_version],[http://rt.cpan.org/NoAuth/Bugs.html?Dist=Marpa])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([])

AC_CONFIG_FILES( [Makefile])
AC_OUTPUT

# vim: expandtab shiftwidth=4:
