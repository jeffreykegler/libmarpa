# Copyright 2012 Jeffrey Kegler
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

package Marpa::R2::Grammar;

use 5.010;

use warnings;

# There's a problem with this perlcritic check
# as of 9 Aug 2010
## no critic (TestingAndDebugging::ProhibitNoWarnings)
no warnings qw(recursion qw);
## use critic

use strict;

use vars qw($VERSION $STRING_VERSION);
$VERSION        = '2.023_010';
$STRING_VERSION = $VERSION;
## no critic(BuiltinFunctions::ProhibitStringyEval)
$VERSION = eval $VERSION;
## use critic

# This structure could be eliminated, and doing so
# would be more efficient, but it is part of the
# external interface.  So this is a stub.
BEGIN {
    my $structure = <<'END_OF_STRUCTURE';
    :package=Marpa::R2::Internal::Symbol
    ID { Unique ID }
END_OF_STRUCTURE
    Marpa::R2::offset($structure);
} ## end BEGIN

BEGIN {
    my $structure = <<'END_OF_STRUCTURE';

    :package=Marpa::R2::Internal::Rule

    ID
    NAME
    DISCARD_SEPARATION
    MASK { Semantic mask of RHS symbols }

END_OF_STRUCTURE
    Marpa::R2::offset($structure);
} ## end BEGIN

BEGIN {
    my $structure = <<'END_OF_STRUCTURE';

    :package=Marpa::R2::Internal::Grammar

    C { A C structure }
    TRACER { Also contains a copy of the C structure.
       It is used frequently, so that an easy memoization
       is probably worthwhile to save a the extra
       indirection.
    }
    RULES { array of rule refs }
    SYMBOLS { array of symbol refs }
    ACTIONS { Default package in which to find actions }
    DEFAULT_ACTION { Action for rules without one }
    TRACE_FILE_HANDLE
    WARNINGS { print warnings about grammar? }
    RULE_NAME_REQUIRED
    RULE_BY_NAME
    INTERFACE { currently 'standard' or 'stuifzand' }

    =LAST_BASIC_DATA_FIELD

    { === Evaluator Fields === }

    DEFAULT_EMPTY_ACTION { default value for empty rules }
    ACTION_OBJECT
    INFINITE_ACTION

    =LAST_EVALUATOR_FIELD

    PROBLEMS { fatal problems }

    =LAST_RECOGNIZER_FIELD

    START_NAME { name of original symbol }
    INACCESSIBLE_OK
    UNPRODUCTIVE_OK
    TRACE_RULES

    =LAST_FIELD

END_OF_STRUCTURE
    Marpa::R2::offset($structure);
} ## end BEGIN

package Marpa::R2::Internal::Grammar;

use English qw( -no_match_vars );

use Marpa::R2::Thin::Trace;

our %DEFAULT_SYMBOLS_RESERVED;
%DEFAULT_SYMBOLS_RESERVED = map { ($_, 1) } split //xms, '}]>)';

sub Marpa::R2::uncaught_error {
    my ($error) = @_;

    # This would be Carp::confess, but in the testing
    # the stack trace includes the hoped for error
    # message, which causes spurious success reports.
    Carp::croak( "libmarpa reported an error which Marpa::R2 did not catch\n",
        $error );
} ## end sub Marpa::R2::uncaught_error

package Marpa::R2::Internal::Grammar;

sub Marpa::R2::Grammar::new {
    my ( $class, @arg_hashes ) = @_;

    my $grammar = [];
    bless $grammar, $class;

    # set the defaults and the default defaults
    $grammar->[Marpa::R2::Internal::Grammar::TRACE_FILE_HANDLE] = *STDERR;

    $grammar->[Marpa::R2::Internal::Grammar::TRACE_RULES]     = 0;
    $grammar->[Marpa::R2::Internal::Grammar::WARNINGS]        = 1;
    $grammar->[Marpa::R2::Internal::Grammar::INACCESSIBLE_OK] = {};
    $grammar->[Marpa::R2::Internal::Grammar::UNPRODUCTIVE_OK] = {};
    $grammar->[Marpa::R2::Internal::Grammar::INFINITE_ACTION] = 'fatal';

    $grammar->[Marpa::R2::Internal::Grammar::SYMBOLS]            = [];
    $grammar->[Marpa::R2::Internal::Grammar::RULES]              = [];
    $grammar->[Marpa::R2::Internal::Grammar::RULE_BY_NAME]       = {};
    $grammar->[Marpa::R2::Internal::Grammar::RULE_NAME_REQUIRED] = 0;

    my $grammar_c = $grammar->[Marpa::R2::Internal::Grammar::C] =
        Marpa::R2::Thin::G->new( { if => 1 } );
    $grammar->[Marpa::R2::Internal::Grammar::TRACER] =
        Marpa::R2::Thin::Trace->new($grammar_c);

    $grammar->set(@arg_hashes);

    return $grammar;
} ## end sub Marpa::R2::Grammar::new

sub Marpa::R2::Grammar::thin {
    return $_[0]->[Marpa::R2::Internal::Grammar::C];
}

sub Marpa::R2::Grammar::thin_symbol {
    my ( $grammar, $symbol_name ) = @_;
    return $grammar->[Marpa::R2::Internal::Grammar::TRACER]
        ->symbol_by_name($symbol_name);
}

use constant GRAMMAR_OPTIONS => [
    qw{
        action_object
        actions
        infinite_action
        default_action
        default_empty_action
        default_rank
        inaccessible_ok
        rule_name_required
        rules
        start
        symbols
        terminals
        trace_file_handle
        unproductive_ok
        warnings
        }
];

