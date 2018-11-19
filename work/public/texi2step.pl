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

if (scalar @ARGV != 2) {
    die("usage: $PROGRAM_NAME step_codes.c steps.table > marpa.h-step");
}

open my $codes_c, '>', $ARGV[0];
open my $codes_table, '>', $ARGV[1];

my @step_type_codes = qw(
MARPA_STEP_INTERNAL1
MARPA_STEP_RULE
MARPA_STEP_TOKEN
MARPA_STEP_NULLING_SYMBOL
MARPA_STEP_TRACE
MARPA_STEP_INACTIVE
MARPA_STEP_INTERNAL2
MARPA_STEP_INITIAL
);

my %step_type_number = map { $step_type_codes[$_], $_ } (0 .. $#step_type_codes);
my @step_types_seen = ();
my @step_types = ();
my $current_step_type_number = undef;

LINE: while ( my $line = <STDIN> ) {

    if ( $line =~ /[@]deftypevr.*MARPA_STEP_/xms ) {
        my ($step_type) = ($line =~ m/(MARPA_STEP_.*)\b/xms);
        if ($step_type) {
            my $step_type_number = $step_type_number{$step_type};
            die("$step_type not in list in $PROGRAM_NAME") if not defined $step_type_number;
            $current_step_type_number = $step_type_number;
            $step_types_seen[$step_type_number] = 1;
            $step_types[$current_step_type_number] = $step_type;
        }
    }

} ## end while ( my $line = <STDIN> )

my $step_type_count = scalar @step_types;
say "#define MARPA_STEP_COUNT $step_type_count";
for ( my $step_type_number = 0; $step_type_number < $step_type_count; $step_type_number++ ) {
    say '#define '
        . $step_types[$step_type_number] . q{ }
        . $step_type_number;
}

say {$codes_table} <<'=== TABLE PREAMBLE ===';
# Format is: number name
# That is each newline-terminated line contains, in order:
#    number, which must be an integer; followed by
#    a single space;  followed by
#    a mnemonic, which is anything not containing whitespace; followed by
#    a newline
#
=== TABLE PREAMBLE ===

say {$codes_c}
    'const struct marpa_step_type_description_s marpa_step_type_description[] = {';
for (
    my $step_type_number = 0;
    $step_type_number < $step_type_count;
    $step_type_number++
    )
{
    my $step_type_name = $step_types[$step_type_number];
    say {$codes_table} join q{ }, $step_type_number, "$step_type_name";
    say {$codes_c} qq[  { $step_type_number, "$step_type_name" },];
} ## end for ( my $step_type_number = 0; $step_type_number...)
say {$codes_c} '};';

# vim: expandtab shiftwidth=4:
