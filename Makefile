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

version=`cat LIB_VERSION`

.PHONY: dummy dist

dummy: 

dists: dist doc_dist

work_install:
	(cd work; make install)

tar: work_install
	cp work/stage/libmarpa-$(version).tar.gz .

doc_tar: work_install
	cp work/doc/libmarpa-doc-$(version).tar.gz .

dist: tar
	sh etc/work_to_dist.sh

doc_dist: doc_tar
	sh etc/work_to_doc_dist.sh

distcheck:
	perl etc/license_check.pl  --verbose=0 `find dist doc_dist -type f`

tar_clean:
	rm work/doc/*.tar.gz
	rm work/stage/*.tar.gz

tag:
	git tag -a v$(version) -m "Version $(version)"