sub Marpa::R2::Grammar::set {
    my ( $grammar, @arg_hashes ) = @_;

    # set trace_fh even if no tracing, because we may turn it on in this method
    my $trace_fh =
        $grammar->[Marpa::R2::Internal::Grammar::TRACE_FILE_HANDLE];
    my $grammar_c = $grammar->[Marpa::R2::Internal::Grammar::C];

    for my $args (@arg_hashes) {

        my $ref_type = ref $args;
        if ( not $ref_type ) {
            Carp::croak(
                'Marpa::R2::Grammar expects args as ref to HASH; arg was non-reference'
            );
        }
        if ( $ref_type ne 'HASH' ) {
            Carp::croak(
                "Marpa::R2::Grammar expects args as ref to HASH, got ref to $ref_type instead"
            );
        }
        if (my @bad_options =
            grep { not $_ ~~ Marpa::R2::Internal::Grammar::GRAMMAR_OPTIONS }
            keys %{$args}
            )
        {
            Carp::croak( 'Unknown option(s) for Marpa::R2::Grammar: ',
                join q{ }, @bad_options );
        } ## end if ( my @bad_options = grep { not $_ ~~ ...})

        # First pass options: These affect processing of other
        # options and are expected to take force for the other
        # options, even if specified afterwards

        if ( defined( my $value = $args->{'trace_file_handle'} ) ) {
            $trace_fh =
                $grammar->[Marpa::R2::Internal::Grammar::TRACE_FILE_HANDLE] =
                $value;
        }

        if ( defined( my $value = $args->{'default_rank'} ) ) {
            Marpa::R2::exception(
                'default_rank option not allowed after grammar is precomputed'
            ) if $grammar_c->is_precomputed();
            $grammar_c->default_rank_set($value);
        } ## end if ( defined( my $value = $args->{'default_rank'} ) )

        # Second pass options

        if ( defined( my $value = $args->{'symbols'} ) ) {
            Marpa::R2::exception(
                'symbols option not allowed after grammar is precomputed')
                if $grammar_c->is_precomputed();
            Marpa::R2::exception('symbols value must be REF to HASH')
                if ref $value ne 'HASH';
            while ( my ( $symbol, $properties ) = each %{$value} ) {
                assign_user_symbol( $grammar, $symbol, $properties );
            }
        } ## end if ( defined( my $value = $args->{'symbols'} ) )

        if ( defined( my $value = $args->{'rule_name_required'} ) ) {
            $grammar->[Marpa::R2::Internal::Grammar::RULE_NAME_REQUIRED] =
                !!$value;
        }

        if ( defined( my $value = $args->{'terminals'} ) ) {
            Marpa::R2::exception(
                'terminals option not allowed after grammar is precomputed')
                if $grammar_c->is_precomputed();
            Marpa::R2::exception('terminals value must be REF to ARRAY')
                if ref $value ne 'ARRAY';
            for my $symbol ( @{$value} ) {
                assign_user_symbol( $grammar, $symbol, { terminal => 1 } );
            }
        } ## end if ( defined( my $value = $args->{'terminals'} ) )

        if ( defined( my $value = $args->{'start'} ) ) {
            Marpa::R2::exception(
                'start option not allowed after grammar is precomputed')
                if $grammar_c->is_precomputed();
            $grammar->[Marpa::R2::Internal::Grammar::START_NAME] = $value;
        } ## end if ( defined( my $value = $args->{'start'} ) )

        if ( defined( my $value = $args->{'rules'} ) ) {
            Marpa::R2::exception(
                'rules option not allowed after grammar is precomputed')
                if $grammar_c->is_precomputed();
            DO_RULES: {
                ## Allow this via a hack for new
                ## Eventually deprecate and eliminate it
                if (    ref $value eq 'ARRAY'
                    and scalar @{$value} == 1
                    and not ref $value->[0] )
                {
                    $value = $value->[0];
                } ## end if ( ref $value eq 'ARRAY' and scalar @{$value} == 1...)
                if ( not ref $value ) {
                    $grammar->[Marpa::R2::Internal::Grammar::INTERFACE] //=
                        'stuifzand';
                    Marpa::R2::exception(
                        qq{Attempt to use the BNF interface with a grammar that is already using the standard interface\n},
                        qq{  Mixing the BNF and standard interface is not allowed},
                        )
                        if $grammar->[Marpa::R2::Internal::Grammar::INTERFACE]
                            ne 'stuifzand';
                    for my $rule (
                        @{  Marpa::R2::Internal::Stuifzand::parse_rules(
                                $value)
                        }
                        )
                    {
                        add_user_rule( $grammar, $rule );
                    } ## end for my $rule ( @{ ...})
                    last DO_RULES;
                } ## end if ( not ref $value )
                Marpa::R2::exception(
                    qq{"rules" named argument must be string or ref to ARRAY}
                ) if ref $value ne 'ARRAY';
                $grammar->[Marpa::R2::Internal::Grammar::INTERFACE] //=
                    'standard';
                Marpa::R2::exception(
                    qq{Attempt to use the standard interface with a grammar that is already using the BNF interface\n},
                    qq{  Mixing the BNF and standard interface is not allowed}
                    )
                    if $grammar->[Marpa::R2::Internal::Grammar::INTERFACE] ne
                        'standard';
                add_user_rules( $grammar, $value );
            } ## end DO_RULES:
        } ## end if ( defined( my $value = $args->{'rules'} ) )

        if ( exists $args->{'default_empty_action'} ) {
            my $value = $args->{'default_empty_action'};
            $grammar->[Marpa::R2::Internal::Grammar::DEFAULT_EMPTY_ACTION] =
                $value;
        }

        if ( defined( my $value = $args->{'actions'} ) ) {
            $grammar->[Marpa::R2::Internal::Grammar::ACTIONS] = $value;
        }

        if ( defined( my $value = $args->{'action_object'} ) ) {
            $grammar->[Marpa::R2::Internal::Grammar::ACTION_OBJECT] = $value;
        }

        if ( defined( my $value = $args->{'default_action'} ) ) {
            $grammar->[Marpa::R2::Internal::Grammar::DEFAULT_ACTION] = $value;
        }

        if ( defined( my $value = $args->{'infinite_action'} ) ) {
            if ( $value && $grammar_c->is_precomputed() ) {
                say {$trace_fh}
                    '"infinite_action" option is useless after grammar is precomputed'
                    or Marpa::R2::exception("Could not print: $ERRNO");
            }
            Marpa::R2::exception(
                q{infinite_action must be 'warn', 'quiet' or 'fatal'})
                if not $value ~~ [qw(warn quiet fatal)];
            $grammar->[Marpa::R2::Internal::Grammar::INFINITE_ACTION] =
                $value;
        } ## end if ( defined( my $value = $args->{'infinite_action'}...))

        if ( defined( my $value = $args->{'warnings'} ) ) {
            if ( $value && $grammar_c->is_precomputed() ) {
                say {$trace_fh}
                    q{"warnings" option is useless after grammar is precomputed}
                    or Marpa::R2::exception("Could not print: $ERRNO");
            }
            $grammar->[Marpa::R2::Internal::Grammar::WARNINGS] = $value;
        } ## end if ( defined( my $value = $args->{'warnings'} ) )

        if ( defined( my $value = $args->{'inaccessible_ok'} ) ) {
            if ( $value && $grammar_c->is_precomputed() ) {
                say {$trace_fh}
                    q{"inaccessible_ok" option is useless after grammar is precomputed}
                    or Marpa::R2::exception("Could not print: $ERRNO");

            } ## end if ( $value && $grammar_c->is_precomputed() )
            GIVEN_REF_VALUE: {
                my $ref_value = ref $value;
                if ( $ref_value eq q{} ) {
                    $value //= {};
                    last GIVEN_REF_VALUE;
                }
                if ( $ref_value eq 'ARRAY' ) {
                    $value = { map { ( $_, 1 ) } @{$value} };
                    last GIVEN_REF_VALUE;
                }
                Marpa::R2::exception(
                    'value of inaccessible_ok option must be boolean or an array ref'
                );
            } ## end GIVEN_REF_VALUE:
            $grammar->[Marpa::R2::Internal::Grammar::INACCESSIBLE_OK] =
                $value;
        } ## end if ( defined( my $value = $args->{'inaccessible_ok'}...))

        if ( defined( my $value = $args->{'unproductive_ok'} ) ) {
            if ( $value && $grammar_c->is_precomputed() ) {
                say {$trace_fh}
                    q{"unproductive_ok" option is useless after grammar is precomputed}
                    or Marpa::R2::exception("Could not print: $ERRNO");
            }
            GIVEN_REF_VALUE: {
                my $ref_value = ref $value;
                if ( $ref_value eq q{} ) {
                    $value //= {};
                    last GIVEN_REF_VALUE;
                }
                if ( $ref_value eq 'ARRAY' ) {
                    $value = { map { ( $_, 1 ) } @{$value} };
                    last GIVEN_REF_VALUE;
                }
                Marpa::R2::exception(
                        'value of unproductive_ok option must be boolean or an array ref'
                );
            } ## end GIVEN_REF_VALUE:
            $grammar->[Marpa::R2::Internal::Grammar::UNPRODUCTIVE_OK] =
                $value;
        } ## end if ( defined( my $value = $args->{'unproductive_ok'}...))

    } ## end for my $args (@arg_hashes)

    return 1;
} ## end sub Marpa::R2::Grammar::set

