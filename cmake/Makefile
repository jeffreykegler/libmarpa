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

# This directory builds the configuration files for the CMake version of
# the libmarpa distribution.  It is *NOT* either a cmake build directory,
# or a CMake distribution directory.  Instead it contains or (as necessary)
# builds the components needed by the cmake distribution directory.

# IMPORTANT NOTES: CMakeLists.txt is a *target* in this directory, not a
# source file.

HASH_PREAMBLE = ../shared/license.hash ../shared/do_not_edit.hash
CONFIGURE_AC_PIECES = $(HASH_PREAMBLE) \
  CMakeLists.txt.p20 CMakeLists.txt.p80

all: CMakeLists.txt

CMakeLists.txt: $(CONFIGURE_AC_PIECES)
	cat ../shared/license.hash > $@
	cat ../shared/do_not_edit.hash > $@
	cat CMakeLists.txt.p20 >> $@
	perl -e \
	'printf "SET(MARPA_LIB_MAJOR_VERSION %d)\n", shift; \
	 printf "SET(MARPA_LIB_MINOR_VERSION %d)\n", shift; \
	 printf "SET(MARPA_LIB_MICRO_VERSION %d)\n", shift;' \
	    `sh ../libmarpa_version.sh  major` \
	    `sh ../libmarpa_version.sh  minor` \
	    `sh ../libmarpa_version.sh  micro` >> $@
	cat CMakeLists.txt.p80 >> $@

CLEAN_FILES = CMakeLists.txt

clean:
	rm -f $(CLEAN_FILES)
