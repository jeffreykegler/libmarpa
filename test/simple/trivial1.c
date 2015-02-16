/*
 * Copyright 2015 Jeffrey Kegler
 * This file is part of Libmarpa.  Libmarpa is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * Libmarpa is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser
 * General Public License along with Libmarpa.  If not, see
 * http://www.gnu.org/licenses/.
 */

/* Tests of Libmarpa methods on trivial grammar */

#include <stdio.h>
#include "marpa.h"

#include "tap/basic.h"

static int
warn (const char *s, Marpa_Grammar g)
{
  printf ("%s returned %d\n", s, marpa_g_error (g, NULL));
}

static int
fail (const char *s, Marpa_Grammar g)
{
  warn (s, g);
  exit (1);
}

Marpa_Symbol_ID S_top;
Marpa_Symbol_ID S_A1;
Marpa_Symbol_ID S_A2;
Marpa_Symbol_ID S_B1;
Marpa_Symbol_ID S_B2;
Marpa_Symbol_ID S_C1;
Marpa_Symbol_ID S_C2;

/* Longest rule is <= 4 symbols */
Marpa_Symbol_ID rhs[4];

Marpa_Rule_ID R_top_1;
Marpa_Rule_ID R_top_2;
Marpa_Rule_ID R_C2_3; // highest rule id

/* For (error) messages */
char msgbuf[80];

char *
symbol_name (Marpa_Symbol_ID id)
{
  if (id == S_top) return "top";
  if (id == S_A1) return "A1";
  if (id == S_A2) return "A2";
  if (id == S_B1) return "B1";
  if (id == S_B2) return "B2";
  if (id == S_C1) return "C1";
  if (id == S_C2) return "C2";
  sprintf (msgbuf, "no such symbol: %d", id);
  return msgbuf;
}

int
is_nullable (Marpa_Symbol_ID id)
{
  if (id == S_top) return 1;
  if (id == S_A1) return 1;
  if (id == S_A2) return 1;
  if (id == S_B1) return 1;
  if (id == S_B2) return 1;
  if (id == S_C1) return 1;
  if (id == S_C2) return 1;
  return 0;
}

int
is_nulling (Marpa_Symbol_ID id)
{
  if (id == S_C1) return 1;
  if (id == S_C2) return 1;
  return 0;
}

static Marpa_Grammar
trivial_grammar(Marpa_Config *config)
{
  Marpa_Grammar g;
  g = marpa_g_new (config);
  if (!g)
    {
      Marpa_Error_Code errcode = marpa_c_error (config, NULL);
      printf ("marpa_g_new returned %d", errcode);
      exit (1);
    }

  ((S_top = marpa_g_symbol_new (g)) >= 0)
    || fail ("marpa_g_symbol_new", g);
  ((marpa_g_symbol_is_nulled_event_set(g, S_top, 1)) >= 0)
    || fail ("marpa_g_symbol_is_nulled_event_set", g);
  ((S_A1 = marpa_g_symbol_new (g)) >= 0)
    || fail ("marpa_g_symbol_new", g);
  ((marpa_g_symbol_is_nulled_event_set(g, S_A1, 1)) >= 0)
    || fail ("marpa_g_symbol_is_nulled_event_set", g);
  ((S_A2 = marpa_g_symbol_new (g)) >= 0)
    || fail ("marpa_g_symbol_new", g);
  ((marpa_g_symbol_is_nulled_event_set(g, S_A2, 1)) >= 0)
    || fail ("marpa_g_symbol_is_nulled_event_set", g);
  ((S_B1 = marpa_g_symbol_new (g)) >= 0)
    || fail ("marpa_g_symbol_new", g);
  ((marpa_g_symbol_is_nulled_event_set(g, S_B1, 1)) >= 0)
    || fail ("marpa_g_symbol_is_nulled_event_set", g);
  ((S_B2 = marpa_g_symbol_new (g)) >= 0)
    || fail ("marpa_g_symbol_new", g);
  ((marpa_g_symbol_is_nulled_event_set(g, S_B2, 1)) >= 0)
    || fail ("marpa_g_symbol_is_nulled_event_set", g);
  ((S_C1 = marpa_g_symbol_new (g)) >= 0)
    || fail ("marpa_g_symbol_new", g);
  ((marpa_g_symbol_is_nulled_event_set(g, S_C1, 1)) >= 0)
    || fail ("marpa_g_symbol_is_nulled_event_set", g);
  ((S_C2 = marpa_g_symbol_new (g)) >= 0)
    || fail ("marpa_g_symbol_new", g);
  ((marpa_g_symbol_is_nulled_event_set(g, S_C2, 1)) >= 0)
    || fail ("marpa_g_symbol_is_nulled_event_set", g);

  rhs[0] = S_A1;
  ((R_top_1 = marpa_g_rule_new (g, S_top, rhs, 1)) >= 0)
    || fail ("marpa_g_rule_new", g);
  rhs[0] = S_A2;
  ((R_top_2 = marpa_g_rule_new (g, S_top, rhs, 1)) >= 0)
    || fail ("marpa_g_rule_new", g);
  rhs[0] = S_B1;
  (marpa_g_rule_new (g, S_A1, rhs, 1) >= 0)
    || fail ("marpa_g_rule_new", g);
  rhs[0] = S_B2;
  (marpa_g_rule_new (g, S_A2, rhs, 1) >= 0)
    || fail ("marpa_g_rule_new", g);
  rhs[0] = S_C1;
  (marpa_g_rule_new (g, S_B1, rhs, 1) >= 0)
    || fail ("marpa_g_rule_new", g);
  rhs[0] = S_C2;
  (marpa_g_rule_new (g, S_B2, rhs, 1) >= 0)
    || fail ("marpa_g_rule_new", g);
  (marpa_g_rule_new (g, S_C1, rhs, 0) >= 0)
    || fail ("marpa_g_rule_new", g);

  ((R_C2_3 = marpa_g_rule_new (g, S_C2, rhs, 0)) >= 0)
    || fail ("marpa_g_rule_new", g);

  return g;
}

