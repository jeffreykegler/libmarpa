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

MAJOR=11
MINOR=0
MICRO=2
VERSION=$(MAJOR).$(MINOR).$(MICRO)

.PHONY: ac_dist asan clean cm_dist dist distcheck dummy \
    realclean tag test cm_test install_test

dummy:
	@echo The target to make the Autoconf distribution is '"ac_dist"'
	@echo The target to make the Cmake distribution is '"cm_dist"'
	@echo The target to make both distributions is '"dist"'
	@echo The target to test Libmarpa is '"test"'

dist: ac_dist cm_dist

timestamp/stage.stamp:
	$(MAKE) libmarpa_version.sh
	cp libmarpa_version.sh work/etc/
	sh libmarpa_version.sh version > work/etc/LIB_VERSION
	(cd work; $(MAKE) install)
	date > timestamp/stage.stamp
	@echo Updating stage time stamp: `cat timestamp/stage.stamp`

ac_dist: timestamp/ac_dist.stamp

timestamp/ac_dist.stamp: timestamp/stage.stamp
	cp work/stage/libmarpa-$(VERSION).tar.gz tars
	tar -xvzf tars/libmarpa-$(VERSION).tar.gz
	rm -r ac_dist || true
	mv libmarpa-$(VERSION) ac_dist
	date > timestamp/ac_dist.stamp
	@echo Updating ac_dist time stamp: `cat timestamp/ac_dist.stamp`

timestamp/cm_dist.stamp: timestamp/stage.stamp
	@echo cm_dist Out of date wrt ac_dist
	rm -r cm_dist || true
	cd cmake; make
	perl ./etc/copier.pl --verbose < cmake/copier.list
	date > timestamp/cm_dist.stamp
	@echo Updating cm_dist time stamp: `cat timestamp/cm_dist.stamp`

distcheck:
	perl etc/license_check.pl  --verbose=0 `find Makefile cm_dist ac_dist -type f`

tag:
	git tag -a v$(version) -m "Version $(VERSION)"

cm_dist: timestamp/cm_dist.stamp

# We make the Addresser sanatizer test extra verbose.
# In particular, I want some output from even a clean run of
# ASAN to make sure it is turned on.
asan: timestamp/cm_dist.stamp
	rm -rf cm_build
	cmake -DCMAKE_BUILD_TYPE:STRING=Asan -S cm_dist -B cm_build
	cd cm_build && $(MAKE) VERBOSE=1
	cat cm_dist/tap_test/tests/TESTS | while read f; do \
	     (cd cm_build/tap_test/tests/; \
	     env ASAN_OPTIONS=atexit=true ./runtests -o $$f); \
	done

test: timestamp/ac_dist.stamp
	rm -rf ac_build
	mkdir ac_build
	cd ac_build && ../ac_dist/configure && make check

# For even more debugging:
#     cmake -DCMAKE_BUILD_TYPE:STRING=Debug -S cm_dist -B cm_build
#
cm_test: timestamp/cm_dist.stamp
	rm -rf cm_build
	cmake -S cm_dist -B cm_build
	cd cm_build && $(MAKE) VERBOSE=1 && \
	$(MAKE) VERBOSE=1 test ARGS="-V"

# While we do build a shared library, Libmarpa is not primarily intended to be installed
# as a system library.  Instead, Libmarpa is expected to be incorporated directly, perhaps
# as a static library, into executables, or into other libraries.
#
# The Perl module is an important case, but unusual case.
# The static library is compiled for position-independent-code (PIC) and then
# linked into a shared library.
#
# Since installing the shared library is an unusual special case, we want to be able
# to ensure that it does work for those who want it.  The following test does this.
# Run this target and then check that the hierarchy below test_install/ looks the
# way the standard hierarchy of installation directories normally does, which is something
# like
#
# test_install/include/marpa.h
# test_install/lib/pkgconfig/libmarpa.pc
# test_install/lib/libmarpa.so
# test_install/lib/libmarpa_s.a
test_install: timestamp/cm_dist.stamp
	rm -rf cm_build test_install
	mkdir test_install
	(cd test_install; tgt=`pwd`; cd ..; echo "TARGET $$tgt"; \
	  cmake -DCMAKE_INSTALL_PREFIX:PATH=$$tgt -S cm_dist -B cm_build)
	cd cm_build && $(MAKE) VERBOSE=1 && $(MAKE) install

clean:
	-rm libmarpa_version.sh
	(cd work; $(MAKE) clean)
	(cd cmake; $(MAKE) clean)
	rm -rf ac_dist ac_build
	rm -rf cm_dist cm_build
	rm -rf test_install
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
