#!/usr/bin/perl
# Copyright 2013 Jeffrey Kegler
# This file is part of Marpa::R2.  Marpa::R2 is free software: you can
# redistribute it and/or modify it under the terms of the GNU Lesser
# General Public License as published by the Free Software Foundation,
# either version 3 of the License, or (at your option) any later version.
#
# Marpa::R2 is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser
# General Public License along with Marpa::R2.  If not, see
# http://www.gnu.org/licenses/.

# This example parses ambiguous English sentences.  The target annotation
# is Penn Treebank's syntactic bracketing tags.  For details, see
# http://www.cis.upenn.edu/~treebank/

# This example originally came from Ralf Muschall.  Ruslan Shvedov
# reworked my implementation, converting it to the SLIF and
# Penn Treebank.  Ruslan and Ralf clearly know English grammar better than
# most of us native speakers.

# 'time', 'fruit', and 'flies' can be nouns or verbs, 'like' can be
# a preposition or a verb.  This creates syntactic ambiguity shown 
# in the parse results.

# Modifier nouns are not tagged or lexed as adjectives (JJ), because
# "Nouns that are used as modifiers, whether in isolation or in sequences,
# should be tagged as nouns (NN, NNS) rather than as adjectives (JJ)."
# -- ftp://ftp.cis.upenn.edu/pub/treebank/doc/tagguide.ps.gz

# The saying "time flies like an arrow; fruit flies like a banana"
# is attributed to Groucho Marx, but there is no reason to believe
# he ever said it.  Apparently, the saying
# first appeared on the Usenet on net.jokes in 1982.
# I've documented this whole thing on Wikipedia:
# http://en.wikipedia.org/wiki/Time_flies_like_an_arrow
#
# The permalink is:
# http://en.wikipedia.org/w/index.php?title=Time_flies_like_an_arrow&oldid=311163283

use 5.010;
use strict;
use warnings;
use English qw( -no_match_vars );

use Test::More tests => 1;
use lib 'inc';
use Marpa::R2::Test;
use Marpa::R2;

my $grammar = Marpa::R2::Scanless::G->new(
    {   bless_package => 'PennTags',
        source => \(<<'END_OF_SOURCE'),
        
:default ::= action => [values] bless => ::lhs
lexeme default = action => [value] bless => ::name

S   ::= NP  VP  period  bless => S

NP  ::= NN              bless => NP
    |   NNS          bless => NP
    |   DT  NN          bless => NP
    |   NN  NNS         bless => NP
    |   NNS CC NNS  bless => NP

VP  ::= VBZ NP          bless => VP
    | VP VBZ NNS        bless => VP
    | VP CC VP bless => VP
    | VP VP CC VP bless => VP
    | VBZ bless => VP

period ~ '.'

:discard ~ whitespace
whitespace ~ [\s]+

CC ~ 'and'
DT  ~ 'a' | 'an'
NN  ~ 'panda'
NNS  ~ 'shoots' | 'leaves'
VBZ ~ 'eats' | 'shoots' | 'leaves'

END_OF_SOURCE
    }
);

my $expected = <<'EOS';
(S (NP (DT a) (NN panda))
   (VP (VBZ eats) (NP (NNS shoots) (CC and) (NNS leaves)))
   (. .))
(S (NP (DT a) (NN panda))
   (VP (VP (VBZ eats) (NP (NNS shoots))) (CC and) (VP (VBZ leaves)))
   (. .))
(S (NP (DT a) (NN panda))
   (VP (VP (VBZ eats)) (VP (VBZ shoots)) (CC and) (VP (VBZ leaves)))
   (. .))
EOS

my $sentence = <<END_OF_SENTENCE;
a panda eats shoots and leaves.
END_OF_SENTENCE

my @actual = ();

my $recce = Marpa::R2::Scanless::R->new( { grammar => $grammar } );

$recce->read( \$sentence );

while ( defined( my $value_ref = $recce->value() ) ) {
    my $value = $value_ref ? ${$value_ref}->bracket() : 'No parse';
    push @actual, $value;
}

$recce->series_restart();
my $asf = Marpa::R2::ASF->new( { slr=>$recce } );
$asf->traverse( sub { my ($glade) = @_; return $glade->rh_length(); } );

package PennTags;

sub contents {
    join( $_[0], map { $_->bracket() } @{ $_[1] } );
}

sub PennTags::S::bracket { "(S " . contents( "\n   ", $_[0] ) . ")" }
sub PennTags::NP::bracket { "(NP " . contents( ' ', $_[0] ) . ")" }
sub PennTags::VP::bracket { "(VP " . contents( ' ', $_[0] ) . ")" }
sub PennTags::PP::bracket { "(PP " . contents( ' ', $_[0] ) . ")" }

sub PennTags::CC::bracket  {"(CC $_[0]->[0])"}
sub PennTags::DT::bracket  {"(DT $_[0]->[0])"}
sub PennTags::IN::bracket  {"(IN $_[0]->[0])"}
sub PennTags::NN::bracket  {"(NN $_[0]->[0])"}
sub PennTags::NNS::bracket {"(NNS $_[0]->[0])"}
sub PennTags::VB::bracket  {"(VB $_[0]->[0])"}
sub PennTags::VBP::bracket {"(VBP $_[0]->[0])"}
sub PennTags::VBZ::bracket {"(VBZ $_[0]->[0])"}

sub PennTags::period::bracket {"(. .)"}

package main;

Marpa::R2::Test::is( ( join "\n", sort @actual ) . "\n",
    $expected, 'Ambiguous English sentences' );

1;    # In case used as "do" file
