#!perl
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

use 5.010;
use strict;
use warnings;
use English qw( -no_match_vars );
use Fatal qw(open close);

if (scalar @ARGV != 0) {
    die("usage: $PROGRAM_NAME < marpa.w > private.h");
}

my $file = do { local $RS = undef; <STDIN>; };
for my $prototype ($file =~ m/^PRIVATE_NOT_INLINE \s (.*?) \s* ^[{]/gxms)
{
   $prototype =~ s/[@][,]//g; # Remove Cweb spacing
   say 'static ' . $prototype . q{;};
}
for my $prototype ($file =~ m/^PRIVATE \s (.*?) \s* ^[{]/gxms)
{
   $prototype =~ s/[@][,]//g; # Remove Cweb spacing
   say 'static inline ' . $prototype . q{;};
}

