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

  if (retval < 0)
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

  plan_lazy();

  marpa_c_init (&marpa_configuration);
  g = trivial_grammar(&marpa_configuration);
  
  /* these must soft fail if there is not start symbol */
#define NO_START_TEST_MSG "fail before marpa_g_start_symbol_set()"

  is_failure(g, MARPA_ERR_INVALID_SYMBOL_ID, -2, marpa_g_symbol_is_start (g, -1), "marpa_g_symbol_is_start", "symbol is not well-formed");
  is_failure(g, MARPA_ERR_NO_SUCH_SYMBOL_ID, -1, marpa_g_symbol_is_start (g, 150), "marpa_g_symbol_is_start", "symbol doesn't exist");
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
  
  /* set a nulling symbol to be terminal and test precomputation failure */
  is_success(g, 1, marpa_g_symbol_is_terminal_set(g, S_C1, 1), 
    "marpa_g_symbol_is_terminal_set()");
  is_failure(g, MARPA_ERR_TERMINAL_IS_LOCKED, -2, marpa_g_symbol_is_terminal_set(g, S_C1, 0), 
    "marpa_g_symbol_is_terminal_set", "on a symbol already set to be a terminal");
  is_failure(g, MARPA_ERR_NULLING_TERMINAL, -2, marpa_g_precompute (g), "marpa_g_precompute", "with a nulling terminal");
  
  /* terminals are locked after setting, so we recreate the grammar */
  marpa_g_unref(g);
  g = trivial_grammar(&marpa_configuration);
  
  is_failure(g, MARPA_ERR_NO_START_SYMBOL, -2, marpa_g_precompute (g), "marpa_g_precompute", "with a nulling terminal");

  /* set start symbol */
  (marpa_g_start_symbol_set (g, S_top) >= 0)
    || fail ("marpa_g_start_symbol_set", g);

  if (marpa_g_precompute (g) < 0)
    fail("marpa_g_precompute", g);
  ok(1, "precomputation succeeded");

  /* Grammar Methods per sections of api.texi */

  /* Symbols -- these do have @<Fail if not precomputed@>@ */
  is_success(g, 1, marpa_g_symbol_is_accessible  (g, S_C2), "marpa_g_symbol_is_accessible()");
  is_success(g, 1, marpa_g_symbol_is_nullable (g, S_A1), "marpa_g_symbol_is_nullable()");
  is_success(g, 1, marpa_g_symbol_is_nulling (g, S_A1), "marpa_g_symbol_is_nulling()");
  is_success(g, 1, marpa_g_symbol_is_productive (g, S_top), "marpa_g_symbol_is_productive()");
  is_success(g, 1, marpa_g_symbol_is_start (g, S_top), "marpa_g_symbol_is_start()");
  is_success(g, 0, marpa_g_symbol_is_terminal(g, S_top), "marpa_g_symbol_is_terminal()");

  is_failure(g, MARPA_ERR_PRECOMPUTED, -2, marpa_g_symbol_is_terminal_set(g, S_top, 0), 
    "marpa_g_symbol_is_terminal_set", "on precomputed grammar");
  
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

  /* recognizer methods */
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
