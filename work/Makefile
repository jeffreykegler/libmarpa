# Copyright 2015 Jeffrey Kegler
# This file is part of Libmarpa.  Libmarpa is free software: you can
# redistribute it and/or modify it under the terms of the GNU Lesser
# General Public License as published by the Free Software Foundation,
# either version 3 of the License, or (at your option) any later version.
#
# Libmarpa is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser
# General Public License along with Libmarpa.  If not, see
# http://www.gnu.org/licenses/.

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