static Marpa_Error_Code
trivial_grammar_precompute(Marpa_Grammar g, Marpa_Symbol_ID S_start)
{
  Marpa_Error_Code rc;

  (marpa_g_start_symbol_set (g, S_start) >= 0)
    || fail ("marpa_g_start_symbol_set", g);

  rc = marpa_g_precompute (g);
  if (rc < 0)
    fail("marpa_g_precompute", g);

  return rc;
}

/* test retcode and error code on expected failure */
static int
is_failure(Marpa_Grammar g, Marpa_Error_Code errcode_wanted, int retcode_wanted, int retcode, char *method_name, char *msg)
{
  int errcode;

  sprintf (msgbuf, "%s(): %s", method_name, msg);
  is_int(retcode_wanted, retcode, msgbuf);

  errcode = marpa_g_error (g, NULL);
  sprintf (msgbuf, "%s() error code", method_name);
  is_int(errcode_wanted, errcode, msgbuf);

  marpa_g_error_clear(g);
}

/* test retval and print error code on unexpected failure */
static int
is_success(Marpa_Grammar g, int wanted, int retval, char *method_name)
{
  is_int(wanted, retval, method_name);

  if (retval < 0 && strstr(method_name, "marpa_g_rule_rank") == NULL )
    warn(method_name, g);

  marpa_g_error_clear(g);
}

