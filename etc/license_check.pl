#!/usr/bin/perl
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
use autodie;
use English qw( -no_match_vars );

use Getopt::Long;
my $verbose = 1;
my $result = Getopt::Long::GetOptions( 'verbose=i' => \$verbose );
die "usage $PROGRAM_NAME [--verbose=n] file ...\n" if not $result;

use lib 'etc';
use libmarpa::License;

my $file_count = @ARGV;
my @license_problems =
    map { libmarpa::License::file_license_problems( $_, $verbose ) } @ARGV;

print join q{}, @license_problems;

my $problem_count = scalar @license_problems;

$problem_count and say +( q{=} x 50 );
say
    "Found $problem_count license language problems after examining $file_count files";
