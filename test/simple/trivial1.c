/*
 * Copyright 2015 Jeffrey Kegler
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

/* Tests of Libmarpa methods on trivial grammar */

#include <stdio.h>
#include <string.h>
#include "marpa.h"

#include "marpa_m_test.h"

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
marpa_g_trivial_new(Marpa_Config *config)
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
  ((S_A1 = marpa_g_symbol_new (g)) >= 0)
    || fail ("marpa_g_symbol_new", g);
  ((S_A2 = marpa_g_symbol_new (g)) >= 0)
    || fail ("marpa_g_symbol_new", g);
  ((S_B1 = marpa_g_symbol_new (g)) >= 0)
    || fail ("marpa_g_symbol_new", g);
  ((S_B2 = marpa_g_symbol_new (g)) >= 0)
    || fail ("marpa_g_symbol_new", g);
  ((S_C1 = marpa_g_symbol_new (g)) >= 0)
    || fail ("marpa_g_symbol_new", g);
  ((S_C2 = marpa_g_symbol_new (g)) >= 0)
    || fail ("marpa_g_symbol_new", g);

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
marpa_g_trivial_precompute(Marpa_Grammar g, Marpa_Symbol_ID S_start)
{
  Marpa_Error_Code rc;

  (marpa_g_start_symbol_set (g, S_start) >= 0)
    || fail ("marpa_g_start_symbol_set", g);

  rc = marpa_g_precompute (g);
  if (rc < 0)
    fail("marpa_g_precompute", g);

  return rc;
}

int test_r_terminals_expected(
  Marpa_Grammar g,
  Marpa_Recognizer r,
  int expected_return
)
{
  Marpa_Symbol_ID buffer[42];
  const char* name = "marpa_r_latest_earley_set_values_set";
  int rv_seen = marpa_r_terminals_expected(r, buffer);

  /* success wanted */
  if ( rv_seen >= 0 ) {
    if (expected_return == rv_seen) {
      ok( 1, "%s(r, ...) succeeded as expected, returned %d", name, rv_seen );
    } else {
      ok( 0, "%s(r, ...) succeeded, but returned %d; %d expected",
	name, rv_seen, expected_return );
    }
  } else {
    ok( 1, "%s(r, ...) failed, returned %d", name, rv_seen );
  }
}

int test_r_progress_item(
  Marpa_Grammar g,
  Marpa_Recognizer r,
  int expected_return,
  int expected_errcode
)
{
  int set_id;
  Marpa_Earley_Set_ID origin;

  const char* name = "marpa_r_progress_item";
  int rv_seen = marpa_r_progress_item(r, &set_id, &origin);

  /* success wanted */
  if ( rv_seen >= 0 ) {
    if (expected_return == rv_seen) {
      ok( 1, "%s(r, ...) succeeded as expected, returned %d", name, rv_seen );
    } else {
      ok( 0, "%s(r, ...) succeeded, but returned %d; %d expected",
	name, rv_seen, expected_return );
    }
  } else {
    ok( 1, "%s(r, ...) failed, returned %d", name, rv_seen );
    int err_seen = marpa_g_error(g, NULL);
    is_int( expected_errcode, err_seen,
        "%s() error is: %s", name, marpa_m_error_message(err_seen) );
  }
}

int test_r_latest_earley_set_values_set(
  Marpa_Grammar g,
  Marpa_Recognizer r,
  int expected_return,
  int int_value,
  void* ptr_value
)
{
  const char* name = "marpa_r_latest_earley_set_values_set";
  int rv_seen = marpa_r_latest_earley_set_values_set(r, int_value, ptr_value);

  /* success wanted */
  if ( rv_seen >= 0 ) {
    if (expected_return == rv_seen) {
      ok( 1, "%s(r, %d, ...) succeeded as expected, returned %d", name, int_value, rv_seen );
    } else {
      ok( 0, "%s(r, %d, ...) succeeded, but returned %d; %d expected",
	name, int_value, rv_seen, expected_return );
    }
  } else {
    ok( 1, "%s(r, %d, ...) failed, returned %d", name, int_value, rv_seen );
  }
}

