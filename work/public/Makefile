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

# This directory does the final build of the public headers
# Because this process collects the data from the various #define's
# It also build marpa_codes.c, which contains data for reverse
# lookup of events, errors, etc., by number.

CPPWRAP = perl ../bin/cppwrap.pl
C_LANGUAGE_PREAMBLE = ../shared/license.c ../shared/do_not_edit.c
MARPA_H_PIECES = $(C_LANGUAGE_PREAMBLE) \
  marpa.h.p10 marpa.h-version marpa.h-err \
  marpa.h-event marpa.h-step \
  marpa.h.p50 marpa.h.p80 marpa.h.p90

all: marpa.h marpa_codes.c \
    error_codes.table events.table steps.table

marpa.h.p50: ../dev/marpa.h.p50
	cp $? $@

marpa.h.p80: ../dev/marpa.h.p80
	$(CPPWRAP) $@ < $? > $@

marpa.h: $(MARPA_H_PIECES)
	cat $(MARPA_H_PIECES) > marpa.h

CPPWRAP = perl ../bin/cppwrap.pl

# The pieces with the code data,
# not yet wrapped with the pre-processor line directive
MARPA_CODES_C_UNWRAPPED = marpa_codes.c.p10 marpa.c-err marpa.c-event marpa.c-step

MARPA_CODES_C_PIECES = $(C_LANGUAGE_PREAMBLE) $(MARPA_CODES_C_UNWRAPPED)

marpa_codes.c: $(MARPA_CODES_C_PIECES)
	(cat $(C_LANGUAGE_PREAMBLE) ; \
		for f in $(MARPA_CODES_C_UNWRAPPED); \
		do $(CPPWRAP) $$f < $$f; \
		done) > $@

API_TEXI_FILES = ../dev/api.texi
error_codes.table marpa.c-err marpa.h-err: $(API_TEXI_FILES)
	cat $(API_TEXI_FILES) | \
	  perl texi2err.pl marpa.c-err error_codes.table.p50 | \
	  $(CPPWRAP) marpa.h-err > marpa.h-err
	cat ../shared/license.hash \
	  ../shared/do_not_edit.hash \
	  error_codes.table.p50 > error_codes.table

events.table marpa.c-event marpa.h-event: ../dev/api.texi
	cat $(API_TEXI_FILES) | \
	  perl texi2event.pl marpa.c-event events.table.p50 | \
	  $(CPPWRAP) marpa.h-event > marpa.h-event
	cat ../shared/license.hash \
	  ../shared/do_not_edit.hash \
	  events.table.p50 > events.table

steps.table marpa.c-step marpa.h-step: ../dev/api.texi
	cat $(API_TEXI_FILES) | \
	  perl texi2step.pl marpa.c-step steps.table.p50 |\
	  $(CPPWRAP) marpa.h-step > marpa.h-step
	cat ../shared/license.hash \
	  ../shared/do_not_edit.hash \
	  steps.table.p50 > steps.table

CLEAN_FILES = marpa.c-err marpa.c-event marpa.c-step \
  marpa.h marpa.h-err marpa.h-event marpa.h-step marpa.h.p50 marpa.h.p80 \
  marpa_codes.c

clean:
	rm -f $(CLEAN_FILES)
