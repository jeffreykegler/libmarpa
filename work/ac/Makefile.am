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

## Makefile.am -- Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS = subdir-objects
AM_CFLAGS = $(WARNING_CFLAGS) $(MARPA_DEBUG_FLAG)
AM_CPPFLAGS = $(WARNING_CPPFLAGS)
RELEASE= $(MARPA_LIB_MAJOR_VERSION).$(MARPA_LIB_MINOR_VERSION).$(MARPA_LIB_MICRO_VERSION)
ACLOCAL_AMFLAGS = -I m4
lib_LTLIBRARIES = libmarpa.la
libmarpa_la_SOURCES = marpa.c marpa.h \
    marpa_obs.c marpa_obs.h \
    marpa_avl.c marpa_avl.h \
    marpa_tavl.c marpa_tavl.h \
    marpa_ami.c marpa_ami.h \
    marpa_codes.c

# After the suggestion in Russ Allbery's "Using the Harness"
# in the page https://www.eyrie.org/~eagle/software/c-tap-harness/readme.html
#
check_PROGRAMS = test/tap/runtests
test_tap_runtests_CPPFLAGS = \
    -DC_TAP_SOURCE='"$(abs_top_srcdir)/test/tap"' \
    -DC_TAP_BUILD='"$(abs_top_builddir)/test/tap"'
check_LIBRARIES = test/tap/libtap/libtap.a
test_tap_libtap_libtap_a_CPPFLAGS = -I$(abs_top_srcdir)/test/tap
test_tap_libtap_libtap_a_SOURCES = test/tap/libtap/basic.c test/tap/libtap/basic.h \
	test/tap/libtap/float.c test/tap/libtap/float.h test/tap/libtap/macros.h

EXTRA_DIST = README.INSTALL README.AIX libmarpa.pc GIT_LOG.txt \
  win32/marpa.def Makefile.win32 win32/do_config_h.pl \
  internals/libmarpa_ami.pdf internals/libmarpa_core.pdf \
  api_docs/libmarpa_api.pdf api_docs/libmarpa_api.html api_docs/api_html.tar \
  api_docs/libmarpa_api.info \
  error_codes.table events.table steps.table \
  version.m4 libmarpa_version.sh LIB_VERSION

libmarpa_la_LIBADD = $(LIBOBJS)
libmarpa_la_LDFLAGS = -release $(RELEASE)
include_HEADERS = marpa.h \
  marpa_avl.h marpa_tavl.h marpa_ami.h \
  marpa_obs.h marpa_codes.h
TEXI2PDF = texi2pdf --pdf --batch --quiet

dist-hook:
	chmod u+w $(distdir)/GIT_LOG.txt
	git log -n 5 >> $(distdir)/GIT_LOG.txt
