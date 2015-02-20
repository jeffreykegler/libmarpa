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

/* Libmarpa method test interface -- marpa_m_test */

#include "marpa_test.h"

Marpa_Symbol_ID S_invalid = -1, S_no_such = 42;
Marpa_Rule_ID R_invalid = -1, R_no_such = 42;

/* For (error) messages */
static char msgbuf[80];

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

  { "marpa_g_highest_rule_id", &marpa_g_highest_rule_id, "" },
  { "marpa_g_rule_is_accessible", &marpa_g_rule_is_accessible, "%r" },
  { "marpa_g_rule_is_nullable", &marpa_g_rule_is_nullable, "%r" },
  { "marpa_g_rule_is_nulling", &marpa_g_rule_is_nulling, "%r" },
  { "marpa_g_rule_is_loop", &marpa_g_rule_is_loop, "%r" },
  { "marpa_g_rule_is_productive", &marpa_g_rule_is_productive, "%r" },
  { "marpa_g_rule_length", &marpa_g_rule_length, "%r" },
  { "marpa_g_rule_lhs", &marpa_g_rule_lhs, "%r" },
  { "marpa_g_rule_rhs", &marpa_g_rule_rhs, "%r, %i" },

  { "marpa_g_sequence_new", &marpa_g_sequence_new, "%s, %s, %s, %i, %i" },
  { "marpa_g_rule_is_proper_separation", &marpa_g_rule_is_proper_separation, "%r" },
  { "marpa_g_sequence_min", &marpa_g_sequence_min, "%r" },
  { "marpa_g_sequence_separator", &marpa_g_sequence_separator, "%r" },
  { "marpa_g_symbol_is_counted", &marpa_g_symbol_is_counted, "%s" },

  { "marpa_g_rule_rank_set", &marpa_g_rule_rank_set, "%r, %i" },
  { "marpa_g_rule_rank", &marpa_g_rule_rank, "%r" },
  { "marpa_g_rule_null_high_set", &marpa_g_rule_null_high_set, "%r, %i" },
  { "marpa_g_rule_null_high", &marpa_g_rule_null_high, "%r" },

  { "marpa_g_symbol_is_completion_event_set", &marpa_g_symbol_is_completion_event_set, "%s, %i" },
  { "marpa_g_symbol_is_completion_event", &marpa_g_symbol_is_completion_event, "%s" },
  { "marpa_g_completion_symbol_activate", &marpa_g_completion_symbol_activate, "%s, %i" },

  { "marpa_g_symbol_is_prediction_event_set", &marpa_g_symbol_is_prediction_event_set, "%s, %i" },
  { "marpa_g_symbol_is_prediction_event", &marpa_g_symbol_is_prediction_event, "%s" },
  { "marpa_g_prediction_symbol_activate", &marpa_g_prediction_symbol_activate, "%s, %i" },

  { "marpa_r_expected_symbol_event_set", &marpa_r_expected_symbol_event_set, "%s, %i" },
  { "marpa_r_is_exhausted", &marpa_r_is_exhausted, "" },

  { "marpa_r_alternative", &marpa_r_alternative, "%s, %i, %i" },
  { "marpa_r_earleme_complete", &marpa_r_earleme_complete, "" },

};

static Marpa_Method_Spec
marpa_m_method_spec(const char *name)
{
  int i;
  for (i = 0; i < sizeof(methspec) / sizeof(Marpa_Method_Spec); i++)
    if ( strcmp(name, methspec[i].n ) == 0 )
      return methspec[i];
  printf("No spec yet for Marpa method %s().\n", name);
  exit(1);
}

const Marpa_Method_Error errspec[] = {
  { MARPA_ERR_NO_START_SYMBOL, "no start symbol" },
  { MARPA_ERR_INVALID_SYMBOL_ID, "invalid symbol id" },
  { MARPA_ERR_NO_SUCH_SYMBOL_ID, "no such symbol id" },
  { MARPA_ERR_NOT_PRECOMPUTED, "grammar not precomputed" },
  { MARPA_ERR_TERMINAL_IS_LOCKED, "terminal locked" },
  { MARPA_ERR_NULLING_TERMINAL, "nulling terminal" },
  { MARPA_ERR_PRECOMPUTED, "grammar precomputed" },
  { MARPA_ERR_SEQUENCE_LHS_NOT_UNIQUE, "sequence lhs not unique" },
  { MARPA_ERR_NOT_A_SEQUENCE, "not a sequence rule" },
  { MARPA_ERR_INVALID_RULE_ID, "invalid rule id" },
  { MARPA_ERR_NO_SUCH_RULE_ID, "no such rule id" },
  { MARPA_ERR_RECCE_NOT_ACCEPTING_INPUT, "recce not accepting input" },
};

