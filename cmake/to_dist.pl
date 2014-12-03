#!perl
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

# Copy things into stage/
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

# Note that order DOES matter here -- the configure.ac files
# MUST be FIRST

__DATA__
cmstage/marpa.c: work/dev/marpa.c
cmstage/marpa.h: work/public/marpa.h
cmstage/marpa_slif.h: work/public/marpa_slif.h
cmstage/marpa_obs.c: work/obs/marpa_obs.c
cmstage/marpa_obs.h: work/obs/marpa_obs.h
cmstage/marpa_ami.c: work/ami/marpa_ami.c
cmstage/marpa_codes.c: work/public/marpa_codes.c
cmstage/marpa_slif.c: work/slif/marpa_slif.c
cmstage/marpa_ami.h: work/ami/marpa_ami.h
cmstage/marpa_avl.c: work/avl/marpa_avl.c
cmstage/marpa_avl.h: work/avl/marpa_avl.h
cmstage/marpa_tavl.h: work/tavl/marpa_tavl.h
cmstage/marpa_tavl.c: work/tavl/marpa_tavl.c
cmstage/COPYING.LESSER: work/ac/COPYING.LESSER
cmstage/README: work/ac/README
cmstage/LIB_VERSION.in: work/public/LIB_VERSION.in
cmstage/CMakeLists.txt: work/cmake/CMakeLists.txt
