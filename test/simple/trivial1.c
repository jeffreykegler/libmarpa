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

/* check that method_name returned -2 and
   error code is set to MARPA_ERR_INVALID_SYMBOL_ID
   */
#define MARPA_TEST_MSG_NO_START_SYMBOL "fail before marpa_g_start_symbol_set()"
#define MARPA_TEST_MSG_INVALID_SYMBOL_ID "malformed symbol id"
#define MARPA_TEST_MSG_NO_SUCH_SYMBOL_ID "invalid symbol id"
static int
is_failure_invalid_symbol_id(Marpa_Grammar g, int retcode, char *method_name)
{
  return is_failure(g, MARPA_ERR_INVALID_SYMBOL_ID, -2, retcode, method_name,
    MARPA_TEST_MSG_INVALID_SYMBOL_ID);
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

/* Marpa method test interface */

typedef int (*marpa_m_pointer)();

/*
    %s -- Marpa_Symbol_ID
    %r -- Marpa_Rule_ID
    %n -- Marpa_Rank
    ...
*/
typedef char *marpa_m_argspec;
typedef char *marpa_m_name;

struct marpa_method_spec {
  marpa_m_name n;
  marpa_m_pointer p;
  marpa_m_argspec as;
};

typedef struct marpa_method_spec Marpa_Method_Spec;

const Marpa_Method_Spec methspec[] = {

  { "marpa_g_start_symbol_set", &marpa_g_start_symbol_set, "%s" },
  { "marpa_g_symbol_is_start", &marpa_g_symbol_is_start, "%s" },
  { "marpa_g_start_symbol", &marpa_g_start_symbol, "" },

  { "marpa_g_symbol_is_terminal_set", &marpa_g_symbol_is_terminal_set, "%s, %i" },
  { "marpa_g_symbol_is_terminal",  &marpa_g_symbol_is_terminal, "%s" },

  { "marpa_g_highest_symbol_id", &marpa_g_highest_symbol_id, ""},

  { "marpa_g_symbol_is_accessible", &marpa_g_symbol_is_accessible, "%s" },
  { "marpa_g_symbol_is_nullable", &marpa_g_symbol_is_nullable, "%s" },
  { "marpa_g_symbol_is_nulling", &marpa_g_symbol_is_nulling, "%s" },
  { "marpa_g_symbol_is_productive", &marpa_g_symbol_is_productive, "%s" },

  { "marpa_g_rule_is_nullable", &marpa_g_rule_is_nullable, "%r" },
  { "marpa_g_rule_is_nulling", &marpa_g_rule_is_nulling, "%r" },
  { "marpa_g_rule_is_loop", &marpa_g_rule_is_loop, "%r" },

  { "marpa_g_precompute", &marpa_g_precompute, "" },
};

static Marpa_Method_Spec
marpa_m_method_spec(const char *name)
{
  Marpa_Method_Spec ms;
  int i;

  for (i = 0; i < sizeof(methspec) / sizeof(Marpa_Method_Spec); i++)
  {
    if ( strcmp(name, methspec[i].n ) == 0 )
    {
      return methspec[i];
    }
  }
  printf("No spec yet for Marpa method %s().\n", name);
  exit(1);
}

typedef char *marpa_m_errmsg;
struct marpa_m_error {
  Marpa_Error_Code c;
  marpa_m_errmsg m;
};

typedef struct marpa_m_error Marpa_Method_Error;

const Marpa_Method_Error errspec[] = {
  { MARPA_ERR_NO_START_SYMBOL, "no start symbol" },
  { MARPA_ERR_INVALID_SYMBOL_ID, "invalid symbol id" },
  { MARPA_ERR_NO_SUCH_SYMBOL_ID, "no such symbol id" },
  { MARPA_ERR_NOT_PRECOMPUTED, "grammar not precomputed" },
  { MARPA_ERR_TERMINAL_IS_LOCKED, "terminal locked" },
  { MARPA_ERR_NULLING_TERMINAL, "nulling terminal" },
  { MARPA_ERR_PRECOMPUTED, "grammar precomputed" },
};

static char *marpa_m_error_message (Marpa_Error_Code error_code)
{
  Marpa_Method_Error me;
  int i;

  for (i = 0; i < sizeof(errspec) / sizeof(Marpa_Method_Error); i++)
  {
    if ( error_code == errspec[i].c )
    {
      return errspec[i].m;
    }
  }
  printf("No message yet for Marpa error code %d.\n", error_code);
  exit(1);
}

static int
marpa_m_test(const char* name, ...)
{
  Marpa_Method_Spec ms;

  Marpa_Grammar g;
  Marpa_Recognizer r;

  Marpa_Symbol_ID S_id;
  Marpa_Rule_ID R_id;
  int intarg;

  int rv_wanted, rv_seen;
  int err_wanted, err_seen;

  char tok_buf[32];  /* strtok() */
  char desc_buf[80]; /* test description  */
  char *curr_arg;

  ms = marpa_m_method_spec(name);

  va_list args;
  va_start(args, name);

  g = NULL;
  if (strncmp(name, "marpa_g_", 8) == 0)
    g = va_arg(args, Marpa_Grammar);

  /* unpack arguments */
  if (ms.as == "")
  {
    /* dispatch based on what object is set */
    if (g != NULL) rv_seen = ms.p(g);
    else if (r != NULL) rv_seen = ms.p(r);
  }
  else
  {
    strcpy( tok_buf, ms.as );
    curr_arg = strtok(tok_buf, " ,-");
    while (curr_arg != NULL)
    {
      if (strncmp(curr_arg, "%s", 2) == 0) S_id = va_arg(args, Marpa_Symbol_ID);
      else if (strncmp(curr_arg, "%r", 2) == 0) R_id   = va_arg(args, Marpa_Rule_ID);
      else if (strncmp(curr_arg, "%i", 2) == 0) intarg = va_arg(args, int);

      curr_arg = strtok(NULL, " ,-");
    }
    /* call marpa method based on signature */
    if (strcmp(ms.as, "%s") == 0) rv_seen = ms.p(g, S_id);
    if (strcmp(ms.as, "%r") == 0) rv_seen = ms.p(g, R_id);
    else if (strcmp(ms.as, "%s, %i") == 0) rv_seen = ms.p(g, S_id, intarg);
  }

  rv_wanted = va_arg(args, int);

  /* success wanted */
  if ( rv_wanted >= 0 )
  {
    /* failure seen */
    if ( rv_seen < 0 )
    {
      sprintf(msgbuf, "%s() unexpectedly returned %d.", name, rv_seen);
      ok(0, msgbuf);
    }
    /* success seen */
    else {
      sprintf(desc_buf, "%s() succeeded", name);
      is_int( rv_wanted, rv_seen, desc_buf );
    }
  }
  /* failure wanted */
  else
  {
    err_wanted = va_arg(args, int);

    if (g == NULL)
      g = va_arg(args, Marpa_Grammar);

    err_seen = marpa_g_error(g, NULL);

    sprintf(desc_buf, "%s() failed, returned %d", name, rv_seen);
    is_int( rv_wanted, rv_seen, desc_buf );

    sprintf(desc_buf, "%s() error: %s", name, marpa_m_error_message(err_seen));
    is_int( err_wanted, err_seen, desc_buf );
  }

  va_end(args);
}

int
main (int argc, char *argv[])
{
  int rc;

  Marpa_Config marpa_configuration;

  Marpa_Grammar g;
  Marpa_Recognizer r;

  Marpa_Symbol_ID S_invalid, S_no_such;
  Marpa_Rank rank;
  int flag;

  int reactivate;
  int value;

  plan_lazy();

  marpa_c_init (&marpa_configuration);
  g = marpa_g_trivial_new(&marpa_configuration);

  /* Grammar Methods per sections of api.texi: Symbols, Rules, Sequnces, Ranks, Events */
  S_invalid = -1;
  S_no_such = 42;
  /* these must soft fail if there is not start symbol */
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
  marpa_m_test("marpa_g_symbol_is_accessible", g, S_C2, -2, MARPA_ERR_NOT_PRECOMPUTED);
  marpa_m_test("marpa_g_symbol_is_nullable", g, S_A1, -2, MARPA_ERR_NOT_PRECOMPUTED);
  marpa_m_test("marpa_g_symbol_is_nulling", g, S_A1, -2, MARPA_ERR_NOT_PRECOMPUTED);
  marpa_m_test("marpa_g_symbol_is_productive", g, S_top, -2, MARPA_ERR_NOT_PRECOMPUTED);
  marpa_m_test("marpa_g_symbol_is_terminal", g, S_top, 0);

  /* Rules */
  marpa_m_test("marpa_g_rule_is_nullable", g, R_top_2, -2, MARPA_ERR_NOT_PRECOMPUTED);
  marpa_m_test("marpa_g_rule_is_nulling", g, R_top_2, -2, MARPA_ERR_NOT_PRECOMPUTED);
  marpa_m_test("marpa_g_rule_is_loop", g, R_C2_3, -2, MARPA_ERR_NOT_PRECOMPUTED);

  /* marpa_g_symbol_is_terminal_set() on invalid and non-existing symbol IDs
     on a non-precomputed grammar */
  marpa_m_test("marpa_g_symbol_is_terminal_set", g, S_invalid, 1, -2, MARPA_ERR_INVALID_SYMBOL_ID);
  marpa_m_test("marpa_g_symbol_is_terminal_set", g, S_no_such, 1, -1, MARPA_ERR_NO_SUCH_SYMBOL_ID);

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
  g = marpa_g_trivial_new(&marpa_configuration);
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
#define MARPA_TEST_MSG_INVALID_RULE_ID "malformed rule id"
#define MARPA_TEST_MSG_NO_SUCH_RULE_ID "invalid rule id"
  /* malformed rule/symbol id */
  is_failure(g, MARPA_ERR_INVALID_RULE_ID, -2, marpa_g_rule_is_proper_separation (g, -1),
    "marpa_g_rule_is_proper_separation", MARPA_TEST_MSG_INVALID_RULE_ID);
  is_failure(g, MARPA_ERR_INVALID_RULE_ID, -2, marpa_g_sequence_min (g, -1),
    "marpa_g_sequence_min", MARPA_TEST_MSG_INVALID_RULE_ID);
  is_failure(g, MARPA_ERR_INVALID_RULE_ID, -2, marpa_g_sequence_separator (g, -1),
    "marpa_g_sequence_separator", MARPA_TEST_MSG_INVALID_RULE_ID);
  is_failure_invalid_symbol_id
    (g, marpa_g_symbol_is_counted (g, -1), "marpa_g_symbol_is_counted");
  /* well-formed, but invalid rule/symbol id */
  is_failure(g, MARPA_ERR_NO_SUCH_RULE_ID, -1, marpa_g_rule_is_proper_separation (g, 42),
    "marpa_g_rule_is_proper_separation", MARPA_TEST_MSG_NO_SUCH_RULE_ID);
  is_failure(g, MARPA_ERR_NO_SUCH_RULE_ID, -2, marpa_g_sequence_min (g, 42),
    "marpa_g_sequence_min", MARPA_TEST_MSG_NO_SUCH_RULE_ID);
  is_failure(g, MARPA_ERR_NO_SUCH_RULE_ID, -2, marpa_g_sequence_separator (g, 42),
    "marpa_g_sequence_separator", MARPA_TEST_MSG_NO_SUCH_RULE_ID);
  is_failure(g, MARPA_ERR_NO_SUCH_SYMBOL_ID, -1, marpa_g_symbol_is_counted (g, 42),
    "marpa_g_symbol_is_counted", MARPA_TEST_MSG_NO_SUCH_SYMBOL_ID);

  /* Ranks */
  rank = -2;
  is_success(g, rank, marpa_g_rule_rank_set (g, R_top_1, rank), "marpa_g_rule_rank_set()");
  is_success(g, rank, marpa_g_rule_rank (g, R_top_1), "marpa_g_rule_rank()");
  flag = 1;
  is_success(g, flag, marpa_g_rule_null_high_set (g, R_top_2, flag), "marpa_g_rule_null_high_set()");
  is_success(g, flag, marpa_g_rule_null_high (g, R_top_2), "marpa_g_rule_null_high()");

  is_failure(g, MARPA_ERR_INVALID_RULE_ID, -2, marpa_g_rule_rank_set (g, -1, rank),
    "marpa_g_rule_rank_set", MARPA_TEST_MSG_INVALID_RULE_ID);
  is_failure(g, MARPA_ERR_INVALID_RULE_ID, -2, marpa_g_rule_rank (g, -1),
    "marpa_g_rule_rank", MARPA_TEST_MSG_INVALID_RULE_ID);
  is_failure(g, MARPA_ERR_INVALID_RULE_ID, -2, marpa_g_rule_null_high_set (g, -1, flag),
    "marpa_g_rule_null_high_set", MARPA_TEST_MSG_INVALID_RULE_ID);
  is_failure(g, MARPA_ERR_INVALID_RULE_ID, -2, marpa_g_rule_null_high (g, -1),
    "marpa_g_rule_null_high", MARPA_TEST_MSG_INVALID_RULE_ID);

  is_failure(g, MARPA_ERR_NO_SUCH_RULE_ID, -2, marpa_g_rule_rank_set (g, 42, rank),
    "marpa_g_rule_rank_set", MARPA_TEST_MSG_NO_SUCH_RULE_ID);
  is_failure(g, MARPA_ERR_NO_SUCH_RULE_ID, -2, marpa_g_rule_rank (g, 42),
    "marpa_g_rule_rank", MARPA_TEST_MSG_NO_SUCH_RULE_ID);
  is_failure(g, MARPA_ERR_NO_SUCH_RULE_ID, -1, marpa_g_rule_null_high_set (g, 42, flag),
    "marpa_g_rule_null_high_set", MARPA_TEST_MSG_NO_SUCH_RULE_ID);
  is_failure(g, MARPA_ERR_NO_SUCH_RULE_ID, -1, marpa_g_rule_null_high (g, 42),
    "marpa_g_rule_null_high", MARPA_TEST_MSG_NO_SUCH_RULE_ID);

  marpa_g_trivial_precompute(g, S_top);
  ok(1, "precomputation succeeded");

  /* Ranks methods on precomputed grammar */
  is_failure(g, MARPA_ERR_PRECOMPUTED, -2, marpa_g_rule_rank_set (g, R_top_1, rank),
    "marpa_g_rule_rank_set", "on precomputed grammar");
  is_success(g, rank, marpa_g_rule_rank (g, R_top_1), "marpa_g_rule_rank()");
  is_failure(g, MARPA_ERR_PRECOMPUTED, -2, marpa_g_rule_null_high_set (g, R_top_2, flag),
    "marpa_g_rule_null_high_set", "on precomputed grammar");
  is_success(g, 1, marpa_g_rule_null_high (g, R_top_2), "marpa_g_rule_null_high()");

  /* recreate the grammar to test event methods except nulled */
  marpa_g_unref(g);
  g = marpa_g_trivial_new(&marpa_configuration);

  /* Events */
  /* test that attempts to create events, other than nulled events,
     results in a reasonable error -- http://irclog.perlgeek.de/marpa/2015-02-13#i_10111838 */
/*

reactivate = 1;
reactivate = 0;
int marpa_g_completion_symbol_activate (g, S_top, reactivate )
int marpa_g_prediction_symbol_activate (g, S_top, reactivate )

// If the active status of the completion event for sym_id cannot be set as
// indicated by reactivate, the method fails. On failure, -2 is returned.

value = 1;
int marpa_g_symbol_is_completion_event (g, S_top)
int marpa_g_symbol_is_completion_event_set ( g, S_top, value)

int marpa_g_symbol_is_prediction_event (g, S_top)
int marpa_g_symbol_is_prediction_event_set (g, S_top, value)

// On success, 1 if symbol sym_id is an event symbol after the call, 0 otherwise.
// If sym_id is well-formed, but there is no such symbol, -1.

// malformed/invalid symbols

*/

  marpa_g_trivial_precompute(g, S_top);
  ok(1, "precomputation succeeded");

  /* event methods after precomputation
     if the grammar g is precomputed; or on other failure, -2.
   */

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