int
test_r_earley_set_values(
  Marpa_Grammar g,
  Marpa_Recognizer r,
  Marpa_Earley_Set_ID set_id,
  int expected_return,
  int expected_int_value,
  void* expected_value2,
  int expected_errcode
)
{
  const int orig_int_value = 86;
  int int_value = orig_int_value;
  /* A pointer that will not match NULL */
  void* orig_value2 = strdup("");
  void* value2 = orig_value2;

  const char* name = "marpa_r_earley_set_values";
  int rv_seen = marpa_r_earley_set_values(r, set_id, &int_value, &value2);

  /* success wanted */
  if ( rv_seen >= 0 ) {
    if (expected_return == rv_seen) {
      ok( 1, "%s(r, %d, ...) succeeded, returned %d", name, set_id, rv_seen );
    } else {
      ok( 0, "%s(r, ...) succeeded, but returned %d; %d expected",
	name, set_id, rv_seen, expected_return );
    }
    is_int ( expected_int_value, int_value,
      "marpa_r_earley_set_values() int* value" );

    /* There is no c89 portable way to test arbitrary pointers.
     * With ifdef's we could cover 99.999% of cases, but for now
     * we do not bother.
     */
    /* ok ( (expected_value2 != value2),
     *  "marpa_r_earley_set_values() void** value" );
     */

  } else {
    /* return value */
    int err_seen = marpa_g_error(g, NULL);
    is_int( expected_errcode, err_seen,
        "%s() error is: %s", name, marpa_m_error_message(err_seen) );

    is_int ( int_value, orig_int_value,
    "marpa_r_earley_set_values() int* value" );

    /* There is no c89 portable way to test arbitrary pointers.
     * With ifdef's we could cover 99.999% of cases, but for now
     * we do not bother.
     */
    /* ok ( (value2 != orig_value2),
     *  "marpa_r_earley_set_values() void** value" );
     */
  }
  free(orig_value2);

}

