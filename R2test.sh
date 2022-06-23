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

# This file is to document the procedure for testing with Marpa::R2.
# It can be run as a shell script to implement some of the procedure.
#
# Step 1: clone the Marpa::R2 repo.   Clone into a repo
# which we will call "test", with something like this:
#
#     git clone $( cd Marpa--R2; pwd) test
#
# Step 2: Copy a Libmarpa autoconf distribution into the clone
# with commands something like the following:
#     
#    (cd test/cpan/engine/; rm -rf read_only; mkdir read_only)
#    (cd libmarpa/ac_dist; tar cf - .) | (cd test/cpan/engine/read_only; tar xvf -)
# 
# Step 3: Hack Marpa::R2 repo and distro to match.
# In test, do the following:
ro=cpan/engine/read_only
major=`sh $ro/libmarpa_version.sh major`
minor=`sh $ro/libmarpa_version.sh minor`
micro=`sh $ro/libmarpa_version.sh micro`
(cd cpan/xs;make)
sed -e "/^#define EXPECTED_LIBMARPA_MAJOR /s/[0-9][0-9]*/$major/" \
   -e "/^#define EXPECTED_LIBMARPA_MINOR /s/[0-9][0-9]*/$minor/" \
   -e "/^#define EXPECTED_LIBMARPA_MICRO /s/[0-9][0-9]*/$micro/" \
     cpan/xs/R2.xs > cpan/xs/R2.xs.hacked
mv cpan/xs/R2.xs.hacked cpan/xs/R2.xs
#
# Step 4: Build in the test/cpan directory:
# perl Build.PL
# ./Build --code
# ./Build test
#
# No-clone Variation:
# To use a new autoconf distro without a new clone, skip Step 1
# (cloning Marpa::R2) and run ".Build clean" before "./Build code"
# in the build (Step 4).
#
# Compile for debug variation:
# In Step 4, instead of "./Build code", run
#
# LIBMARPA_CFLAGS=-g ./Build code --Marpa-debug=1 --config optimize=-g
