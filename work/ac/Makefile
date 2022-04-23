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

.PHONY: all

# This directory builds the configuration files for libmarpa.

# IMPORTANT NOTE:  configure.ac is a *target* in this directory,
#not a source file.

CONFIGURE_AC_PIECES = configure.ac.p20 configure.ac-version configure.ac.p80

all: configure.ac

configure.ac: $(CONFIGURE_AC_PIECES)
	cat $(CONFIGURE_AC_PIECES) > $@

configure.ac-version: ../../Makefile
	perl -e \
	'printf "m4_define([marpa_major_version], [%d])\n", shift; \
	 printf "m4_define([marpa_minor_version], [%d])\n", shift; \
	 printf "m4_define([marpa_micro_version], [%d])\n", shift;' \
	    `sh ../../libmarpa_version.sh  major` \
	    `sh ../../libmarpa_version.sh  minor` \
	    `sh ../../libmarpa_version.sh  micro` > $@

CLEAN_FILES = configure.ac configure.ac-version

clean:
	rm -f $(CLEAN_FILES)
