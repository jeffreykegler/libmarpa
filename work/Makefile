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

.PHONY: install docs docs1 sources stage_copy

dummy:
	@echo install is the main target

install: sources timestamp/doc_build.stamp timestamp/doc1_build.stamp timestamp/stage_build.stamp

# Order matters in MAKEABLE_DIR -- some directories depend on others
MAKEABLE_DIR = ac ami dev tavl public
CLEANABLE_DIR = $(MAKEABLE_DIR)

sources:
	for d in $(MAKEABLE_DIR); do (cd $$d; $(MAKE)); done

clean:
	for d in $(CLEANABLE_DIR); do (cd $$d; $(MAKE) clean); done
	mv timestamp timestamp.$$.temp; mkdir timestamp; \
	  mv timestamp.$$.temp/ABOUT_ME timestamp; rm -r timestamp.$$.temp
	-rm -r stage doc doc1

timestamp/stage_build.stamp:
	test -d stage || mkdir stage
	test -d stage/m4 || mkdir stage/m4
	perl ../etc/copier.pl --verbose < ac/copier.list
	cd stage && autoreconf -iv
	cd stage && ./configure
	(cd stage; $(MAKE) dist)
	date > $@

timestamp/doc_build.stamp:
	@( \
	  echo doc/configure.ac: ac_doc1/configure.ac; \
	  echo doc/Makefile.am: ac_doc/Makefile.am; \
	  echo doc/README: ac_doc/ABOUT_ME; \
	  echo doc/COPYING: shared/COPYING; \
	  echo doc/NEWS: ac/NEWS; \
	  echo doc/AUTHORS: ac/AUTHORS; \
	  echo doc/ChangeLog: ac/ChangeLog; \
	  echo doc/api.texi: dev/api.texi; \
	  echo doc/internal.texi: dev/internal.texi; \
	) | perl ../etc/copier.pl --verbose
	test -d doc/m4 || mkdir doc/m4
	cd doc && autoreconf -ivf
	cd doc && ./configure
	(cd doc; $(MAKE) html)
	(cd doc/api.html; tar cf - .) > doc/api_html.tar
	date > $@

timestamp/doc1_build.stamp:
	@( \
	  echo doc1/configure.ac: ac_doc1/configure.ac; \
	  echo doc1/Makefile.am: ac_doc1/Makefile.am; \
	  echo doc1/README: ac_doc1/ABOUT_ME; \
	  echo doc1/COPYING: shared/COPYING; \
	  echo doc1/NEWS: ac/NEWS; \
	  echo doc1/AUTHORS: ac/AUTHORS; \
	  echo doc1/ChangeLog: ac/ChangeLog; \
	  echo doc1/api.texi: dev/api.texi; \
	  echo doc1/internal.texi: dev/internal.texi; \
	) | perl ../etc/copier.pl --verbose
	test -d doc1/m4 || mkdir doc1/m4
	cd doc1 && autoreconf -ivf
	cd doc1 && ./configure
	(cd doc1; $(MAKE) html pdf info)
	date > $@
