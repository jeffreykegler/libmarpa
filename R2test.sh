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
# In test, and assuming ../libmarpa is the libmarpa directory:
major=`sh ../libmarpa/libmarpa_version.sh major`
minor=`sh ../libmarpa/libmarpa_version.sh minor`
micro=`sh ../libmarpa/libmarpa_version.sh micro`
ro=cpan/engine/read_only
echo $major $minor $micro | sed -e 's/ /./g' > $ro/LIB_VERSION
cp $ro/LIB_VERSION $ro/LIB_VERSION.in
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
