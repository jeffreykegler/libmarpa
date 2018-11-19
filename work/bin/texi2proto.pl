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

sub usage {
    say STDERR "usage: $0 def_file <texi_file > proto_file";
    exit 1;
}

usage() unless @ARGV == 1;
my ($def_file) = @ARGV;
open my $def_fh, q{>}, $def_file;

my @protos;
my @defs;
LINE: while ( my $line = <STDIN> ) {

    next LINE if $line =~ m/ [{] Macro [}] /xms;

    next LINE if $line !~ m/[@]deftypefun/xms;

    my $def = q{};
    while ( $line =~ / [@] \s* \z /xms ) {
        $def .= $line;
        $def =~ s/ [@] \s* \z//xms;
        $line = <STDIN>;
    }
    $def .= $line;
    $def =~ s/\A \s* [@] deftypefun x? \s* //xms;
    $def =~ s/ [@]var[{] ([^}]*) [}]/$1/xmsg;
    $def =~ s/ [@]code[{] ([^}]*) [}]/$1/xmsg;
    $def =~ s/\s+/ /xmsg;
    $def =~ s/\s \z/;/xmsg;
    push @protos, $def;

    $def =~ s/ \s* [(] .* //xms;
    $def =~ s/ \s* [(] .* //xms;
    $def =~ s/ \A .* \s //xms;
    push @defs, $def;

} ## end LINE: while ( my $line = <STDIN> )

say join "\n", @protos;

my $def_preamble = << 'END_OF_PREAMBLE';
; DO NOT EDIT DIRECTLY.
; This file was automatically generated.
; This file is for the Microsoft linker.
END_OF_PREAMBLE
$def_preamble .= q{; } . localtime() . "\n";

say {$def_fh} $def_preamble, "EXPORTS\n", join "\n",
    map { q{   } . $_ } @defs;

# vim: expandtab shiftwidth=4:
