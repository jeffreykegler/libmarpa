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
tar_file=work/doc/libmarpa-doc-$version.tar.gz
if test -d doc_dist && test doc_dist/stamp-h1 -nt $tar_file;
then exit 0;
fi
rm -rf doc_dist
mkdir doc_dist.$$
(cd doc_dist.$$; tar -xzf ../$tar_file)
mv doc_dist.$$/libmarpa-doc-$version doc_dist
date > doc_dist/stamp-h1
rmdir doc_dist.$$
