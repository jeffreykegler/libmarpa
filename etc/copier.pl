#!perl
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

# Copy things according to instructions in the input

use 5.010;
use strict;
use warnings;

use File::Spec;
use File::Copy;
use Getopt::Long;
use autodie;    # Portability not essential in this script

my $verbose;
my $stampfile;
GetOptions( "verbose|v" => \$verbose,
  "stamp=s" => \$stampfile
)
    or die("Error in command line arguments\n");

my $copy_count = 0;

FILE: while ( my $copy = <STDIN> ) {
    chomp $copy;
    $copy =~ s/[#] .* \z//xms;
    next FILE if $copy =~ m/\A \w* \z/xms;
    my ( $to, $from ) = $copy =~ m/\A (.*) [:] \s+ (.*) \z/xms;
    die "Bad copy spec: $copy" if not defined $to;
    # Don't skip copying even if timestamps suggest it is unnecessary
    # next FILE if -e $to and ( -M $to <= -M $from );
    my ( undef, $to_dirs, $to_file ) = File::Spec->splitpath($to);
    my @to_dirs = File::Spec->splitdir($to_dirs);
    my @dir_found_so_far = ();
    # Make the directories we do not find
    DIR_PIECE: for my $dir_piece (@to_dirs) {
	push @dir_found_so_far, $dir_piece;
	my $dir_so_far = File::Spec->catdir(@dir_found_so_far);
        next DIR_PIECE if -e $dir_so_far;
	mkdir $dir_so_far;
    }
    File::Copy::copy($from, $to) or die "Cannot copy $from -> $to";
    $copy_count++;
    say "Copied $from -> $to" if $verbose;
} ## end FILE: while ( my $copy = <STDIN> )

say "Files copied: $copy_count";

# If we have defined a stamp file, and we copied files
# or there is no stamp file, update it.
if ($stampfile and ($copy_count or not -e $stampfile)) {
   open my $stamp_fh, q{>}, $stampfile;
   say {$stamp_fh} "" . localtime;
   close $stamp_fh;
}