int
main (int argc, char *argv[])
{
  int rc;

  Marpa_Config marpa_configuration;

  Marpa_Grammar g;
  Marpa_Recognizer r;

  Marpa_Rank rank;
  int flag;

  plan_lazy();

  marpa_c_init (&marpa_configuration);
  g = trivial_grammar(&marpa_configuration);

  /* Grammar Methods per sections of api.texi: Symbols, Rules, Sequnces, Ranks, Events */

  /* these must soft fail if there is not start symbol */
#define NO_START_TEST_MSG "fail before marpa_g_start_symbol_set()"

  is_failure(g, MARPA_ERR_INVALID_SYMBOL_ID, -2, marpa_g_symbol_is_start (g, -1), "marpa_g_symbol_is_start",
    "symbol is not well-formed");
  is_failure(g, MARPA_ERR_NO_SUCH_SYMBOL_ID, -1, marpa_g_symbol_is_start (g, 42), "marpa_g_symbol_is_start",
    "symbol is well-formed, but doesn't exist");
  /* Returns 0 if sym_id is not the start symbol, either because the start symbol
     is different from sym_id, or because the start symbol has not been set yet. */
  is_success(g, 0, marpa_g_symbol_is_start (g, S_top), "marpa_g_symbol_is_start");
  is_failure(g, MARPA_ERR_NO_START_SYMBOL, -1, marpa_g_start_symbol (g), "marpa_g_start_symbol", NO_START_TEST_MSG);

  (marpa_g_start_symbol_set (g, S_top) >= 0)
    || fail ("marpa_g_start_symbol_set", g);

  /* these must succeed after the start symbol is set */
  is_success(g, 1, marpa_g_symbol_is_start (g, S_top), "marpa_g_symbol_is_start");
  is_success(g, S_top, marpa_g_start_symbol (g), "marpa_g_start_symbol()");
  is_success(g, S_C2, marpa_g_highest_symbol_id (g), "marpa_g_highest_symbol_id()");

  /* these must return -2 and set error code to MARPA_ERR_NOT_PRECOMPUTED */
  /* Symbols */
#define NOT_PRECOMPUTED_TEST_MSG "fail before marpa_g_precompute()"
  is_failure(g, MARPA_ERR_NOT_PRECOMPUTED, -2, marpa_g_symbol_is_accessible  (g, S_C2), "marpa_g_symbol_is_accessible", NOT_PRECOMPUTED_TEST_MSG);
  is_failure(g, MARPA_ERR_NOT_PRECOMPUTED, -2, marpa_g_symbol_is_nullable (g, S_A1), "marpa_g_symbol_is_nullable", NOT_PRECOMPUTED_TEST_MSG);
  is_failure(g, MARPA_ERR_NOT_PRECOMPUTED, -2, marpa_g_symbol_is_nulling (g, S_A1), "marpa_g_symbol_is_nulling", NOT_PRECOMPUTED_TEST_MSG);
  is_failure(g, MARPA_ERR_NOT_PRECOMPUTED, -2, marpa_g_symbol_is_productive (g, S_top), "marpa_g_symbol_is_productive", NOT_PRECOMPUTED_TEST_MSG);
  is_success(g, 0, marpa_g_symbol_is_terminal(g, S_top), "marpa_g_symbol_is_terminal");

  /* Rules */
  is_failure(g, MARPA_ERR_NOT_PRECOMPUTED, -2, marpa_g_rule_is_nullable (g, R_top_2), "marpa_g_rule_is_nullable", NOT_PRECOMPUTED_TEST_MSG);
  is_failure(g, MARPA_ERR_NOT_PRECOMPUTED, -2, marpa_g_rule_is_nulling (g, R_top_2), "marpa_g_rule_is_nulling", NOT_PRECOMPUTED_TEST_MSG);
  is_failure(g, MARPA_ERR_NOT_PRECOMPUTED, -2, marpa_g_rule_is_loop (g, R_C2_3), "marpa_g_rule_is_loop", NOT_PRECOMPUTED_TEST_MSG);

  /* expected failures on attempts to non-well-formed and non-existing symbols as terminals */
  is_failure(g, MARPA_ERR_INVALID_SYMBOL_ID, -2, marpa_g_symbol_is_terminal_set (g, -1, 1), "marpa_g_symbol_is_terminal",
    "symbol is not well-formed");
  is_failure(g, MARPA_ERR_NO_SUCH_SYMBOL_ID, -1, marpa_g_symbol_is_terminal_set (g, 42, 1), "marpa_g_symbol_is_terminal",
    "symbol is well-formed, but doesn't exist");
  /* set a nulling symbol to be terminal and test precomputation failure */
  is_success(g, 1, marpa_g_symbol_is_terminal_set(g, S_C1, 1),
    "marpa_g_symbol_is_terminal_set()");
  is_failure(g, MARPA_ERR_TERMINAL_IS_LOCKED, -2, marpa_g_symbol_is_terminal_set(g, S_C1, 0),
    "marpa_g_symbol_is_terminal_set", "on a symbol already set to be a terminal");
  is_failure(g, MARPA_ERR_NULLING_TERMINAL, -2, marpa_g_precompute (g), "marpa_g_precompute", "with a nulling terminal");

  /* terminals are locked after setting, so we recreate the grammar */
  marpa_g_unref(g);
  g = trivial_grammar(&marpa_configuration);

  is_failure(g, MARPA_ERR_NO_START_SYMBOL, -2, marpa_g_precompute (g), "marpa_g_precompute", "before marpa_g_start_symbol_set()");

  trivial_grammar_precompute(g, S_top);
  ok(1, "precomputation succeeded");

  /* Symbols -- these do have @<Fail if not precomputed@>@ */
  is_success(g, 1, marpa_g_symbol_is_accessible  (g, S_C2), "marpa_g_symbol_is_accessible()");
  is_success(g, 1, marpa_g_symbol_is_nullable (g, S_A1), "marpa_g_symbol_is_nullable()");
  is_success(g, 1, marpa_g_symbol_is_nulling (g, S_A1), "marpa_g_symbol_is_nulling()");
  is_success(g, 1, marpa_g_symbol_is_productive (g, S_top), "marpa_g_symbol_is_productive()");
  is_success(g, 1, marpa_g_symbol_is_start (g, S_top), "marpa_g_symbol_is_start()");
  is_success(g, 0, marpa_g_symbol_is_terminal(g, S_top), "marpa_g_symbol_is_terminal()");
  is_failure(g, MARPA_ERR_INVALID_SYMBOL_ID, -2, marpa_g_symbol_is_terminal(g, -1), "marpa_g_symbol_is_terminal",
    "symbol is not well-formed");
  is_failure(g, MARPA_ERR_NO_SUCH_SYMBOL_ID, -1, marpa_g_symbol_is_terminal (g, 42), "marpa_g_symbol_is_terminal",
    "symbol is well-formed, but doesn't exist");

  is_failure(g, MARPA_ERR_PRECOMPUTED, -2, marpa_g_symbol_is_terminal_set (g, S_top, 0),
    "marpa_g_symbol_is_terminal_set", "on precomputed grammar");
  is_failure(g, MARPA_ERR_PRECOMPUTED, -2, marpa_g_start_symbol_set (g, S_top),
    "marpa_g_start_symbol_set", "on precomputed grammar");

  /* Rules */
  is_success(g, R_C2_3, marpa_g_highest_rule_id (g), "marpa_g_highest_rule_id()");
  is_success(g, 1, marpa_g_rule_is_accessible (g, R_top_1), "marpa_g_rule_is_accessible()");
  is_success(g, 1, marpa_g_rule_is_nullable (g, R_top_2), "marpa_g_rule_is_nullable()");
  is_success(g, 1, marpa_g_rule_is_nulling (g, R_top_2), "marpa_g_rule_is_nulling()");
  is_success(g, 0, marpa_g_rule_is_loop (g, R_C2_3), "marpa_g_rule_is_loop()");
  is_success(g, 1, marpa_g_rule_is_productive (g, R_C2_3), "marpa_g_rule_is_productive()");
  is_success(g, 1, marpa_g_rule_length (g, R_top_1), "marpa_g_rule_length()");
  is_success(g, 0, marpa_g_rule_length (g, R_C2_3), "marpa_g_rule_length()");
  is_success(g, S_top, marpa_g_rule_lhs (g, R_top_1), "marpa_g_rule_lhs()");
  is_success(g, S_A1, marpa_g_rule_rhs (g, R_top_1, 0), "marpa_g_rule_rhs()");
  is_success(g, S_A2, marpa_g_rule_rhs (g, R_top_2, 0), "marpa_g_rule_rhs()");

  /* Sequences */
  /* try to add a nulling sequence, and make sure that it fails with an appropriate
     error code -- http://irclog.perlgeek.de/marpa/2015-02-13#i_10111831  */
  /* recreate the grammar */
  marpa_g_unref(g);
  g = trivial_grammar(&marpa_configuration);
  /* try to add a nulling sequence */
  is_failure(g, MARPA_ERR_SEQUENCE_LHS_NOT_UNIQUE, -2,
    marpa_g_sequence_new (g, S_top, S_B1, S_B2, 0, MARPA_PROPER_SEPARATION),
      "marpa_g_sequence_new", "with non-unique lhs");
  /* test error codes of other sequence methods */
  /* non-sequence rule id */
  is_success(g, 0, marpa_g_rule_is_proper_separation (g, R_top_1), "marpa_g_rule_is_proper_separation");
  is_failure(g, 0, -1, marpa_g_sequence_min (g, R_top_1),
    "marpa_g_sequence_min", "non-sequence rule id");
  is_failure(g, MARPA_ERR_NOT_A_SEQUENCE, -2, marpa_g_sequence_separator (g, R_top_1),
    "marpa_g_sequence_separator", "non-sequence rule id");
  is_success(g, 0, marpa_g_symbol_is_counted (g, R_top_1), "marpa_g_symbol_is_counted");
  /* malformed rule/symbol id */
  is_failure(g, MARPA_ERR_INVALID_RULE_ID, -2, marpa_g_rule_is_proper_separation (g, -1),
    "marpa_g_rule_is_proper_separation", "malformed rule id");
  is_failure(g, MARPA_ERR_INVALID_RULE_ID, -2, marpa_g_sequence_min (g, -1),
    "marpa_g_sequence_min", "malformed rule id");
  is_failure(g, MARPA_ERR_INVALID_RULE_ID, -2, marpa_g_sequence_separator (g, -1),
    "marpa_g_sequence_separator", "malformed rule id");
  is_failure(g, MARPA_ERR_INVALID_SYMBOL_ID, -2, marpa_g_symbol_is_counted (g, -1),
    "marpa_g_symbol_is_counted", "malformed rule id");
  /* well-formed, but invalid rule/symbol id */
  is_failure(g, MARPA_ERR_NO_SUCH_RULE_ID, -1, marpa_g_rule_is_proper_separation (g, 42),
    "marpa_g_rule_is_proper_separation", "non-existent rule id");
  is_failure(g, MARPA_ERR_NO_SUCH_RULE_ID, -2, marpa_g_sequence_min (g, 42),
    "marpa_g_sequence_min", "non-existent rule id");
  is_failure(g, MARPA_ERR_NO_SUCH_RULE_ID, -2, marpa_g_sequence_separator (g, 42),
    "marpa_g_sequence_separator", "non-existent rule id");
  is_failure(g, MARPA_ERR_NO_SUCH_SYMBOL_ID, -1, marpa_g_symbol_is_counted (g, 42),
    "marpa_g_symbol_is_counted", "non-existent symbol id");

  /* Ranks */
  rank = -2;
  is_success(g, rank, marpa_g_rule_rank_set (g, R_top_1, rank), "marpa_g_rule_rank_set()");
  is_success(g, rank, marpa_g_rule_rank (g, R_top_1), "marpa_g_rule_rank()");
  flag = 1;
  is_success(g, flag, marpa_g_rule_null_high_set (g, R_top_2, flag), "marpa_g_rule_null_high_set()");
  is_success(g, flag, marpa_g_rule_null_high (g, R_top_2), "marpa_g_rule_null_high()");

  is_failure(g, MARPA_ERR_INVALID_RULE_ID, -2, marpa_g_rule_rank_set (g, -1, rank),
    "marpa_g_rule_rank_set", "malformed rule id");
  is_failure(g, MARPA_ERR_INVALID_RULE_ID, -2, marpa_g_rule_rank (g, -1),
    "marpa_g_rule_rank", "malformed rule id");
  is_failure(g, MARPA_ERR_INVALID_RULE_ID, -2, marpa_g_rule_null_high_set (g, -1, flag),
    "marpa_g_rule_null_high_set", "malformed rule id");
  is_failure(g, MARPA_ERR_INVALID_RULE_ID, -2, marpa_g_rule_null_high (g, -1),
    "marpa_g_rule_null_high", "malformed rule id");

  is_failure(g, MARPA_ERR_NO_SUCH_RULE_ID, -2, marpa_g_rule_rank_set (g, 42, rank),
    "marpa_g_rule_rank_set", "invalid rule id");
  is_failure(g, MARPA_ERR_NO_SUCH_RULE_ID, -2, marpa_g_rule_rank (g, 42),
    "marpa_g_rule_rank", "invalid rule id");
  is_failure(g, MARPA_ERR_NO_SUCH_RULE_ID, -2, marpa_g_rule_null_high_set (g, 42, flag),
    "marpa_g_rule_null_high_set", "invalid rule id");
  is_failure(g, MARPA_ERR_NO_SUCH_RULE_ID, -2, marpa_g_rule_null_high (g, 42),
    "marpa_g_rule_null_high", "invalid rule id");

  trivial_grammar_precompute(g, S_top);
  ok(1, "precomputation succeeded");

  /* Ranks methods on precomputed grammar */
  is_failure(g, MARPA_ERR_PRECOMPUTED, -2, marpa_g_rule_rank_set (g, R_top_1, rank),
    "marpa_g_rule_rank_set", "on precomputed grammar");
  is_success(g, rank, marpa_g_rule_rank (g, R_top_1), "marpa_g_rule_rank()");
  is_failure(g, MARPA_ERR_PRECOMPUTED, -2, marpa_g_rule_null_high_set (g, R_top_2, flag),
    "marpa_g_rule_null_high_set", "on precomputed grammar");
  is_success(g, 1, marpa_g_rule_null_high (g, R_top_2), "marpa_g_rule_null_high()");

  /* Events */
  /* test that attempts to create events, other than nulled events,
     results in a reasonable error -- http://irclog.perlgeek.de/marpa/2015-02-13#i_10111838 */

  /* Recognizer Methods */
  r = marpa_r_new (g);
  if (!r)
    fail("marpa_r_new", g);

  rc = marpa_r_start_input (r);
  if (!rc)
    fail("marpa_r_start_input", g);

  diag ("at earleme 0");
  is_success(g, 1, marpa_r_is_exhausted(r), "marpa_r_is_exhausted()");

  return 0;
}