int
main (int argc, char *argv[])
{
  int rc;
  int ix;

  Marpa_Config marpa_configuration;

  Marpa_Grammar g;
  Marpa_Recognizer r;

  Marpa_Rank negative_rank, positive_rank;
  int flag;

  int whatever;

  plan(343);

  /* We are not guaranteed the ability to safely match pointers strings to
   * garbage pointers, the best we can do is compare NULL and non-NULL
   */
  char *value2_base = NULL;
  void *value2 = value2_base;

  marpa_c_init (&marpa_configuration);
  g = marpa_g_trivial_new(&marpa_configuration);

  marpa_m_grammar_set(g); /* for marpa_g_error() in marpa_m_test() */

  /* Grammar Methods per sections of api.texi: Symbols, Rules, Sequences, Ranks, Events ... */

  marpa_m_test("marpa_g_symbol_is_start", g, S_invalid, -2, MARPA_ERR_INVALID_SYMBOL_ID);
  marpa_m_test("marpa_g_symbol_is_start", g, S_no_such, -1, MARPA_ERR_NO_SUCH_SYMBOL_ID);
  /* Returns 0 if sym_id is not the start symbol, either because the start symbol
     is different from sym_id, or because the start symbol has not been set yet. */
  marpa_m_test("marpa_g_symbol_is_start", g, S_top, 0);
  marpa_m_test("marpa_g_start_symbol", g, -1, MARPA_ERR_NO_START_SYMBOL);

  (marpa_g_start_symbol_set (g, S_top) >= 0)
    || fail ("marpa_g_start_symbol_set", g);

  /* these must succeed after the start symbol is set */
  marpa_m_test("marpa_g_symbol_is_start", g, S_top, 1);
  marpa_m_test("marpa_g_start_symbol", g, S_top);
  marpa_m_test("marpa_g_highest_symbol_id", g, S_C2);

  /* these must return -2 and set error code to MARPA_ERR_NOT_PRECOMPUTED */
  /* Symbols */
  const char *marpa_g_symbol_classifiers[] = {
    "marpa_g_symbol_is_accessible", "marpa_g_symbol_is_nullable",
    "marpa_g_symbol_is_nulling", "marpa_g_symbol_is_productive",
  };
  marpa_m_tests(marpa_g_symbol_classifiers, g, whatever, -2, MARPA_ERR_NOT_PRECOMPUTED);

  marpa_m_test("marpa_g_symbol_is_terminal", g, S_top, 0);

  /* Rules */
  const char *marpa_g_rule_classifiers[] = {
    "marpa_g_rule_is_nullable",
    "marpa_g_rule_is_nulling",
    "marpa_g_rule_is_loop",

    "marpa_g_rule_is_accessible",
    "marpa_g_rule_is_productive",
  };

  marpa_m_tests(marpa_g_rule_classifiers, g, whatever, -2, MARPA_ERR_NOT_PRECOMPUTED);

  /* marpa_g_symbol_is_terminal_set() on invalid and non-existing symbol IDs
     on a non-precomputed grammar */
  marpa_m_test("marpa_g_symbol_is_terminal_set", g, S_invalid, 1, -2, MARPA_ERR_INVALID_SYMBOL_ID);
  marpa_m_test("marpa_g_symbol_is_terminal_set", g, S_no_such, 1, -1, MARPA_ERR_NO_SUCH_SYMBOL_ID);

  /* Rules */
  marpa_m_test("marpa_g_highest_rule_id", g, R_C2_3, "before precomputation");
  marpa_m_test("marpa_g_rule_length", g, R_top_1, 1, "before precomputation");
  marpa_m_test("marpa_g_rule_length", g, R_C2_3, 0, "before precomputation");
  marpa_m_test("marpa_g_rule_lhs", g, R_top_1, S_top, "before precomputation");
  marpa_m_test("marpa_g_rule_rhs", g, R_top_1, 0, S_A1, "before precomputation");
  marpa_m_test("marpa_g_rule_rhs", g, R_top_2, 0, S_A2, "before precomputation");

  /* marpa_g_symbol_is_terminal_set() on a nulling symbol */
  marpa_m_test("marpa_g_symbol_is_terminal_set", g, S_C1, 1, 1);
  /* can't change terminal status after it's been set */
  marpa_m_test("marpa_g_symbol_is_terminal_set", g, S_C1, 0, -2, MARPA_ERR_TERMINAL_IS_LOCKED);

  marpa_m_test("marpa_g_precompute", g, -2, MARPA_ERR_NULLING_TERMINAL);

  /* terminals are locked after setting, so we recreate the grammar */
  marpa_g_unref(g);
  g = marpa_g_trivial_new(&marpa_configuration);

  marpa_m_test("marpa_g_precompute", g, -2, MARPA_ERR_NO_START_SYMBOL);

  marpa_g_trivial_precompute(g, S_top);
  ok(1, "precomputation succeeded");

  /* Symbols -- status accessors must succeed on precomputed grammar */
  marpa_m_test("marpa_g_symbol_is_accessible", g, S_C2, 1);
  marpa_m_test("marpa_g_symbol_is_nullable", g, S_A1, 1);
  marpa_m_test("marpa_g_symbol_is_nulling", g, S_A1, 1);
  marpa_m_test("marpa_g_symbol_is_productive", g, S_top, 1);
  marpa_m_test("marpa_g_symbol_is_start", g, S_top, 1);
  marpa_m_test("marpa_g_symbol_is_terminal", g, S_top, 0);

  /* terminal and start symbols can't be set on precomputed grammar */
  marpa_m_test("marpa_g_symbol_is_terminal_set", g, S_top, 0, -2, MARPA_ERR_PRECOMPUTED);
  marpa_m_test("marpa_g_start_symbol_set", g, S_top, -2, MARPA_ERR_PRECOMPUTED);

  /* Rules */
  marpa_m_test("marpa_g_highest_rule_id", g, R_C2_3);
  marpa_m_test("marpa_g_rule_is_accessible", g, R_top_1, 1);
  marpa_m_test("marpa_g_rule_is_nullable", g, R_top_2, 1);
  marpa_m_test("marpa_g_rule_is_nulling", g, R_top_2, 1);
  marpa_m_test("marpa_g_rule_is_loop", g, R_C2_3, 0);
  marpa_m_test("marpa_g_rule_is_productive", g, R_C2_3, 1);
  marpa_m_test("marpa_g_rule_length", g, R_top_1, 1);
  marpa_m_test("marpa_g_rule_length", g, R_C2_3, 0);
  marpa_m_test("marpa_g_rule_lhs", g, R_top_1, S_top);

  {
    marpa_m_test("marpa_g_rule_rhs", g, R_top_1, 0, S_A1);
    marpa_m_test("marpa_g_rule_rhs", g, R_top_2, 0, S_A2);

    int ix_out_of_bounds = 25;
    marpa_m_test("marpa_g_rule_rhs", g, R_top_2, ix_out_of_bounds, -2, MARPA_ERR_RHS_IX_OOB);

    int ix_negative = -1;
    marpa_m_test("marpa_g_rule_rhs", g, R_top_2, ix_negative, -2, MARPA_ERR_RHS_IX_NEGATIVE);
  }

  /* invalid/no such rule id error handling */
  const char *marpa_g_rule_accessors[] = {
    "marpa_g_rule_is_accessible", "marpa_g_rule_is_loop", "marpa_g_rule_is_productive",
    "marpa_g_rule_is_nullable", "marpa_g_rule_is_nulling",
    "marpa_g_rule_length", "marpa_g_rule_lhs",
  };
  marpa_m_tests(marpa_g_rule_accessors, g, R_invalid, -2, MARPA_ERR_INVALID_RULE_ID);
  marpa_m_tests(marpa_g_rule_accessors, g, R_no_such, -1, MARPA_ERR_NO_SUCH_RULE_ID);

  /* Sequences */
  /* try to add a nulling sequence, and make sure that it fails with an appropriate
     error code -- http://irclog.perlgeek.de/marpa/2015-02-13#i_10111831  */

  /* recreate the grammar */
  marpa_g_unref(g);
  g = marpa_g_trivial_new(&marpa_configuration);

  /* try to add a nulling sequence */
  marpa_m_test("marpa_g_sequence_new", g, S_top, S_B1, S_B2, 0, MARPA_PROPER_SEPARATION,
    -2, MARPA_ERR_SEQUENCE_LHS_NOT_UNIQUE);

  /* test error codes of other sequence methods */
  /* non-sequence rule id */
  marpa_m_test("marpa_g_rule_is_proper_separation", g, R_top_1, 0);
  marpa_m_test("marpa_g_sequence_min", g, R_top_1, -1, MARPA_ERR_NOT_A_SEQUENCE);
  marpa_m_test("marpa_g_sequence_separator", g, R_top_1, -2, MARPA_ERR_NOT_A_SEQUENCE);
  marpa_m_test("marpa_g_symbol_is_counted", g, S_top, 0);

  /* invalid/no such rule id error handling */
  const char *marpa_g_sequence_mutators[] = {
    "marpa_g_sequence_separator",
    "marpa_g_sequence_min",
  };
  marpa_m_tests(marpa_g_sequence_mutators, g, R_invalid, -2, MARPA_ERR_INVALID_RULE_ID);
  marpa_m_tests(marpa_g_sequence_mutators, g, R_no_such, -2, MARPA_ERR_NO_SUCH_RULE_ID);

  marpa_m_test("marpa_g_rule_is_proper_separation", g, R_invalid,
    -2, MARPA_ERR_INVALID_RULE_ID);
  marpa_m_test("marpa_g_rule_is_proper_separation", g, R_no_such,
    -1, MARPA_ERR_NO_SUCH_RULE_ID);

  marpa_m_test("marpa_g_symbol_is_counted", g, S_invalid, -2, MARPA_ERR_INVALID_SYMBOL_ID);
  marpa_m_test("marpa_g_symbol_is_counted", g, S_no_such, -1, MARPA_ERR_NO_SUCH_SYMBOL_ID);

  /* Ranks */
  negative_rank = -1;
  marpa_m_test("marpa_g_rule_rank_set", g, R_top_1, negative_rank, negative_rank);
  marpa_m_test("marpa_g_rule_rank", g, R_top_1, negative_rank);

  positive_rank = 1;
  marpa_m_test("marpa_g_rule_rank_set", g, R_top_2, positive_rank, positive_rank);
  marpa_m_test("marpa_g_rule_rank", g, R_top_2, positive_rank);

  flag = 1;
  marpa_m_test("marpa_g_rule_null_high_set", g, R_top_2, flag, flag);
  marpa_m_test("marpa_g_rule_null_high", g, R_top_2, flag);

  /* invalid/no such rule id error handling */
  const char *marpa_g_rule_rank_setters[] = {
    "marpa_g_rule_rank_set",
    "marpa_g_rule_null_high_set",
  };
  marpa_m_tests(marpa_g_rule_rank_setters, g, R_invalid, whatever, -2, MARPA_ERR_INVALID_RULE_ID);

  marpa_m_test("marpa_g_rule_rank_set", g, R_no_such, whatever, -2, MARPA_ERR_NO_SUCH_RULE_ID);
  marpa_m_test("marpa_g_rule_null_high_set", g, R_no_such, whatever, -1, MARPA_ERR_NO_SUCH_RULE_ID);

  const char *marpa_g_rule_rank_getters[] = {
    "marpa_g_rule_rank",
    "marpa_g_rule_null_high",
  };
  marpa_m_tests(marpa_g_rule_rank_getters, g, R_invalid, -2, MARPA_ERR_INVALID_RULE_ID);

  marpa_m_test("marpa_g_rule_null_high", g, R_invalid, -2, MARPA_ERR_INVALID_RULE_ID);
  marpa_m_test("marpa_g_rule_null_high", g, R_no_such, -1, MARPA_ERR_NO_SUCH_RULE_ID);

  marpa_g_trivial_precompute(g, S_top);
  ok(1, "precomputation succeeded");

  /* Ranks methods on precomputed grammar */
  /* setters fail */
  marpa_m_test("marpa_g_rule_rank_set", g, R_top_1, negative_rank, -2, MARPA_ERR_PRECOMPUTED);
  marpa_m_test("marpa_g_rule_rank_set", g, R_top_2, positive_rank, -2, MARPA_ERR_PRECOMPUTED);
  marpa_m_test("marpa_g_rule_null_high_set", g, R_top_2, flag, -2, MARPA_ERR_PRECOMPUTED);

  /* getters succeed */
  marpa_m_test("marpa_g_rule_rank", g, R_top_1, negative_rank);
  marpa_m_test("marpa_g_rule_rank", g, R_top_2, positive_rank);
  marpa_m_test("marpa_g_rule_null_high", g, R_top_2, flag);

  /* recreate the grammar to test event methods except nulled */
  marpa_g_unref(g);
  g = marpa_g_trivial_new(&marpa_configuration);

  /* Events */
  /* test that attempts to create events, other than nulled events,
     results in a reasonable error -- http://irclog.perlgeek.de/marpa/2015-02-13#i_10111838 */
  int reactivate;
  int value;
  Marpa_Symbol_ID S_predicted, S_completed;

  /* completion */
  S_completed = S_B1;

  value = 0;
  marpa_m_test("marpa_g_symbol_is_completion_event_set", g, S_completed, value, value);
  marpa_m_test("marpa_g_symbol_is_completion_event", g, S_completed, value);

  value = 1;
  marpa_m_test("marpa_g_symbol_is_completion_event_set", g, S_completed, value, value);
  marpa_m_test("marpa_g_symbol_is_completion_event", g, S_completed, value);

  reactivate = 0;
  marpa_m_test("marpa_g_completion_symbol_activate", g, S_completed, reactivate, reactivate);

  reactivate = 1;
  marpa_m_test("marpa_g_completion_symbol_activate", g, S_completed, reactivate, reactivate);

  /* prediction */
  S_predicted = S_A1;

  value = 0;
  marpa_m_test("marpa_g_symbol_is_prediction_event_set", g, S_predicted, value, value);
  marpa_m_test("marpa_g_symbol_is_prediction_event", g, S_predicted, value);

  value = 1;
  marpa_m_test("marpa_g_symbol_is_prediction_event_set", g, S_predicted, value, value);
  marpa_m_test("marpa_g_symbol_is_prediction_event", g, S_predicted, value);

  reactivate = 0;
  marpa_m_test("marpa_g_prediction_symbol_activate", g, S_predicted, reactivate, reactivate);

  reactivate = 1;
  marpa_m_test("marpa_g_prediction_symbol_activate", g, S_predicted, reactivate, reactivate);

  /* completion on predicted symbol */
  value = 1;
  marpa_m_test("marpa_g_symbol_is_completion_event_set", g, S_predicted, value, value);
  marpa_m_test("marpa_g_symbol_is_completion_event", g, S_predicted, value);

  /* prediction on completed symbol */
  value = 1;
  marpa_m_test("marpa_g_symbol_is_prediction_event_set", g, S_completed, value, value);
  marpa_m_test("marpa_g_symbol_is_prediction_event", g, S_completed, value);

  /* invalid/no such symbol IDs */
  const char *marpa_g_event_setters[] = {
    "marpa_g_symbol_is_completion_event_set", "marpa_g_completion_symbol_activate",
    "marpa_g_symbol_is_prediction_event_set","marpa_g_prediction_symbol_activate",

  };
  marpa_m_tests(marpa_g_event_setters, g, S_invalid, whatever, -2, MARPA_ERR_INVALID_SYMBOL_ID);
  marpa_m_tests(marpa_g_event_setters, g, S_no_such, whatever, -1, MARPA_ERR_NO_SUCH_SYMBOL_ID);

  const char *marpa_g_event_getters[] = {
    "marpa_g_symbol_is_completion_event", "marpa_g_symbol_is_prediction_event",
  };

  marpa_m_tests(marpa_g_event_getters, g, S_invalid, -2, MARPA_ERR_INVALID_SYMBOL_ID);
  marpa_m_tests(marpa_g_event_getters, g, S_no_such, -1, MARPA_ERR_NO_SUCH_SYMBOL_ID);

  /* precomputation */
  marpa_g_trivial_precompute(g, S_top);
  ok(1, "precomputation succeeded");

  /* event methods after precomputation */
  marpa_m_tests(marpa_g_event_setters, g, whatever, whatever, -2, MARPA_ERR_PRECOMPUTED);

  marpa_m_test("marpa_g_symbol_is_prediction_event", g, S_predicted, value);
  marpa_m_test("marpa_g_symbol_is_completion_event", g, S_completed, value);

  /* Recognizer Methods */
  {
    r = marpa_r_new (g);
    if (!r)
      fail("marpa_r_new", g);

    /* the recce hasn't been started yet */
    marpa_m_test("marpa_r_current_earleme", r, -1, MARPA_ERR_RECCE_NOT_STARTED);
    marpa_m_test("marpa_r_progress_report_reset", r, -2, MARPA_ERR_RECCE_NOT_STARTED);
    marpa_m_test("marpa_r_progress_report_start", r, whatever, -2, MARPA_ERR_RECCE_NOT_STARTED);
    marpa_m_test("marpa_r_progress_report_finish", r, -2, MARPA_ERR_RECCE_NOT_STARTED);
    test_r_progress_item( g, r, -2, MARPA_ERR_RECCE_NOT_STARTED);

    /* start the recce */
    rc = marpa_r_start_input (r);
    if (!rc)
      fail("marpa_r_start_input", g);

    diag ("The below recce tests are at earleme 0");

    { /* event loop -- just count events so far -- there must be no event except exhausted */
      Marpa_Event event;
      int exhausted_event_triggered = 0;
      int spurious_events = 0;
      int prediction_events = 0;
      int completion_events = 0;
      int event_ix;
      const int event_count = marpa_g_event_count (g);

      is_int(1, event_count, "event count at earleme 0 is %ld", (long) event_count);

      for (event_ix = 0; event_ix < event_count; event_ix++)
      {
        int event_type = marpa_g_event (g, &event, event_ix);
        if (event_type == MARPA_EVENT_SYMBOL_COMPLETED)
          completion_events++;
        else if (event_type == MARPA_EVENT_SYMBOL_PREDICTED)
          prediction_events++;
        else if (event_type == MARPA_EVENT_EXHAUSTED)
          exhausted_event_triggered++;
        else
        {
          printf ("spurious event type is %ld\n", (long) event_type);
          spurious_events++;
        }
      }

      is_int(0, spurious_events, "spurious events triggered: %ld", (long) spurious_events);
      is_int(0, completion_events, "completion events triggered: %ld", (long) completion_events);
      is_int(0, prediction_events, "completion events triggered: %ld", (long) prediction_events);
      ok (exhausted_event_triggered, "exhausted event triggered");

    } /* event loop */

    /* recognizer reading methods */
    Marpa_Symbol_ID S_token = S_A2;
    marpa_m_test("marpa_r_alternative", r, S_invalid, 0, 0,
      MARPA_ERR_RECCE_NOT_ACCEPTING_INPUT, "not accepting input is checked before invalid symbol");
    marpa_m_test("marpa_r_alternative", r, S_no_such, 0, 0,
      MARPA_ERR_RECCE_NOT_ACCEPTING_INPUT, "not accepting input is checked before no such symbol");
    marpa_m_test("marpa_r_alternative", r, S_token, 0, 0,
      MARPA_ERR_RECCE_NOT_ACCEPTING_INPUT, "not accepting input");
    marpa_m_test("marpa_r_earleme_complete", r, -2, MARPA_ERR_RECCE_NOT_ACCEPTING_INPUT);

    marpa_m_test("marpa_r_is_exhausted", r, 1, "at earleme 0");

    /* Location accessors */
    {
      /* the below 2 always succeed */
      unsigned int current_earleme = 0;
      marpa_m_test("marpa_r_current_earleme", r, current_earleme);

      unsigned int furthest_earleme = current_earleme;
      marpa_m_test("marpa_r_furthest_earleme", r, furthest_earleme);

      marpa_m_test("marpa_r_latest_earley_set", r, furthest_earleme);
      marpa_m_test("marpa_r_earleme", r, current_earleme, current_earleme);
      marpa_m_test("marpa_r_earley_set_value", r, current_earleme, -1, MARPA_ERR_RECCE_NOT_ACCEPTING_INPUT);

      /* marpa_r_earley_set_value_*() methods */
      int taxicab = 1729;

      int earley_set;

      struct marpa_r_earley_set_value_test {
        int earley_set;

        int rv_marpa_r_earleme;
        int rv_marpa_r_latest_earley_set_value_set;
        int rv_marpa_r_earley_set_value;
        int rv_marpa_r_latest_earley_set_values_set;

        int   rv_marpa_r_earley_set_values;
        int   int_p_value_rv_marpa_r_earley_set_values;
        void* void_p_value_rv_marpa_r_earley_set_values;

        Marpa_Error_Code errcode;
      };
      typedef struct marpa_r_earley_set_value_test Marpa_R_Earley_Set_Value_Test;

      const Marpa_R_Earley_Set_Value_Test tests[] = {
        { -1, -2, taxicab,      -2, 1, -2, taxicab, value2, MARPA_ERR_INVALID_LOCATION },
        {  0,  0, taxicab, taxicab, 1,  1,      42, value2, MARPA_ERR_INVALID_LOCATION },
        {  1, -2,      42,      -2, 1, -2,      42, value2, MARPA_ERR_NO_EARLEY_SET_AT_LOCATION },
        {  2, -2,      42,      -2, 1, -2,      42, value2, MARPA_ERR_NO_EARLEY_SET_AT_LOCATION },
      };

      for (ix = 0; ix < sizeof(tests) / sizeof(Marpa_R_Earley_Set_Value_Test); ix++)
        {
          const Marpa_R_Earley_Set_Value_Test t = tests[ix];
          diag("marpa_r_earley_set_value_*() methods, earley_set: %d", t.earley_set);

          if (t.earley_set == -1 || t.earley_set == 1 || t.earley_set == 2)
            marpa_m_test("marpa_r_earleme", r, t.earley_set, t.rv_marpa_r_earleme, t.errcode);
          else
            marpa_m_test("marpa_r_earleme", r, t.earley_set, t.rv_marpa_r_earleme);

          diag("Trying marpa_r_latest_earley_set_value_set(); earley_set: %d; value: %d", t.earley_set, taxicab);
          marpa_m_test("marpa_r_latest_earley_set_value_set", r,
            t.rv_marpa_r_latest_earley_set_value_set,
            t.rv_marpa_r_latest_earley_set_value_set);
          is_int(t.errcode, marpa_g_error(g, NULL),
            "marpa_r_latest_earley_set_value_set() error code");

          if (t.earley_set == -1 || t.earley_set == 1 || t.earley_set == 2)
            marpa_m_test("marpa_r_earley_set_value", r,
              t.earley_set, t.rv_marpa_r_earley_set_value, t.errcode);
          else
            marpa_m_test("marpa_r_earley_set_value", r,
              t.earley_set, t.rv_marpa_r_earley_set_value);

          diag("Trying marpa_r_latest_earley_set_values_set(); int value: %d",
	    taxicab);
	  test_r_latest_earley_set_values_set(g, r,
	      t.rv_marpa_r_latest_earley_set_values_set, 42, value2);
	  is_int(t.errcode, marpa_g_error(g, NULL),
	    "marpa_r_latest_earley_set_values_set() error code");

	  test_r_earley_set_values(g, r,
	     t.earley_set,
	     t.rv_marpa_r_earley_set_values,
	     t.int_p_value_rv_marpa_r_earley_set_values,
	     t.void_p_value_rv_marpa_r_earley_set_values,
	     t.errcode);

        }
    } /* Location Accessors */

    /* Other parse status methods */
    {
      int boolean = 0;
      marpa_m_test("marpa_r_prediction_symbol_activate", r, S_predicted, boolean, boolean );
      marpa_m_test("marpa_r_prediction_symbol_activate", r, S_invalid, boolean,
        -2, MARPA_ERR_INVALID_SYMBOL_ID);
      marpa_m_test("marpa_r_prediction_symbol_activate", r, S_no_such, boolean,
        -1, MARPA_ERR_NO_SUCH_SYMBOL_ID);

      reactivate = 1;
      marpa_m_test("marpa_r_completion_symbol_activate", r, S_completed, reactivate, reactivate );
      marpa_m_test("marpa_r_completion_symbol_activate", r, S_invalid, reactivate,
        -2, MARPA_ERR_INVALID_SYMBOL_ID);
      marpa_m_test("marpa_r_completion_symbol_activate", r, S_no_such, reactivate,
        -1, MARPA_ERR_NO_SUCH_SYMBOL_ID);

      boolean = 1;
      Marpa_Symbol_ID S_nulled = S_C1;
      marpa_m_test("marpa_r_nulled_symbol_activate", r, S_nulled, boolean, boolean );
      marpa_m_test("marpa_r_nulled_symbol_activate", r, S_invalid, boolean,
        -2, MARPA_ERR_INVALID_SYMBOL_ID);
      marpa_m_test("marpa_r_nulled_symbol_activate", r, S_no_such, boolean,
        -1, MARPA_ERR_NO_SUCH_SYMBOL_ID);

      int threshold = 1;
      marpa_m_test("marpa_r_earley_item_warning_threshold_set", r, threshold, threshold);
      marpa_m_test("marpa_r_earley_item_warning_threshold", r, threshold);

      Marpa_Symbol_ID S_expected = S_C1;
      value = 1;
      marpa_m_test("marpa_r_expected_symbol_event_set", r, S_B1, value,
        -2, MARPA_ERR_SYMBOL_IS_NULLING);

      test_r_terminals_expected(g, r, 0);

      marpa_m_test("marpa_r_terminal_is_expected", r, S_C1, 0);
      marpa_m_test("marpa_r_terminal_is_expected", r, S_invalid,
        -2, MARPA_ERR_INVALID_SYMBOL_ID);
      marpa_m_test("marpa_r_terminal_is_expected", r, S_no_such,
        -2, MARPA_ERR_NO_SUCH_SYMBOL_ID);
    } /* Other parse status methods */

    /* Progress reports */
    {
      marpa_m_test("marpa_r_progress_report_reset", r,
        -2, MARPA_ERR_PROGRESS_REPORT_NOT_STARTED);

      marpa_m_test("marpa_r_progress_report_finish", r,
        -2, MARPA_ERR_PROGRESS_REPORT_NOT_STARTED);

      test_r_progress_item( g, r, -2, MARPA_ERR_PROGRESS_REPORT_NOT_STARTED);

      /* start report at bad locations */
      Marpa_Earley_Set_ID ys_id_negative = -1;
      marpa_m_test("marpa_r_progress_report_start", r, ys_id_negative,
        -2, MARPA_ERR_INVALID_LOCATION);

      Marpa_Earley_Set_ID ys_id_not_existing = 1;
      marpa_m_test("marpa_r_progress_report_start", r, ys_id_not_existing,
        -2, MARPA_ERR_NO_EARLEY_SET_AT_LOCATION);

      /* start report at earleme 0 */
      Marpa_Earley_Set_ID earleme_0 = 0;
      marpa_m_test("marpa_r_progress_report_start", r, earleme_0, 0, "no items at earleme 0");

      test_r_progress_item( g, r, -1, MARPA_ERR_PROGRESS_REPORT_EXHAUSTED);

      int non_negative_value = 1;
      marpa_m_test("marpa_r_progress_report_reset", r, non_negative_value);

      marpa_m_test("marpa_r_progress_report_finish", r, non_negative_value, "at earleme 0");
    }

    /* Bocage, Order, Tree, Value */
    {
      /* Bocage */
      Marpa_Earley_Set_ID ys_invalid = -2;
      marpa_m_test("marpa_b_new", r, ys_invalid, NULL, MARPA_ERR_INVALID_LOCATION);

      Marpa_Earley_Set_ID ys_non_existing = 1;
      marpa_m_test("marpa_b_new", r, ys_non_existing, NULL, MARPA_ERR_NO_PARSE);

      Marpa_Earley_Set_ID ys_at_current_earleme = -1;
      Marpa_Bocage b = marpa_b_new(r, ys_at_current_earleme);
      if (!b)
        fail("marpa_b_new", g);
      else
        ok(1, "marpa_b_new(): parse at current earleme of trivial parse");

      marpa_b_unref(b);

      b = marpa_b_new(r, 0);

      if (!b)
        fail("marpa_b_new", g);
      else
        ok(1, "marpa_b_new(): null parse at earleme 0");

      marpa_m_test("marpa_b_ambiguity_metric", b, 1);
      marpa_m_test("marpa_b_is_null", b, 1);

      /* Order */
      Marpa_Order o = marpa_o_new (b);

      if (!o)
        fail("marpa_o_new", g);
      else
        ok(1, "marpa_o_new() at earleme 0");

      int flag = 1;
      marpa_m_test("marpa_o_high_rank_only_set", o, flag, flag);
      marpa_m_test("marpa_o_high_rank_only", o, flag);

      marpa_m_test("marpa_o_ambiguity_metric", o, 1);
      marpa_m_test("marpa_o_is_null", o, 1);

      marpa_m_test("marpa_o_high_rank_only_set", o, flag, -2, MARPA_ERR_ORDER_FROZEN);
      marpa_m_test("marpa_o_high_rank_only", o, flag);

      /* Tree */
      Marpa_Tree t;

      t = marpa_t_new (o);
      if (!t)
        fail("marpa_t_new", g);
      else
        ok(1, "marpa_t_new() at earleme 0");

      marpa_m_test("marpa_t_parse_count", t, 0, "before the first parse tree");

      marpa_m_test("marpa_t_next", t, 0);

      /* Value */
      Marpa_Value v = marpa_v_new(t);
      if (!t)
        fail("marpa_v_new", g);
      else
        ok(1, "marpa_v_new() at earleme 0");

      int step_inactive_count = 0;
      int step_initial_count = 0;
      int step_token_count = 0;
      int step_rule_count = 0;
      int step_nulling_symbol_count = 0;
      while (1)
      {
        Marpa_Step_Type step_type = marpa_v_step (v);
        Marpa_Symbol_ID token;

        if (step_type < 0)
            fail("marpa_v_step", g);

        if (step_type == MARPA_STEP_INACTIVE)
        {
            step_inactive_count++;
            break;
        }

        switch (step_type)
        {
          case MARPA_STEP_INITIAL:
            step_initial_count++;
            break;
          case MARPA_STEP_TOKEN:
            step_token_count++;
            break;
          case MARPA_STEP_RULE:
            step_rule_count++;
            break;
          case MARPA_STEP_NULLING_SYMBOL:
            step_nulling_symbol_count++;
            break;
         }
      }
      is_int(1, step_inactive_count, "MARPA_STEP_INACTIVE seen once.");
      is_int(0, step_initial_count, "MARPA_STEP_INITIAL not seen.");
      is_int(0, step_token_count, "MARPA_STEP_TOKEN not seen.");
      is_int(0, step_rule_count, "MARPA_STEP_RULE not seen.");
      is_int(0, step_nulling_symbol_count, "MARPA_STEP_NULLING_SYMBOL not seen.");

      marpa_m_test("marpa_t_parse_count", t, 1);
      marpa_m_test("marpa_t_next", t, -2, MARPA_ERR_TREE_PAUSED);

      marpa_v_unref(v);

      marpa_m_test("marpa_t_parse_count", t, 1);
      marpa_m_test("marpa_t_next", t, -1, MARPA_ERR_TREE_EXHAUSTED);

    } /* Bocage, Order, Tree, Value */

  } /* recce method tests */

  return 0;
}
