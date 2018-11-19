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

CWEAVE = cweave -p
CTANGLE = ctangle -p

.PHONY: docs

.SUFFIXES: .dvi .tex .w .pdf

.w.tex:
	$(CWEAVE) $*

.tex.dvi:	
	tex $<

.w.dvi:
	make $*.tex
	make $*.dvi

.w.c:
	$(CTANGLE) $*

.w.o:
	make $*.c
	make $*.o

.w.pdf:
	make $*.tex
	pdftex -interaction=batchmode -halt-on-error $* || cat $*.log

all: legacy.pdf marpa.pdf sources

sources: marpa.c marpa.h.p50 marpa.h.p80 marpa.h.p80

C_LANGUAGE_PREAMBLE = ../shared/license.c ../shared/do_not_edit.c
MARPA_C_PIECES = $(C_LANGUAGE_PREAMBLE) marpa.c.p10 marpa.c.p40 marpa.c.p50

marpa.c: $(MARPA_C_PIECES)
	cat $(MARPA_C_PIECES) > $@

marpa.h.p50 marpa.c.p10 marpa.c.p50: ../shared/copyright_page_license.w marpa.w
	$(CTANGLE) marpa.w

marpa.h.p80 marpa.def: api.texi internal.texi
	cat api.texi internal.texi | \
	  perl ../bin/texi2proto.pl marpa.def > marpa.h.p80

CPPWRAP = perl ../bin/cppwrap.pl

marpa.c.p40: marpa.w
	perl w2private_h.pl < $? | $(CPPWRAP) $@ > $@

cwebmac.tex: ../shared/cwebmac.tex
	cp $? $@

marpa.pdf: marpa.tex cwebmac.tex

legacy.pdf: legacy.tex cwebmac.tex

docs: marpa.pdf legacy.pdf

update_texi:
	emacs -batch api.texi \
	    -eval '(texinfo-master-menu 1)' \
	    -f save-buffer -kill
	emacs -batch internal.texi \
	    -eval '(texinfo-master-menu 1)' \
	    -f save-buffer -kill

CLEAN_FILES = marpa.c marpa.c.p10 marpa.c.p40 marpa.c.p50 \
  marpa.h.p50 marpa.h.p80 \
  marpa.idx marpa.log marpa.pdf marpa.scn marpa.tex marpa.toc \
  legacy.idx legacy.log legacy.pdf legacy.scn legacy.tex legacy.toc

clean:
	rm -f $(CLEAN_FILES)
