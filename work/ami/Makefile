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

CWEAVE = cweave -p
CTANGLE = ctangle -p

.PHONY: all

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

all: marpa_ami.pdf sources

sources: marpa_ami.c marpa_ami.h

C_LANGUAGE_PREAMBLE = ../shared/license.c ../shared/do_not_edit.c
MARPA_AMI_H_PIECES =  $(C_LANGUAGE_PREAMBLE) marpa_ami.h.p10 marpa_ami.h.p50  marpa_ami.h.p90
MARPA_AMI_C_PIECES = $(C_LANGUAGE_PREAMBLE) marpa_ami.c.p10 marpa_ami.c.p40 marpa_ami.c.p50

marpa_ami.h.p10 marpa_ami.h.p90 marpa_ami.c.p10: ../shared/copyright_page_license.w marpa_ami.w
	$(CTANGLE) marpa_ami.w

cwebmac.tex: ../shared/cwebmac.tex
	cp $? $@

marpa_ami.tex: cwebmac.tex ../shared/copyright_page_license.w

marpa_ami.pdf: marpa_ami.tex

marpa_ami.h: $(MARPA_AMI_H_PIECES)
	cat $(MARPA_AMI_H_PIECES) > $@

marpa_ami.c: $(MARPA_AMI_C_PIECES)
	cat $(MARPA_AMI_C_PIECES) > $@

CPPWRAP = perl ../bin/cppwrap.pl

marpa_ami.c.p40: marpa_ami.w
	perl w2private_h.pl < $? | $(CPPWRAP) $@ > $@

marpa_ami.h.p50: api.texi internal.texi
	cat api.texi internal.texi | \
	  perl ../bin/texi2proto.pl /dev/null > marpa_ami.h.p50

update_texi:
	emacs -batch internal.texi \
	    -f texinfo-every-node-update \
	    -f texinfo-all-menus-update \
	    -f save-buffer -kill

CLEAN_FILES = cwebmac.tex marpa_ami.c marpa_ami.c.p10 marpa_ami.c.p40 marpa_ami.c.p50 \
  marpa_ami.h marpa_ami.h.p10 marpa_ami.h.p50 marpa_ami.h.p90 marpa_ami.idx \
  marpa_ami.log marpa_ami.pdf marpa_ami.scn marpa_ami.tex marpa_ami.toc

clean:
	rm -f $(CLEAN_FILES)
