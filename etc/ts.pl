#!/usr/bin/env perl
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

use 5.010;
use strict;
use warnings FATAL => 'all';
use autodie;
use POSIX qw(strftime);
use File::Copy;
use File::Spec;
use English qw( -no_match_vars );

sub usage {
   die "Usage: $PROGRAM_NAME from";
}

usage() if scalar @ARGV != 1;
my ($from ) = @ARGV;
die "$from does not exist" if not -e $from;

# Do not worry a lot about portability
my (undef, undef, $filename) = File::Spec->splitpath($from);
my @dotted_pieces = split /[.]/xms, $filename;
my ($base, $extension);
if (@dotted_pieces > 1) {
   $base = join '.', @dotted_pieces[0 .. $#dotted_pieces-1];
   $extension = '.' . $dotted_pieces[-1];
} else {
   $base = $dotted_pieces[0];
   $extension = '';
}
my $date = strftime("%d%m%y", localtime);
my $to = join q{}, $base, '-', $date, $extension;
die "$to exists" if -e $to;
copy($from, $to);
