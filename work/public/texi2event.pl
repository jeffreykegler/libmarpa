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
    die("usage: $PROGRAM_NAME marpa.c-event events.table > marpa.h-event");
}

open my $codes_c, '>', $ARGV[0];
open my $codes_table, '>', $ARGV[1];

my @event_codes = qw(
MARPA_EVENT_NONE
MARPA_EVENT_COUNTED_NULLABLE
MARPA_EVENT_EARLEY_ITEM_THRESHOLD
MARPA_EVENT_EXHAUSTED
MARPA_EVENT_LOOP_RULES
MARPA_EVENT_NULLING_TERMINAL
MARPA_EVENT_SYMBOL_COMPLETED
MARPA_EVENT_SYMBOL_EXPECTED
MARPA_EVENT_SYMBOL_NULLED
MARPA_EVENT_SYMBOL_PREDICTED
);

my %event_number = map { $event_codes[$_], $_ } (0 .. $#event_codes);
my @events_seen = ();
my @events = ();
my $current_event_number = undef;
my @event_suggested_messages = ();

LINE: while ( my $line = <STDIN> ) {

     if ( defined $current_event_number ) {
        my ($message) = ($line =~ /Suggested \s* message [:] \s " ([^"]*) " /xms );
        if ($message) {
            $event_suggested_messages[$current_event_number] = $message;
            $current_event_number = undef;
        }
     }
    if ( $line =~ /[@]deftypevr.*MARPA_EVENT_/xms ) {
        my ($event) = ($line =~ m/(MARPA_EVENT_.*)\b/xms);
        if ($event) {
            my $event_number = $event_number{$event};
            die("$event not in list in $PROGRAM_NAME") if not defined $event_number;
            $current_event_number = $event_number;
            $events_seen[$event_number] = 1;
            $events[$current_event_number] = $event;
        }
    }

} ## end while ( my $line = <STDIN> )

my @events_not_seen = grep { !$events_seen[$_] } (0 .. $#event_codes);
if (@events_not_seen) {
  for my $event_not_seen (@events_not_seen) {
      say STDERR "Event not in document: ", $event_codes[$event_not_seen];
  }
  die 'Event(s) in list, but not in document';
}

my $event_count = scalar @events;
say "#define MARPA_EVENT_COUNT $event_count";
for ( my $event_number = 0; $event_number < $event_count; $event_number++ ) {
    say '#define '
        . $events[$event_number] . q{ }
        . $event_number;
}

say {$codes_table} <<'=== TABLE PREAMBLE ===';
# Format is: number mnemonic message
# That is each newline-terminated line contains, in order:
#    number, which must be an integer; followed by
#    a single space;  followed by
#    a mnemonic, which is anything not containing whitespace; followed by
#    a single space;  followed by
#    a message, which is anything not containing a newline
#
=== TABLE PREAMBLE ===

say {$codes_c}
    'const struct marpa_event_description_s marpa_event_description[] = {';
for ( my $event_number = 0; $event_number < $event_count; $event_number++ ) {
    my $suggested_description = $event_suggested_messages[$event_number]
        // "Unknown event";
    my $event_name = $events[$event_number];
    say {$codes_c}
        qq[  { $event_number, "$event_name", "$suggested_description" },];
    say {$codes_table}
        join q{ }, $event_number, "$event_name", "$suggested_description";
} ## end for ( my $event_number = 0; $event_number < $event_count...)
say {$codes_c} '};';

# vim: expandtab shiftwidth=4:
