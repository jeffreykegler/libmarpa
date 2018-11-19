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

version=`cat LIB_VERSION`

.PHONY: dummy dist doc_dist doc1_dist cm_dist test tar work_install

dummy:
	@echo The target to make the distributions is '"dists"'

dists: dist doc_dist doc1_dist cm_dist

work_install:
	(cd work; make install)

tar: work_install timestamp/tar.stamp

timestamp/tar.stamp: work/doc/doc.stamp \
  work/doc1/doc1.stamp \
  work/stage/stage.stamp
	cp work/stage/libmarpa-$(version).tar.gz .
	test -d timestamp || mkdir timestamp
	date > timestamp/tar.stamp

doc_tar: work_install
	cp work/doc/libmarpa-doc-$(version).tar.gz .

doc1_tar: work_install
	cp work/doc1/libmarpa-doc1-$(version).tar.gz .

dist: tar
	sh etc/work_to_dist.sh

doc_dist: doc_tar
	sh etc/work_to_doc_dist.sh

doc1_dist: doc1_tar
	sh etc/work_to_doc1_dist.sh

timestamp/cm_dist.stamp: timestamp/tar.stamp
	perl cmake/to_dist.pl
	test -d timestamp || mkdir timestamp
	date > timestamp/cm_dist.stamp

distcheck:
	perl etc/license_check.pl  --verbose=0 `find Makefile cm_dist dist doc_dist doc1_dist -type f`

tar_clean:
	rm -f work/doc/*.tar.gz
	rm -f work/doc1/*.tar.gz
	rm -f work/stage/*.tar.gz

tag:
	git tag -a v$(version) -m "Version $(version)"

timestamp/cm_debug.stamp: timestamp/cm_dist.stamp
	rm -rf cm_build
	mkdir cm_build
	cd cm_build && cmake -DCMAKE_BUILD_TYPE:STRING=Debug ../cm_dist && make VERBOSE=1
	cd cm_build && make DESTDIR=../test install
	test -d timestamp || mkdir timestamp
	date > timestamp/cm_debug.stamp

timestamp/do_test.stamp: timestamp/cm_debug.stamp
	rm -rf do_test
	mkdir do_test
	cd do_test && cmake ../test && make VERBOSE=1
	test -d timestamp || mkdir timestamp
	date > timestamp/do_test.stamp

test: work_install timestamp/do_test.stamp
	cd do_test && make && ./tap/runtests -l ../test/TESTS

test_clean:
	rm -f timestamp/do_test.stamp

dist_clean: clean
	rm -rf dist
	rm -rf doc_dist
	rm -rf doc1_dist
	rm -f libmarpa-$(version).tar.gz libmarpa-doc-$(version).tar.gz libmarpa-doc1-$(version).tar.gz

clean:
	rm -rf work/doc
	rm -rf work/doc1
	rm -rf work/stage
	rm -rf cm_build
	rm -rf cm_dist
	rm -rf do_test
	rm -rf timestamp

realclean: dist_clean test_clean tar_clean