sub Marpa::R2::Grammar::symbol_reserved_set {
    my ( $grammar, $final_character, $boolean ) = @_;
    if ( length $final_character != 1 ) {
        Marpa::R2::exception( 'symbol_reserved_set(): "',
            $final_character, '" is not a symbol' );
    }
    if ( $final_character eq ']' ) {
        return if $boolean;
        Marpa::R2::exception(
            q{symbol_reserved_set(): Attempt to unreserve ']'; this is not allowed}
        );
    } ## end if ( $final_character eq ']' ) ([)
    if ( not exists $DEFAULT_SYMBOLS_RESERVED{$final_character} ) {
        Marpa::R2::exception(
            qq{symbol_reserved_set(): "$final_character" is not a reservable symbol}
        );
    }
    # Return a value to make perlcritic happy
    return $DEFAULT_SYMBOLS_RESERVED{$final_character} = $boolean ? 1 : 0;
} ## end sub Marpa::R2::Grammar::symbol_reserved_set

sub Marpa::R2::Grammar::precompute {
    my $grammar = shift;

    my $rules     = $grammar->[Marpa::R2::Internal::Grammar::RULES];
    my $grammar_c = $grammar->[Marpa::R2::Internal::Grammar::C];
    my $trace_fh =
        $grammar->[Marpa::R2::Internal::Grammar::TRACE_FILE_HANDLE];

    my $problems = $grammar->[Marpa::R2::Internal::Grammar::PROBLEMS];
    if ($problems) {
        Marpa::R2::exception(
            Marpa::R2::Grammar::show_problems($grammar),
            "Second attempt to precompute grammar with fatal problems\n",
            'Marpa::R2 cannot proceed'
        );
    } ## end if ($problems)

    return $grammar if $grammar_c->is_precomputed();

    set_start_symbol($grammar);

    # Catch errors in precomputation
    my $precompute_error_code = $Marpa::R2::Error::NONE;
    $grammar_c->throw_set(0);
    my $precompute_result = $grammar_c->precompute();
    $grammar_c->throw_set(1);

    if ( $precompute_result < 0 ) {
        ($precompute_error_code) = $grammar_c->error();
        if ( not defined $precompute_error_code ) {
            Marpa::R2::exception(
                'libmarpa error, but no error code returned');
        }

        # If already precomputed, just return success
        return $grammar
            if $precompute_error_code == $Marpa::R2::Error::PRECOMPUTED;

        # Cycles are not necessarily errors,
        # and get special handling
        $precompute_error_code = $Marpa::R2::Error::NONE
            if $precompute_error_code == $Marpa::R2::Error::GRAMMAR_HAS_CYCLE;

    } ## end if ( $precompute_result < 0 )

    if ( $precompute_error_code != $Marpa::R2::Error::NONE ) {

        # Report the errors, then return failure

        if ( $precompute_error_code == $Marpa::R2::Error::NO_RULES ) {
            Marpa::R2::exception(
                'Attempted to precompute grammar with no rules');
        }
        if ( $precompute_error_code == $Marpa::R2::Error::NULLING_TERMINAL ) {
            my @nulling_terminals = ();
            my $event_count       = $grammar_c->event_count();
            EVENT:
            for ( my $event_ix = 0; $event_ix < $event_count; $event_ix++ ) {
                my ( $event_type, $value ) = $grammar_c->event($event_ix);
                if ( $event_type eq 'MARPA_EVENT_NULLING_TERMINAL' ) {
                    push @nulling_terminals, $grammar->symbol_name($value);
                }
            } ## end EVENT: for ( my $event_ix = 0; $event_ix < $event_count; ...)
            my @nulling_terminal_messages =
                map {qq{Nulling symbol "$_" is also a terminal\n}}
                @nulling_terminals;
            Marpa::R2::exception( @nulling_terminal_messages,
                'A terminal symbol cannot also be a nulling symbol' );
        } ## end if ( $precompute_error_code == ...)
        if ( $precompute_error_code == $Marpa::R2::Error::COUNTED_NULLABLE ) {
            my @counted_nullables = ();
            my $event_count       = $grammar_c->event_count();
            EVENT:
            for ( my $event_ix = 0; $event_ix < $event_count; $event_ix++ ) {
                my ( $event_type, $value ) = $grammar_c->event($event_ix);
                if ( $event_type eq 'MARPA_EVENT_COUNTED_NULLABLE' ) {
                    push @counted_nullables, $grammar->symbol_name($value);
                }
            } ## end EVENT: for ( my $event_ix = 0; $event_ix < $event_count; ...)
            my @counted_nullable_messages = map {
                      q{Nullable symbol "}
                    . $_
                    . qq{" is on rhs of counted rule\n}
            } @counted_nullables;
            Marpa::R2::exception( @counted_nullable_messages,
                'Counted nullables confuse Marpa -- please rewrite the grammar'
            );
        } ## end if ( $precompute_error_code == ...)

        if ( $precompute_error_code == $Marpa::R2::Error::NO_START_SYMBOL ) {
            Marpa::R2::exception('No start symbol');
        }
        if ( $precompute_error_code == $Marpa::R2::Error::START_NOT_LHS ) {
            my $name = $grammar->[Marpa::R2::Internal::Grammar::START_NAME];
            Marpa::R2::exception(
                qq{Start symbol "$name" not on LHS of any rule});
        }
        if ( $precompute_error_code == $Marpa::R2::Error::UNPRODUCTIVE_START )
        {
            my $name = $grammar->[Marpa::R2::Internal::Grammar::START_NAME];
            Marpa::R2::exception(qq{Unproductive start symbol: "$name"});
        }

        Marpa::R2::uncaught_error( scalar $grammar_c->error() );

    } ## end if ( $precompute_error_code != $Marpa::R2::Error::NONE)

    # Shadow all the new rules
    {
        my $highest_rule_id = $grammar_c->highest_rule_id();
        RULE:
        for ( my $rule_id = 0; $rule_id <= $highest_rule_id; $rule_id++ ) {
            next RULE if defined $rules->[$rule_id];

            # The Marpa::R2 logic assumes no "gaps" in the rule numbering,
            # which is currently the case for Libmarpa,
            # but not guaranteed.
            shadow_rule( $grammar, $rule_id );
        } ## end RULE: for ( my $rule_id = 0; $rule_id <= $highest_rule_id; ...)
    }

    my $infinite_action =
        $grammar->[Marpa::R2::Internal::Grammar::INFINITE_ACTION];

    # Above I went through the error events
    # Here I go through the events for situations where there was no
    # hard error returned from libmarpa
    my $loop_rule_count = 0;
    {
        my $event_count = $grammar_c->event_count();
        EVENT:
        for ( my $event_ix = 0; $event_ix < $event_count; $event_ix++ ) {
            my ( $event_type, $value ) = $grammar_c->event($event_ix);
            if ( $event_type ne 'MARPA_EVENT_LOOP_RULES' ) {
                Marpa::R2::exception(
                    qq{Unknown grammar precomputation event; type="$event_type"}
                );
            }
            $loop_rule_count = $value;
        } ## end EVENT: for ( my $event_ix = 0; $event_ix < $event_count; ...)
    }

    if ( $loop_rule_count and $infinite_action ne 'quiet' ) {
        my @loop_rules =
            grep { $grammar_c->rule_is_loop($_) } ( 0 .. $#{$rules} );
        for my $rule_id (@loop_rules) {
            print {$trace_fh}
                'Cycle found involving rule: ',
                $grammar->brief_rule($rule_id), "\n"
                or Marpa::R2::exception("Could not print: $ERRNO");
        } ## end for my $rule_id (@loop_rules)
        Marpa::R2::exception('Cycles in grammar, fatal error')
            if $infinite_action eq 'fatal';
    } ## end if ( $loop_rule_count and $infinite_action ne 'quiet')

    # A bit hackish here: INACCESSIBLE_OK is not a HASH ref iff
    # it is a Boolean TRUE indicating that all inaccessibles are OK.
    # A Boolean FALSE will have been replaced with an empty hash.
    if ($grammar->[Marpa::R2::Internal::Grammar::WARNINGS]
        and ref(
            my $ok = $grammar->[Marpa::R2::Internal::Grammar::INACCESSIBLE_OK]
        ) eq 'HASH'
        )
    {
        SYMBOL:
        for my $symbol (
            @{ Marpa::R2::Grammar::inaccessible_symbols($grammar) } )
        {

            # Inaccessible internal symbols may be created
            # from inaccessible use symbols -- ignore these.
            # This assumes that Marpa's logic
            # is correct and that
            # it is not creating inaccessible symbols from
            # accessible ones.
            next SYMBOL if $symbol =~ /\]/xms;
            next SYMBOL if $ok->{$symbol};
            say {$trace_fh} "Inaccessible symbol: $symbol"
                or Marpa::R2::exception("Could not print: $ERRNO");
        } ## end SYMBOL: for my $symbol ( @{ ...})
    } ## end if ( $grammar->[Marpa::R2::Internal::Grammar::WARNINGS...])

    # A bit hackish here: UNPRODUCTIVE_OK is not a HASH ref iff
    # it is a Boolean TRUE indicating that all inaccessibles are OK.
    # A Boolean FALSE will have been replaced with an empty hash.
    if ($grammar->[Marpa::R2::Internal::Grammar::WARNINGS]
        and ref(
            my $ok = $grammar->[Marpa::R2::Internal::Grammar::UNPRODUCTIVE_OK]
        ) eq 'HASH'
        )
    {
        SYMBOL:
        for my $symbol (
            @{ Marpa::R2::Grammar::unproductive_symbols($grammar) } )
        {

            # Unproductive internal symbols may be created
            # from unproductive use symbols -- ignore these.
            # This assumes that Marpa's logic
            # is correct and that
            # it is not creating unproductive symbols from
            # productive ones.
            next SYMBOL if $symbol =~ /\]/xms;
            next SYMBOL if $ok->{$symbol};
            say {$trace_fh} "Unproductive symbol: $symbol"
                or Marpa::R2::exception("Could not print: $ERRNO");
        } ## end SYMBOL: for my $symbol ( @{ ...})
    } ## end if ( $grammar->[Marpa::R2::Internal::Grammar::WARNINGS...])

    return $grammar;

} ## end sub Marpa::R2::Grammar::precompute

sub Marpa::R2::Grammar::rule_by_name {
    my ( $grammar, $name ) = @_;
    return $grammar->[Marpa::R2::Internal::Grammar::RULE_BY_NAME]->{$name};
}

sub Marpa::R2::Grammar::show_problems {
    my ($grammar) = @_;

    my $problems = $grammar->[Marpa::R2::Internal::Grammar::PROBLEMS];
    if ($problems) {
        my $problem_count = scalar @{$problems};
        return
            "Grammar has $problem_count problems:\n"
            . ( join "\n", @{$problems} ) . "\n";
    } ## end if ($problems)
    return "Grammar has no problems\n";
} ## end sub Marpa::R2::Grammar::show_problems

sub Marpa::R2::Grammar::show_symbol {
    my ( $grammar, $symbol ) = @_;
    my $grammar_c = $grammar->[Marpa::R2::Internal::Grammar::C];
    my $text      = q{};
    my $symbol_id = $symbol->[Marpa::R2::Internal::Symbol::ID];

    my $name = $grammar->symbol_name($symbol_id);
    $text .= "$symbol_id: $name";

    my @tag_list = ();
    $grammar_c->symbol_is_productive($symbol_id)
        or push @tag_list, 'unproductive';
    $grammar_c->symbol_is_accessible($symbol_id)
        or push @tag_list, 'inaccessible';
    $grammar_c->symbol_is_nulling($symbol_id)  and push @tag_list, 'nulling';
    $grammar_c->symbol_is_terminal($symbol_id) and push @tag_list, 'terminal';

    $text .= join q{ }, q{,}, @tag_list if scalar @tag_list;
    $text .= "\n";
    return $text;

} ## end sub Marpa::R2::Grammar::show_symbol

sub Marpa::R2::Grammar::show_symbols {
    my ($grammar) = @_;
    my $symbols   = $grammar->[Marpa::R2::Internal::Grammar::SYMBOLS];
    my $text      = q{};
    for my $symbol_ref ( @{$symbols} ) {
        $text .= $grammar->show_symbol($symbol_ref);
    }
    return $text;
} ## end sub Marpa::R2::Grammar::show_symbols

sub Marpa::R2::Grammar::show_nulling_symbols {
    my ($grammar) = @_;
    my $grammar_c = $grammar->[Marpa::R2::Internal::Grammar::C];
    my $symbols   = $grammar->[Marpa::R2::Internal::Grammar::SYMBOLS];
    return join q{ }, sort map { $grammar->symbol_name($_) }
        grep { $grammar_c->symbol_is_nulling($_) } ( 0 .. $#{$symbols} );
} ## end sub Marpa::R2::Grammar::show_nulling_symbols

sub Marpa::R2::Grammar::show_productive_symbols {
    my ($grammar) = @_;
    my $grammar_c = $grammar->[Marpa::R2::Internal::Grammar::C];
    my $symbols   = $grammar->[Marpa::R2::Internal::Grammar::SYMBOLS];
    return join q{ }, sort map { $grammar->symbol_name($_) }
        grep { $grammar_c->symbol_is_productive($_) } ( 0 .. $#{$symbols} );
} ## end sub Marpa::R2::Grammar::show_productive_symbols

sub Marpa::R2::Grammar::show_accessible_symbols {
    my ($grammar) = @_;
    my $grammar_c = $grammar->[Marpa::R2::Internal::Grammar::C];
    my $symbols   = $grammar->[Marpa::R2::Internal::Grammar::SYMBOLS];
    return join q{ }, sort map { $grammar->symbol_name($_) }
        grep { $grammar_c->symbol_is_accessible($_) } ( 0 .. $#{$symbols} );
} ## end sub Marpa::R2::Grammar::show_accessible_symbols

sub Marpa::R2::Grammar::inaccessible_symbols {
    my ($grammar) = @_;
    my $grammar_c = $grammar->[Marpa::R2::Internal::Grammar::C];
    my $symbols   = $grammar->[Marpa::R2::Internal::Grammar::SYMBOLS];
    return [
        sort map { $grammar->symbol_name($_) }
            grep { !$grammar_c->symbol_is_accessible($_) }
            ( 0 .. $#{$symbols} )
    ];
} ## end sub Marpa::R2::Grammar::inaccessible_symbols

sub Marpa::R2::Grammar::unproductive_symbols {
    my ($grammar) = @_;
    my $grammar_c = $grammar->[Marpa::R2::Internal::Grammar::C];
    my $symbols   = $grammar->[Marpa::R2::Internal::Grammar::SYMBOLS];
    return [
        sort map { $grammar->symbol_name($_) }
            grep { !$grammar_c->symbol_is_productive($_) }
            ( 0 .. $#{$symbols} )
    ];
} ## end sub Marpa::R2::Grammar::unproductive_symbols

sub Marpa::R2::Grammar::brief_rule {
    my ( $grammar, $rule_id ) = @_;
    my $grammar_c = $grammar->[Marpa::R2::Internal::Grammar::C];
    my ( $lhs, @rhs ) = $grammar->rule($rule_id);
    my $minimum = $grammar_c->sequence_min($rule_id);
    my $quantifier = defined $minimum ? $minimum <= 0 ? q{*} : q{+} : q{};
    return ( join q{ }, "$rule_id:", $lhs, '->', @rhs ) . $quantifier;
} ## end sub Marpa::R2::Grammar::brief_rule

sub Marpa::R2::Grammar::show_rule {
    my ( $grammar, $rule ) = @_;

    my $grammar_c = $grammar->[Marpa::R2::Internal::Grammar::C];
    my $rule_id   = $rule->[Marpa::R2::Internal::Rule::ID];
    my @comment   = ();

    $grammar_c->rule_length($rule_id) == 0 and push @comment, 'empty';
    $grammar->rule_is_used($rule_id)         or push @comment, '!used';
    $grammar_c->rule_is_productive($rule_id) or push @comment, 'unproductive';
    $grammar_c->rule_is_accessible($rule_id) or push @comment, 'inaccessible';
    $rule->[Marpa::R2::Internal::Rule::DISCARD_SEPARATION]
        and push @comment, 'discard_sep';

    my $text = $grammar->brief_rule($rule_id);

    if (@comment) {
        $text .= q{ } . ( join q{ }, q{/*}, @comment, q{*/} );
    }

    return $text .= "\n";

}    # sub show_rule

sub Marpa::R2::Grammar::show_rules {
    my ($grammar) = @_;
    my $rules = $grammar->[Marpa::R2::Internal::Grammar::RULES];
    my $text;

    for my $rule ( @{$rules} ) {
        $text .= $grammar->show_rule($rule);
    }
    return $text;
} ## end sub Marpa::R2::Grammar::show_rules

# This logic tests for gaps in the rule numbering.
# Currently there are none, but Libmarpa does not
# guarantee this.
sub Marpa::R2::Grammar::rule_ids {
    my ($grammar) = @_;
    my $grammar_c = $grammar->[Marpa::R2::Internal::Grammar::C];
    return
        grep { $grammar_c->rule_length($_); }
        0 .. $grammar_c->highest_rule_id();
} ## end sub Marpa::R2::Grammar::rule_ids

sub Marpa::R2::Grammar::rule {
    my ( $grammar, $rule_id ) = @_;
    my $grammar_c   = $grammar->[Marpa::R2::Internal::Grammar::C];
    my $rule_length = $grammar_c->rule_length($rule_id);
    return if not defined $rule_length;
    my @symbol_ids = ( $grammar_c->rule_lhs($rule_id) );
    push @symbol_ids,
        map { $grammar_c->rule_rhs( $rule_id, $_ ) }
        ( 0 .. $rule_length - 1 );
    my @symbol_names = ();
    for my $symbol_id (@symbol_ids) {
        ## The name of the symbols, before the BNF rewrites
        push @symbol_names,
            Marpa::R2::Grammar::original_symbol_name(
            $grammar->symbol_name($symbol_id) );
    } ## end for my $symbol_id (@symbol_ids)
    return @symbol_names;
} ## end sub Marpa::R2::Grammar::rule

# Deprecated and for removal
# Used in blog post, and part of
# CPAN version 2.023_008 but
# never documented in any CPAN version
sub Marpa::R2::Grammar::bnf_rule {
    goto &Marpa::R2::Grammar::rule;
} ## end sub Marpa::R2::Grammar::bnf_rule

sub Marpa::R2::Grammar::show_dotted_rule {
    my ( $grammar, $rule_id, $dot_position ) = @_;
    my $grammar_c = $grammar->[Marpa::R2::Internal::Grammar::C];
    my ( $lhs, @rhs ) = $grammar->rule($rule_id);

    my $minimum = $grammar_c->sequence_min($rule_id);
    if (defined $minimum) {
        my $quantifier = $minimum <= 0 ? q{*} : q{+} ;
        $rhs[0] .= $quantifier;
    }
    $dot_position = 0 if $dot_position < 0;
    splice @rhs, $dot_position, 0, q{.};
    return join q{ }, $lhs, q{->}, @rhs;
} ## end sub Marpa::R2::Grammar::show_dotted_rule

# Used by lexers to check that symbol is a terminal
sub Marpa::R2::Grammar::check_terminal {
    my ( $grammar, $name ) = @_;
    return 0 if not defined $name;
    my $grammar_c = $grammar->[Marpa::R2::Internal::Grammar::C];
    my $symbol_id =
        $grammar->[Marpa::R2::Internal::Grammar::TRACER]
        ->symbol_by_name($name);
    return 0 if not defined $symbol_id;
    my $symbols = $grammar->[Marpa::R2::Internal::Grammar::SYMBOLS];
    my $symbol  = $symbols->[$symbol_id];
    return $grammar_c->symbol_is_terminal($symbol_id) ? 1 : 0;
} ## end sub Marpa::R2::Grammar::check_terminal

sub Marpa::R2::Grammar::symbol_name {
    my ( $grammar, $id ) = @_;
    my $symbol_name =
        $grammar->[Marpa::R2::Internal::Grammar::TRACER]->symbol_name($id);
    return defined $symbol_name ? $symbol_name : '[SYMBOL' . $id . ']';
} ## end sub Marpa::R2::Grammar::symbol_name

sub shadow_symbol {
    my ( $grammar, $symbol_id ) = @_;
    my $symbols = $grammar->[Marpa::R2::Internal::Grammar::SYMBOLS];
    my $symbol = $symbols->[$symbol_id] = [];
    $symbol->[Marpa::R2::Internal::Symbol::ID] = $symbol_id;
    return $symbol;
} ## end sub shadow_symbol

# Create the structure which "shadows" the libmarpa rule
sub shadow_rule {
    my ( $grammar, $rule_id ) = @_;
    my $rules = $grammar->[Marpa::R2::Internal::Grammar::RULES];
    my $new_rule = $rules->[$rule_id] = [];
    $new_rule->[Marpa::R2::Internal::Rule::ID] = $rule_id;
    return $new_rule;
} ## end sub shadow_rule

sub assign_symbol {
    my ( $grammar, $name ) = @_;

    my $grammar_c = $grammar->[Marpa::R2::Internal::Grammar::C];
    my $tracer    = $grammar->[Marpa::R2::Internal::Grammar::TRACER];
    my $symbol_id = $tracer->symbol_by_name($name);
    if ( defined $symbol_id ) {
        my $symbols = $grammar->[Marpa::R2::Internal::Grammar::SYMBOLS];
        return $symbols->[$symbol_id];
    }
    $symbol_id = $tracer->symbol_new($name);
    return shadow_symbol( $grammar, $symbol_id );
} ## end sub assign_symbol

sub assign_user_symbol {
    my $grammar   = shift;
    my $name      = shift;
    my $options   = shift;
    my $grammar_c = $grammar->[Marpa::R2::Internal::Grammar::C];

    if ( my $type = ref $name ) {
        Marpa::R2::exception(
            "Symbol name was ref to $type; it must be a scalar string");
    }
    my $final_symbol = substr $name, -1;
    if ( $DEFAULT_SYMBOLS_RESERVED{$final_symbol} ) {
        Marpa::R2::exception(
            qq{Symbol name $name ends in "$final_symbol": that's not allowed}
        );
    }
    my $symbol = assign_symbol( $grammar, $name );
    my $symbol_id = $symbol->[Marpa::R2::Internal::Symbol::ID];

    PROPERTY: while ( my ( $property, $value ) = each %{$options} ) {
        if ( not $property ~~ [qw(terminal rank )] ) {
            Marpa::R2::exception(qq{Unknown symbol property "$property"});
        }
        if ( $property eq 'terminal' ) {
            $grammar_c->symbol_is_terminal_set( $symbol_id, $value );
        }
        if ( $property eq 'rank' ) {
            Marpa::R2::exception(qq{Symbol "$name": rank must be an integer})
                if not Scalar::Util::looks_like_number($value)
                    or int($value) != $value;
            $grammar_c->symbol_rank_set($symbol_id) = $value;
        } ## end if ( $property eq 'rank' )
    } ## end PROPERTY: while ( my ( $property, $value ) = each %{$options} )

    return $symbol;

} ## end sub assign_user_symbol

# add one or more rules
sub add_user_rules {
    my ( $grammar, $rules ) = @_;

    my @hash_rules = ();
    RULE: for my $rule ( @{$rules} ) {

        # Translate other rule formats into hash rules
        my $ref_rule = ref $rule;
        if ( $ref_rule eq 'HASH' ) {
            push @hash_rules, $rule;
            next RULE;
        }
        if ( $ref_rule eq 'ARRAY' ) {
            my $arg_count = @{$rule};

            if ( $arg_count > 4 or $arg_count < 1 ) {
                Marpa::R2::exception(
                    "Rule has $arg_count arguments: "
                        . join( ', ',
                        map { defined $_ ? $_ : 'undef' } @{$rule} )
                        . "\n"
                        . 'Rule must have from 1 to 4 arguments'
                );
            } ## end if ( $arg_count > 4 or $arg_count < 1 )
            my ( $lhs, $rhs, $action ) = @{$rule};
            push @hash_rules,
                {
                lhs           => $lhs,
                rhs           => $rhs,
                action        => $action,
                };
            next RULE;
        } ## end if ( $ref_rule eq 'ARRAY' )
        Marpa::R2::exception(
            'Invalid rule: ',
            Data::Dumper->new( [$rule], ['Invalid_Rule'] )->Indent(2)
                ->Terse(1)->Maxdepth(2)->Dump,
            'Rule must be ref to HASH or ARRAY'
        );

    }    # RULE

    for my $hash_rule (@hash_rules) {
        add_user_rule( $grammar, $hash_rule );
    }

    return;

} ## end sub add_user_rules

sub add_user_rule {
    my ( $grammar, $options ) = @_;

    Marpa::R2::exception('Missing argument to add_user_rule')
        if not defined $grammar
            or not defined $options;

    my $grammar_c    = $grammar->[Marpa::R2::Internal::Grammar::C];
    my $tracer    = $grammar->[Marpa::R2::Internal::Grammar::TRACER];
    my $rules        = $grammar->[Marpa::R2::Internal::Grammar::RULES];
    my $default_rank = $grammar_c->default_rank();

    my ( $lhs_name, $rhs_names, $action );
    my ( $min, $separator_name );
    my $rank;
    my $null_ranking;
    my $rule_name;
    my $mask;
    my $proper_separation = 0;
    my $keep_separation   = 0;

    OPTION: while ( my ( $option, $value ) = each %{$options} ) {
        if ( $option eq 'name' )   { $rule_name = $value; next OPTION; }
        if ( $option eq 'rhs' )    { $rhs_names = $value; next OPTION }
        if ( $option eq 'lhs' )    { $lhs_name  = $value; next OPTION }
        if ( $option eq 'action' ) { $action    = $value; next OPTION }
        if ( $option eq 'rank' )   { $rank      = $value; next OPTION }
        if ( $option eq 'null_ranking' ) {
            $null_ranking = $value;
            next OPTION;
        }
        if ( $option eq 'min' ) { $min = $value; next OPTION }
        if ( $option eq 'separator' ) {
            $separator_name = $value;
            next OPTION;
        }
        if ( $option eq 'proper' ) {
            $proper_separation = $value;
            next OPTION;
        }
        if ( $option eq 'keep' ) { $keep_separation = $value; next OPTION }
        if ( $option eq 'mask' ) { $mask = $value; next OPTION }
        Marpa::R2::exception("Unknown user rule option: $option");
    } ## end OPTION: while ( my ( $option, $value ) = each %{$options} )

    if ( defined $min and not Scalar::Util::looks_like_number($min) ) {
        Marpa::R2::exception(
            q{"min" must be undefined or a valid Perl number});
    }
    my $stuifzand_interface = 
        $grammar->[Marpa::R2::Internal::Grammar::INTERFACE] eq 'stuifzand';

    my $lhs =
        $stuifzand_interface
        ? assign_symbol( $grammar, $lhs_name )
        : assign_user_symbol( $grammar, $lhs_name );
    $rhs_names //= [];

    my @rule_problems = ();

    my $rhs_ref_type = ref $rhs_names;
    if ( not $rhs_ref_type or $rhs_ref_type ne 'ARRAY' ) {
        my $problem =
              "RHS is not ref to ARRAY\n"
            . '  Type of rhs is '
            . ( $rhs_ref_type ? $rhs_ref_type : 'not a ref' ) . "\n";
        my $d = Data::Dumper->new( [$rhs_names], ['rhs'] );
        $problem .= $d->Dump();
        push @rule_problems, $problem;
    } ## end if ( not $rhs_ref_type or $rhs_ref_type ne 'ARRAY' )
    if ( not defined $lhs_name ) {
        push @rule_problems, "Missing LHS\n";
    }

    if ( defined $rank
        and
        ( not Scalar::Util::looks_like_number($rank) or int($rank) != $rank )
        )
    {
        push @rule_problems, "Rank must be undefined or an integer\n";
    } ## end if ( defined $rank and ( not Scalar::Util::looks_like_number...))
    $rank //= $default_rank;

    $null_ranking //= 'low';
    if ( $null_ranking ne 'high' and $null_ranking ne 'low' ) {
        push @rule_problems,
            "Null Ranking must be undefined, 'high' or 'low'\n";
    }

    # Determine the rule's name
    my $rules_by_name =
        $grammar->[Marpa::R2::Internal::Grammar::RULE_BY_NAME];
    if ( defined $rule_name and defined $rules_by_name->{$rule_name} ) {
        push @rule_problems, qq{rule named "$rule_name" already exists};
    }
    if ( !defined $rule_name
        and $grammar->[Marpa::R2::Internal::Grammar::RULE_NAME_REQUIRED] )
    {
        $rule_name =
            $grammar->symbol_name( $lhs->[Marpa::R2::Internal::Symbol::ID] );
        if ( defined $rules_by_name->{$rule_name} ) {
            push @rule_problems,
                qq{Cannot name rule from LHS; rule named "$rule_name" already exists};
        }
    } ## end if ( !defined $rule_name and $grammar->[...])

    if ( scalar @rule_problems ) {
        my %dump_options = %{$options};
        delete $dump_options{grammar};
        my $msg = ( scalar @rule_problems )
            . " problem(s) in the following rule:\n";
        my $d = Data::Dumper->new( [ \%dump_options ], ['rule'] );
        $msg .= $d->Dump();
        for my $problem_number ( 0 .. $#rule_problems ) {
            $msg
                .= 'Problem '
                . ( $problem_number + 1 ) . q{: }
                . $rule_problems[$problem_number] . "\n";
        } ## end for my $problem_number ( 0 .. $#rule_problems )
        Marpa::R2::exception($msg);
    } ## end if ( scalar @rule_problems )

    my $rhs = [
        map {
            $stuifzand_interface
                ? assign_symbol( $grammar, $_ )
                : assign_user_symbol( $grammar, $_ );
        } @{$rhs_names}
    ];

    # Is this is an ordinary, non-counted rule?
    my $is_ordinary_rule = scalar @{$rhs_names} == 0 || !defined $min;
    if ( defined $separator_name and $is_ordinary_rule ) {
        if ( defined $separator_name ) {
            Marpa::R2::exception(
                'separator defined for rule without repetitions');
        }
    } ## end if ( defined $separator_name and $is_ordinary_rule )

    my @rhs_ids = map { $_->[Marpa::R2::Internal::Symbol::ID] } @{$rhs};
    my $lhs_id = $lhs->[Marpa::R2::Internal::Symbol::ID];

    if ($is_ordinary_rule) {

        # Capture errors
        $grammar_c->throw_set(0);
        my $ordinary_rule_id = $grammar_c->rule_new( $lhs_id, \@rhs_ids );
        $grammar_c->throw_set(1);


        if ( $ordinary_rule_id < 0 ) {
            my $rule_description =
                "$lhs_name -> " . ( join q{ }, @{$rhs_names} );
            my ( $error_code, $error_string ) = $grammar_c->error();
            $error_code //= -1;
            my $problem_description =
                $error_code == $Marpa::R2::Error::DUPLICATE_RULE
                ? 'Duplicate rule'
                : $error_string;
            Marpa::R2::exception("$problem_description: $rule_description");
        } ## end if ( $ordinary_rule_id < 0 )
        shadow_rule( $grammar, $ordinary_rule_id );
        my $ordinary_rule = $rules->[$ordinary_rule_id];

        # Only the Stuifzand interface can set a custom mask
        if (not defined $mask or not $stuifzand_interface) {
            $mask = [(1) x scalar @rhs_ids];
        }
        $ordinary_rule->[Marpa::R2::Internal::Rule::MASK] = $mask;

        $tracer->action_set( $ordinary_rule_id, $action );
        if ( defined $rank ) {
            $grammar_c->rule_rank_set( $ordinary_rule_id, $rank );
        }
        $grammar_c->rule_null_high_set( $ordinary_rule_id,
            ( $null_ranking eq 'high' ? 1 : 0 ) );
        if ( defined $rule_name ) {
            $ordinary_rule->[Marpa::R2::Internal::Rule::NAME] = $rule_name;
            $rules_by_name->{$rule_name} = $ordinary_rule;
        }
        return;
    }    # not defined $min

    Marpa::R2::exception('Only one rhs symbol allowed for counted rule')
        if scalar @{$rhs_names} != 1;

    # create the separator symbol, if we're using one
    my $separator;
    my $separator_id = -1;
    if ( defined $separator_name ) {
        $separator =
            $stuifzand_interface
            ? assign_symbol( $grammar, $separator_name )
            : assign_user_symbol( $grammar, $separator_name );
        $separator_id = $separator->[Marpa::R2::Internal::Symbol::ID];
    } ## end if ( defined $separator_name )

    my $original_rule_id = $grammar_c->sequence_new(
        $lhs_id,
        $rhs_ids[0],
        {   separator => $separator_id,
            proper    => $proper_separation,
            min       => $min,
        }
    );
    if ( not defined $original_rule_id ) {
        my $rule_description = "$lhs_name -> " . ( join q{ }, @{$rhs_names} );
        my ( $error_code, $error_string ) = $grammar_c->error();
        $error_code //= -1;
        my $problem_description =
            $error_code == $Marpa::R2::Error::DUPLICATE_RULE
            ? 'Duplicate rule'
            : $error_string;
        Marpa::R2::exception("$problem_description: $rule_description");
    } ## end if ( not defined $original_rule_id )

    shadow_rule( $grammar, $original_rule_id );

    # The original rule for a sequence rule is
    # not actually used in parsing,
    # but some of the rewritten sequence rules are its
    # semantic equivalents.
    my $original_rule = $rules->[$original_rule_id];
    $tracer->action_set( $original_rule_id, $action );
    $original_rule->[Marpa::R2::Internal::Rule::DISCARD_SEPARATION] =
        $separator_id >= 0 && !$keep_separation;
    $grammar_c->rule_null_high_set( $original_rule_id,
        ( $null_ranking eq 'high' ? 1 : 0 ) );
    $grammar_c->rule_rank_set( $original_rule_id, $rank );

    if ( defined $rule_name ) {
        $original_rule->[Marpa::R2::Internal::Rule::NAME] = $rule_name;
        $rules_by_name->{$rule_name} = $original_rule;
    }

    return;

} ## end sub add_user_rule

sub set_start_symbol {
    my $grammar = shift;

    my $grammar_c  = $grammar->[Marpa::R2::Internal::Grammar::C];
    my $start_name = $grammar->[Marpa::R2::Internal::Grammar::START_NAME];

    return if not defined $start_name;
    my $start_id =
        $grammar->[Marpa::R2::Internal::Grammar::TRACER]
        ->symbol_by_name($start_name);
    Marpa::R2::exception(qq{Start symbol "$start_name" not in grammar})
        if not defined $start_id;

    if ( not defined $grammar_c->start_symbol_set($start_id) ) {
        Marpa::R2::uncaught_error( $grammar_c->error() );
    }
    return 1;
} ## end sub set_start_symbol

sub Marpa::R2::Grammar::error {
    my ($grammar) = @_;
    my $grammar_c = $grammar->[Marpa::R2::Internal::Grammar::C];
    return $grammar_c->error();
}

# INTERNAL OK AFTER HERE _marpa_

sub Marpa::R2::Grammar::show_ISY {
    my ( $grammar, $isy_id ) = @_;
    my $grammar_c = $grammar->[Marpa::R2::Internal::Grammar::C];
    my $tracer    = $grammar->[Marpa::R2::Internal::Grammar::TRACER];
    my $text      = q{};

    my $name = $tracer->isy_name($isy_id);
    $text .= "$isy_id: $name";

    my @tag_list = ();
    $grammar_c->_marpa_g_isy_is_nulling($isy_id)
        and push @tag_list, 'nulling';

    $text .= join q{ }, q{,}, @tag_list if scalar @tag_list;
    $text .= "\n";

    return $text;

} ## end sub Marpa::R2::Grammar::show_ISY

sub Marpa::R2::Grammar::show_ISYs {
    my ($grammar) = @_;
    my $grammar_c = $grammar->[Marpa::R2::Internal::Grammar::C];
    my $text      = q{};
    for my $isy_id ( 0 .. $grammar_c->_marpa_g_isy_count() - 1 ) {
        $text .= $grammar->show_ISY($isy_id);
    }
    return $text;
} ## end sub Marpa::R2::Grammar::show_ISYs

sub Marpa::R2::Grammar::brief_irl {
    my ( $grammar, $irl_id ) = @_;
    my $grammar_c = $grammar->[Marpa::R2::Internal::Grammar::C];
    my $tracer    = $grammar->[Marpa::R2::Internal::Grammar::TRACER];
    my $lhs_id    = $grammar_c->_marpa_g_irl_lhs($irl_id);
    my $text = $irl_id . ': ' . $tracer->isy_name($lhs_id) . ' ->';
    if ( my $rh_length = $grammar_c->_marpa_g_irl_length($irl_id) ) {
        my @rhs_ids = ();
        for my $ix ( 0 .. $rh_length - 1 ) {
            push @rhs_ids, $grammar_c->_marpa_g_irl_rhs( $irl_id, $ix );
        }
        $text .= q{ } . ( join q{ }, map { $tracer->isy_name($_) } @rhs_ids );
    } ## end if ( my $rh_length = $grammar_c->_marpa_g_irl_length...)
    return $text;
} ## end sub Marpa::R2::Grammar::brief_irl

sub Marpa::R2::Grammar::show_IRLs {
    my ($grammar) = @_;
    my $grammar_c = $grammar->[Marpa::R2::Internal::Grammar::C];
    my $text      = q{};
    for my $irl_id ( 0 .. $grammar_c->_marpa_g_irl_count() - 1 ) {
        $text .= $grammar->brief_irl($irl_id) . "\n";
    }
    return $text;
} ## end sub Marpa::R2::Grammar::show_IRLs

sub Marpa::R2::Grammar::rule_is_used {
    my ( $grammar, $rule_id ) = @_;
    my $grammar_c = $grammar->[Marpa::R2::Internal::Grammar::C];
    return $grammar_c->_marpa_g_rule_is_used($rule_id);
}

sub Marpa::R2::Grammar::show_AHFA {
    my ( $grammar, $verbose ) = @_;
    return $grammar->[Marpa::R2::Internal::Grammar::TRACER]
        ->show_AHFA($verbose);
}

sub Marpa::R2::Grammar::show_AHFA_items {
    my ( $grammar, $verbose ) = @_;
    return $grammar->[Marpa::R2::Internal::Grammar::TRACER]
        ->show_AHFA_items($verbose);
}

1;

# vim: expandtab shiftwidth=4:
