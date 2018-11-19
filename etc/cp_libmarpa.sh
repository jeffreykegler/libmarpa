#!/bin/sh
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

# Copy the libmarpa distribution to a directory
# Note: This script needs to be portable

if [ $# != 1 ] 
then
	echo "usage: $0 target-directory" 1>&2
	exit 1
fi
todir="$1"
fromdir=dist
needtobe="you need to be in a Libmarpa repository top-level directory to run this command"
if [ ! -e "$fromdir" ]
then
    echo "cannot find directory $fromdir -- $needtobe" 1>&2
    exit 1
fi
if [ ! -d "$fromdir" ]
then
    echo "$fromdir is not a directory -- $needtobe" 1>&2
    exit 1
fi
checkfile="$fromdir/configure.ac"
if [ ! -e "$checkfile" ]
then
    echo "$fromdir does not contain a configure.ac file -- $needtobe" 1>&2
    exit 1
fi
if [ ! -r "$checkfile" ]
then
    echo "You not have the sufficient permissions to copy $checkfile" 1>&2
    exit 1
fi
if [ -e "$todir" ]
then
    echo "cannot create directory $todir -- something with that name already exists" 1>&2
    exit 1
fi
mkdir "$todir" || exit 1
# Copy using copying user's permissions and new modification times
(cd "$fromdir"; tar cf - . ) | (cd "$todir"; tar xfom -)