static char *marpa_m_error_message (Marpa_Error_Code error_code)
{
  int i;
  for (i = 0; i < sizeof(errspec) / sizeof(Marpa_Method_Error); i++)
    if ( error_code == errspec[i].c )
      return errspec[i].m;
  printf("No message yet for Marpa error code %d.\n", error_code);
  exit(1);
}

/* we need a grammar to call marpa_g_error() */
static Marpa_Grammar marpa_m_g = NULL;

int
marpa_m_grammar_set(Marpa_Grammar g) { marpa_m_g = g; }

Marpa_Grammar
marpa_m_grammar() { return marpa_m_g; }

int
marpa_m_test(const char* name, ...)
{
  Marpa_Method_Spec ms;

  Marpa_Grammar g;
  Marpa_Recognizer r;

  Marpa_Symbol_ID S_id, S_id1, S_id2;
  Marpa_Rule_ID R_id;
  int intarg, intarg1;

  int rv_wanted, rv_seen;
  int err_wanted, err_seen;

  char tok_buf[32];  /* strtok() */
  char desc_buf[80]; /* test description  */
  char *curr_arg;
  int curr_arg_ix;

  ms = marpa_m_method_spec(name);

  va_list args;
  va_start(args, name);

  g = marpa_m_grammar();

  void *marpa_m_object = va_arg(args, void*);

#define ARG_UNDEF 42424242
  R_id = S_id = S_id1 = S_id2 = intarg = intarg1 = ARG_UNDEF;
  strcpy( tok_buf, ms.as );
  curr_arg = strtok(tok_buf, " ,-");
  while (curr_arg != NULL)
  {
    if (strncmp(curr_arg, "%s", 2) == 0){
      if (S_id == ARG_UNDEF) S_id = va_arg(args, Marpa_Symbol_ID);
      else if (S_id1 == ARG_UNDEF) S_id1 = va_arg(args, Marpa_Symbol_ID);
      else if (S_id2 == ARG_UNDEF) S_id2 = va_arg(args, Marpa_Symbol_ID);
    }
    else if (strncmp(curr_arg, "%r", 2) == 0)
    {
      R_id   = va_arg(args, Marpa_Rule_ID);
    }
    else if (strncmp(curr_arg, "%i", 2) == 0)
    {
      if (intarg == ARG_UNDEF) intarg = va_arg(args, int);
      else if (intarg1 == ARG_UNDEF) intarg1 = va_arg(args, int);
    }

    curr_arg = strtok(NULL, " ,-");
    curr_arg_ix++;
  }

  /* call marpa method based on argspec */
  if (ms.as == "") rv_seen = ms.p(marpa_m_object);
  else if (strcmp(ms.as, "%s") == 0) rv_seen = ms.p(marpa_m_object, S_id);
  else if (strcmp(ms.as, "%r") == 0) rv_seen = ms.p(marpa_m_object, R_id);
  else if (strcmp(ms.as, "%s, %i") == 0) rv_seen = ms.p(marpa_m_object, S_id, intarg);
  else if (strcmp(ms.as, "%s, %i, %i") == 0) rv_seen = ms.p(marpa_m_object, S_id, intarg, intarg1);
  else if (strcmp(ms.as, "%r, %i") == 0) rv_seen = ms.p(marpa_m_object, R_id, intarg);
  else if (strcmp(ms.as, "%s, %s, %s, %i, %i") == 0) rv_seen = ms.p(marpa_m_object, S_id, S_id1, S_id2, intarg, intarg1);
  else
  {
    printf("No method yet for argument spec %s.\n", ms.as);
    exit(1);
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
      sprintf(desc_buf, "%s()", name);
      is_int( rv_wanted, rv_seen, desc_buf );
    }
  }
  /* marpa_g_rule_rank() and marpa_g_rule_rank_set() may return negative values,
     but they are actually ranks if marpa_g_error() returns MARPA_ERR_NONE.
     So, we don't count them as failures. */
  else if ( strncmp( name, "marpa_g_rule_rank", 17 ) == 0
              && marpa_g_error(g, NULL) == MARPA_ERR_NONE )
    {
      sprintf(desc_buf, "%s() succeeded", name);
      is_int( rv_wanted, rv_seen, desc_buf );
    }
  /* failure wanted */
  else
  {
    /* return value */
    err_wanted = va_arg(args, int);
    sprintf(desc_buf, "%s() failed, returned %d", name, rv_seen);
    is_int( rv_wanted, rv_seen, desc_buf );

    /* error code */
    err_seen = marpa_g_error(g, NULL);

    if (err_seen == MARPA_ERR_NONE && rv_seen < 0)
    {
      sprintf(msgbuf, "%s(): marpa_g_error() returned MARPA_ERR_NONE, but return value was %d.", name, rv_seen);
      ok(0, msgbuf);
    }
    /* test error code */
    else
    {
      sprintf(desc_buf, "%s() error is: %s", name, marpa_m_error_message(err_seen));
      is_int( err_wanted, err_seen, desc_buf );
    }
  }

  va_end(args);
}
