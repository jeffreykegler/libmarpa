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

# Releases with major number 0-9 are development releases.  Also releases
# with odd major numbers are development releases

# Releases with even major numbers, beginning with major number 10, are
# stable releases

MAJOR=9
MINOR=0
MICRO=4
VERSION=$(MAJOR).$(MINOR).$(MICRO)

.PHONY: ac_dist asan clean cm_dist dist distcheck dummy \
    realclean tag test test_clean

dummy:
	@echo The target to make the distribution is '"dist"'

timestamp/stage.stamp:
	$(MAKE) libmarpa_version.sh
	cp libmarpa_version.sh work/etc/
	sh libmarpa_version.sh version > work/etc/LIB_VERSION
	(cd work; $(MAKE) install)
	date > timestamp/stage.stamp
	@echo Updating stage time stamp: `cat timestamp/stage.stamp`

dist: timestamp/ac_dist.stamp \
  timestamp/cm_dist.stamp

timestamp/ac_dist.stamp: timestamp/stage.stamp
	cp work/stage/libmarpa-$(VERSION).tar.gz tars
	tar -xvzf tars/libmarpa-$(VERSION).tar.gz
	rm -r ac_dist || true
	mv libmarpa-$(VERSION) ac_dist
	date > timestamp/ac_dist.stamp
	@echo Updating ac_dist time stamp: `cat timestamp/ac_dist.stamp`

timestamp/cm_dist.stamp: timestamp/ac_dist.stamp
	@echo cm_dist Out of date wrt ac_dist
	cd cmake; make
	perl ./etc/copier.pl --verbose < cmake/copier.list
	date > timestamp/cm_dist.stamp
	@echo Updating cm_dist time stamp: `cat timestamp/cm_dist.stamp`

distcheck:
	perl etc/license_check.pl  --verbose=0 `find Makefile cm_dist ac_dist -type f`

tag:
	git tag -a v$(version) -m "Version $(VERSION)"

cm_dist: timestamp/cm_dist.stamp

timestamp/cm_debug.stamp: timestamp/cm_dist.stamp
	@echo cm_debug Out of date wrt cm_dist
	rm -rf cm_build
	mkdir cm_build
	cmake -DCMAKE_BUILD_TYPE:STRING=Debug -S cm_dist -B cm_build
	cd cm_build && $(MAKE) VERBOSE=1 DESTDIR=../test install
	# Shares a directory with the cm_build time stamp
	-rm timestamp/cm_build.stamp
	date > timestamp/cm_debug.stamp
	@echo Updating cm_debug time stamp: `cat timestamp/cm_debug.stamp`

asan: timestamp/cm_debug.stamp
	rm -rf do_test
	mkdir do_test
	cmake -DCMAKE_BUILD_TYPE:STRING=Asan -S test -B do_test
	cd do_test && $(MAKE) VERBOSE=1 && ./tap/runtests -l ../test/TESTS

timestamp/test.stamp: timestamp/cm_debug.stamp
	@echo test Out of date wrt cm_debug
	rm -rf do_test
	mkdir do_test
	cmake -S test -B do_test
	-rm timestamp/asan_test.stamp
	date > timestamp/test.stamp
	@echo Updating test time stamp: `cat timestamp/test.stamp`

test: timestamp/ac_dist.stamp
	rm -rf ac_build
	mkdir ac_build
	cd ac_build && ../ac_dist/configure && make check

cm_test: timestamp/cm_dist.stamp
	rm -rf cm_build
	cmake -S cm_dist -B cm_build
	cd cm_build && $(MAKE) && $(MAKE) test

old_test: timestamp/test.stamp
	cd do_test && $(MAKE) VERBOSE=1 && ./tap/runtests -l ../test/TESTS

old_test_clean:
	rm -f timestamp/do_test.stamp

clean:
	-rm libmarpa_version.sh
	(cd work; $(MAKE) clean)
	(cd cmake; $(MAKE) clean)
	rm -rf ac_dist ac_build
	rm -rf cm_dist cm_build
	rm -rf do_test
	mv timestamp timestamp.$$.temp; mkdir timestamp; \
	  mv timestamp.$$.temp/ABOUT_ME timestamp; rm -r timestamp.$$.temp
	mv tars tars.$$.temp; mkdir tars; \
	  mv tars.$$.temp/ABOUT_ME tars; rm -r tars.$$.temp

realclean: clean

libmarpa_version.sh:
	@echo 'for arg;do' > $@
	@echo '  if test "$$arg" = major; then echo '$(MAJOR)'; continue; fi' >> $@
	@echo '  if test "$$arg" = minor; then echo '$(MINOR)'; continue; fi' >> $@
	@echo '  if test "$$arg" = micro; then echo '$(MICRO)'; continue; fi' >> $@
	@echo '  if test "$$arg" = version; then echo '$(VERSION)'; continue; fi' >> $@
	@echo '  echo Bad arg to $$0: $$arg' >> $@
	@echo 'done' >> $@
