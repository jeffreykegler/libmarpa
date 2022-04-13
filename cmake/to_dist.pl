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

# Copy things into cm_dist/
# It makes more sense to do this in Perl than in the Makefile

use 5.010;
use File::Spec;
use File::Copy;
use Getopt::Long;
use autodie;    # Portability not essential in this script

my $verbose;
GetOptions( "verbose|v" => \$verbose )
    or die("Error in command line arguments\n");

FILE: while ( my $copy = <DATA> ) {
    chomp $copy;
    my ( $to, $from ) = $copy =~ m/\A (.*) [:] \s+ (.*) \z/xms;
    die "Bad copy spec: $copy" if not defined $to;
    next FILE if -e $to and ( -M $to <= -M $from );
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
    say "Copied $from -> $to" if $verbose;
} ## end FILE: while ( my $copy = <DATA> )

# Note that order DOES matter here -- any configure.ac files
# MUST be FIRST

__DATA__
cm_dist/marpa.c: work/stage/marpa.c
cm_dist/include/marpa.h: work/stage/marpa.h
cm_dist/libmarpa.pc.in: work/stage/libmarpa.pc.in
cm_dist/marpa_obs.c: work/stage/marpa_obs.c
cm_dist/marpa_obs.h: work/stage/marpa_obs.h
cm_dist/marpa_ami.c: work/stage/marpa_ami.c
cm_dist/marpa_ami.h: work/stage/marpa_ami.h
cm_dist/marpa_avl.c: work/stage/marpa_avl.c
cm_dist/marpa_avl.h: work/stage/marpa_avl.h
cm_dist/marpa_tavl.h: work/stage/marpa_tavl.h
cm_dist/marpa_tavl.c: work/stage/marpa_tavl.c
cm_dist/error_codes.table: work/stage/error_codes.table
cm_dist/steps.table: work/stage/steps.table
cm_dist/events.table: work/stage/events.table
cm_dist/COPYING.LESSER: work/stage/COPYING.LESSER
cm_dist/COPYING: work/stage/COPYING
cm_dist/README: work/stage/README
cm_dist/CMakeLists.txt: cmake/CMakeLists.txt
cm_dist/config.h.cmake: cmake/config.h.cmake
cm_dist/modules/FindInline.cmake: cmake/modules/FindInline.cmake
cm_dist/modules/FindNullIsZeroes.cmake: cmake/modules/FindNullIsZeroes.cmake
cm_dist/modules/inline.c: cmake/modules/inline.c
