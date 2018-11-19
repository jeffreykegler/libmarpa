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

.PHONY: install docs docs1 sources stage_copy

dummy:
	@echo install is the main target


doc/api.pdf: stage_copy.stamp doc/Makefile doc/api.texi
	cd doc && make pdf

doc/api.html/index.html: stage_copy.stamp doc/Makefile doc/api.texi
	cd doc && make html

doc1/api.html/index.html: stage_copy.stamp doc1/Makefile doc1/api.texi
	cd doc1 && make html

doc/internal.pdf: stage_copy.stamp doc/Makefile doc/internal.texi
	cd doc && make pdf

doc/internal.html/index.html: stage_copy.stamp doc/Makefile doc/internal.texi
	cd doc && make html

doc1/internal.html/index.html: stage_copy.stamp doc1/Makefile doc1/internal.texi
	cd doc1 && make html

install: sources stage/stage.stamp doc/doc.stamp doc1/doc1.stamp

MAKEABLE_DIR = ami dev tavl public
CLEANABLE_DIR = $(MAKEABLE_DIR)

sources:
	for d in $(MAKEABLE_DIR); do (cd $$d; make); done
	perl bin/to_stage.pl --stamp stage_copy.stamp

clean:
	for d in $(CLEANABLE_DIR); do (cd $$d; make clean); done
	rm -rf stage

stage/stage.stamp: stage_copy.stamp
	test -d stage || mkdir stage
	test -d stage/m4 || mkdir stage/m4
	cd stage && autoreconf -iv
	cd stage && ./configure
	(cd stage; make dist)
	date > $@

doc/doc.stamp: stage_copy.stamp
	test -d doc/m4 || mkdir doc/m4
	cd doc && autoreconf -ivf
	cd doc && ./configure
	(cd doc; make dist)
	date > $@

doc1/doc1.stamp: stage_copy.stamp
	test -d doc1/m4 || mkdir doc1/m4
	cd doc1 && autoreconf -ivf
	cd doc1 && ./configure
	(cd doc1; make dist)
	date > $@

docs: \
  doc/Makefile \
  doc/api.pdf doc/api.html/index.html \
  doc/internal.pdf doc/internal.html/index.html \

docs1: \
  doc1/Makefile doc1/api.html/index.html \
  doc1/internal.html/index.html

