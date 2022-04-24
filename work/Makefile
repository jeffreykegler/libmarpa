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

install: sources doc/doc_build.stamp doc1/doc1_build.stamp stage/stage_build.stamp

# Order matters in MAKEABLE_DIR -- some directories depend on others
MAKEABLE_DIR = ac ac_doc ac_doc1 ami dev tavl public
CLEANABLE_DIR = $(MAKEABLE_DIR)

sources:
	for d in $(MAKEABLE_DIR); do (cd $$d; $(MAKE)); done

clean:
	for d in $(CLEANABLE_DIR); do (cd $$d; $(MAKE) clean); done
	rm -rf stage

stage/stage_build.stamp:
	test -d stage || mkdir stage
	test -d stage/m4 || mkdir stage/m4
	@( \
          echo stage/configure.ac: ac/configure.ac; \
          echo stage/Makefile.am: ac/Makefile.am; \
          echo stage/Makefile.win32: win32/Makefile.win32; \
          echo stage/win32/do_config_h.pl: win32/do_config_h.pl; \
          echo stage/marpa.c: dev/marpa.c; \
          echo stage/win32/README: dev/README; \
          echo stage/win32/marpa.def: dev/marpa.def; \
          echo stage/marpa.h: public/marpa.h; \
          echo stage/marpa_obs.c: obs/marpa_obs.c; \
          echo stage/marpa_obs.h: obs/marpa_obs.h; \
          echo stage/marpa_ami.c: ami/marpa_ami.c; \
          echo stage/marpa_codes.c: public/marpa_codes.c; \
          echo stage/marpa_codes.h: public/marpa_codes.h; \
          echo stage/marpa_ami.h: ami/marpa_ami.h; \
          echo stage/marpa_avl.c: avl/marpa_avl.c; \
          echo stage/marpa_avl.h: avl/marpa_avl.h; \
          echo stage/marpa_tavl.h: tavl/marpa_tavl.h; \
          echo stage/marpa_tavl.c: tavl/marpa_tavl.c; \
          echo stage/AUTHORS: ac/AUTHORS; \
          echo stage/COPYING: shared/COPYING; \
          echo stage/COPYING.LESSER: obs/COPYING.LESSER; \
          echo stage/ChangeLog: ac/ChangeLog; \
          echo stage/NEWS: ac/NEWS; \
          echo stage/README: ac/README; \
          echo stage/README.INSTALL: etc/README.INSTALL; \
          echo stage/README.AIX: etc/README.AIX; \
          echo stage/GIT_LOG.txt: etc/GIT_LOG.txt; \
          echo stage/libmarpa.pc.in: etc/libmarpa.pc.in; \
          echo stage/error_codes.table: public/error_codes.table; \
          echo stage/events.table: public/events.table; \
          echo stage/steps.table: public/steps.table; \
          echo stage/internals/libmarpa_core.pdf: dev/marpa.pdf; \
          echo stage/internals/libmarpa_ami.pdf: ami/marpa_ami.pdf; \
          echo stage/api_docs/libmarpa_api.pdf: doc1/api.pdf; \
          echo stage/api_docs/libmarpa_api.html: doc1/api.html; \
        ) | perl ../etc/copier.pl --verbose
	cd stage && autoreconf -iv
	cd stage && ./configure
	(cd stage; $(MAKE) dist)
	date > $@

doc/doc_build.stamp:
	@( \
	  echo doc/configure.ac: ac_doc/configure.ac; \
	  echo doc/Makefile.am: ac_doc/Makefile.am; \
	  echo doc/README: ac_doc/README; \
	  echo doc/NEWS: ac/NEWS; \
	  echo doc/AUTHORS: ac/AUTHORS; \
	  echo doc/ChangeLog: ac/ChangeLog; \
	  echo doc/api.texi: dev/api.texi; \
	  echo doc/internal.texi: dev/internal.texi; \
	) | perl ../etc/copier.pl --verbose
	test -d doc/m4 || mkdir doc/m4
	cd doc && autoreconf -ivf
	cd doc && ./configure
	(cd doc; $(MAKE) dist)
	date > $@

doc1/doc1_build.stamp:
	@( \
	  echo doc1/configure.ac: ac_doc1/configure.ac; \
	  echo doc1/Makefile.am: ac_doc1/Makefile.am; \
	  echo doc1/README: ac_doc1/README; \
	  echo doc1/NEWS: ac/NEWS; \
	  echo doc1/AUTHORS: ac/AUTHORS; \
	  echo doc1/ChangeLog: ac/ChangeLog; \
	  echo doc1/api.texi: dev/api.texi; \
	  echo doc1/internal.texi: dev/internal.texi; \
	) | perl ../etc/copier.pl --verbose
	test -d doc1/m4 || mkdir doc1/m4
	cd doc1 && autoreconf -ivf
	cd doc1 && ./configure
	(cd doc1; $(MAKE) html pdf)
	date > $@
