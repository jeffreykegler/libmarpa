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

version=`work/stage/configure --version | sed -ne '1s/^libmarpa configure *//p'`
tar_file=work/doc1/libmarpa-doc1-$version.tar.gz
if test -d doc1_dist && test doc1_dist/stamp-h1 -nt $tar_file;
then exit 0;
fi
rm -rf doc1_dist
mkdir doc1_dist.$$
(cd doc1_dist.$$; tar -xzf ../$tar_file)
mv doc1_dist.$$/libmarpa-doc1-$version doc1_dist
date > doc1_dist/stamp-h1
rmdir doc1_dist.$$
