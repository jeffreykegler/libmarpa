# Copyright 2014 Jeffrey Kegler
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
