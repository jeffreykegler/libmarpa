# First, clone the Marpa::R2 repo, which we will call "test"
# with something like this:
#     git clone $( cd Marpa--R2; pwd) test
#
# Second, copy a Libmarpa autoconf distribution into the clone
# with a command something like the following:
#     
#    (cd test/cpan/engine/; rm -rf read_only; mkdir read_only)
#    (cd libmarpa/ac_dist; tar cf - .) | (cd test/cpan/engine/read_only; tar xvf -)
# 
# In test, and assuming ../libmarpa is the libmarpa directory:
major=`sh ../libmarpa/libmarpa_version.sh major`
minor=`sh ../libmarpa/libmarpa_version.sh minor`
micro=`sh ../libmarpa/libmarpa_version.sh micro`
ro=cpan/engine/read_only
echo $major $minor $micro | sed -e 's/ /./g' > $ro/LIB_VERSION
cp $ro/LIB_VERSION $ro/LIB_VERSION.in
(cd cpan/xs;make)
sed -e "/^#define EXPECTED_LIBMARPA_MAJOR /s/[0-9]*/$major/" \
   -e "/^#define EXPECTED_LIBMARPA_MINOR /s/[0-9]*/$minor/" \
   -e "/^#define EXPECTED_LIBMARPA_MICRO /s/[0-9]*/$micro/" cpan/xs/R2.xs
# perl Build.PL
# ./Build --code
# ./Build test
