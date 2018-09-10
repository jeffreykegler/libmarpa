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

/* Libmarpa method test interface -- marpa_m_test */

#include "marpa_m_test.h"

Marpa_Symbol_ID S_invalid = -1, S_no_such = 42;
Marpa_Rule_ID R_invalid = -1, R_no_such = 42;

/*

  argument specifiers:

    %s    Marpa_Symbol_ID
    %r    Marpa_Rule_ID
    %i    int

*/

const Marpa_Method_Spec methspec[] = {

  { "marpa_g_start_symbol_set", &marpa_g_start_symbol_set, "%s" },
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

  { "marpa_r_current_earleme", (marpa_m_pointer)&marpa_r_current_earleme, "" },
  { "marpa_r_furthest_earleme", (marpa_m_pointer)&marpa_r_furthest_earleme, "" },
  { "marpa_r_latest_earley_set", &marpa_r_latest_earley_set, "" },
  { "marpa_r_earleme", &marpa_r_earleme, "%i" },

  { "marpa_r_earley_set_value", &marpa_r_earley_set_value, "%i" },
  { "marpa_r_latest_earley_set_value_set", &marpa_r_latest_earley_set_value_set, "%i" },

  { "marpa_r_prediction_symbol_activate", &marpa_r_prediction_symbol_activate, "%s, %i" },
  { "marpa_r_completion_symbol_activate", &marpa_r_completion_symbol_activate, "%s, %i" },
  { "marpa_r_nulled_symbol_activate", &marpa_r_nulled_symbol_activate, "%s, %i" },

  { "marpa_r_earley_item_warning_threshold_set", &marpa_r_earley_item_warning_threshold_set, "%i" },
  { "marpa_r_earley_item_warning_threshold", &marpa_r_earley_item_warning_threshold, "" },

  { "marpa_r_expected_symbol_event_set", &marpa_r_expected_symbol_event_set, "%s, %i" },
  { "marpa_r_terminal_is_expected", &marpa_r_terminal_is_expected, "%s" },

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
  { MARPA_ERR_NONE, "no error" },
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
  { MARPA_ERR_TOKEN_LENGTH_LE_ZERO, "token length less than zero" },
  { MARPA_ERR_PARSE_EXHAUSTED, "parse exhausted" },
  { MARPA_ERR_NO_EARLEY_SET_AT_LOCATION, "no earley set at location" },
  { MARPA_ERR_INVALID_LOCATION, "location not valid" },
  { MARPA_ERR_SYMBOL_IS_NULLING, "symbol is nulling" },
  { MARPA_ERR_RECCE_NOT_STARTED, "recce not started" },
  { MARPA_ERR_PROGRESS_REPORT_NOT_STARTED, "progress report not started" },
  { MARPA_ERR_INVALID_LOCATION, "invalid location" },
  { MARPA_ERR_NO_EARLEY_SET_AT_LOCATION, "no earley set at location" },
  { MARPA_ERR_PROGRESS_REPORT_EXHAUSTED, "progress report exhausted" },
  { MARPA_ERR_NO_PARSE, "no parse" },
  { MARPA_ERR_ORDER_FROZEN, "order frozen" },
  { MARPA_ERR_TREE_EXHAUSTED, "tree exhausted" },
  { MARPA_ERR_TREE_PAUSED, "tree paused" },
  { MARPA_ERR_RHS_IX_OOB, "rhs index out of bounds" },
  { MARPA_ERR_RHS_IX_NEGATIVE, "rhs index negative" },
};

char *marpa_m_error_message (Marpa_Error_Code error_code)
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

/*
// if a method returns a value, which is NOT an error code
// Note: passing anything except char* as optional_message will dump core
//       because one can't check if an int contains a valid pointer
marpa_m_test(
  "marpa_method_name", method_arg1, method_arg2, ...,
  expected_return_value,
  "optional_message"
);

// if a method returns an negative error code
marpa_m_test(
  "marpa_method_name", method_arg1, method_arg2, ...,
  expected_negative_return_value, EXPECTED_ERROR_CODE,
  "optional_message"
);

// if a method returns a negative value indicating failure
// and sets an error code
marpa_m_test(
  "marpa_method_name", method_arg1, method_arg2, ...,
  expected_negative_return_value, EXPECTED_ERROR_CODE
);

*/

int
marpa_m_test_func(const char* name, ...)
{
  Marpa_Method_Spec ms;

  Marpa_Grammar g;
  Marpa_Recognizer r;

  int args[MARPA_M_MAX_ARG];
  char strtok_buf[32];
  char *curr_arg;
  int curr_arg_ix;

  int rv_wanted, rv_seen;
  int err_wanted, err_seen;

  ms = marpa_m_method_spec(name);

  va_list va_args;
  va_start(va_args, name);

  g = marpa_m_grammar();

  void *marpa_m_object = va_arg(va_args, void*);

  strcpy( strtok_buf, ms.as );
  curr_arg = strtok(strtok_buf, " ,-");
  curr_arg_ix = 0;
  while (curr_arg != NULL)
  {
    if (strcmp(curr_arg, "%s") == 0)        args[curr_arg_ix] = va_arg(va_args, Marpa_Symbol_ID);
    else if (strcmp(curr_arg, "%r") == 0)   args[curr_arg_ix] = va_arg(va_args, Marpa_Rule_ID);
    else if (strcmp(curr_arg, "%i") == 0)   args[curr_arg_ix] = va_arg(va_args, int);
    else
    {
      printf("No variable yet for argument spec %s.\n", curr_arg);
      exit(1);
    }

    curr_arg = strtok(NULL, " ,-");
    curr_arg_ix++;
  }

  /* call marpa method based on argspec */
  switch (curr_arg_ix)
  {
    case 0: rv_seen = ms.p(marpa_m_object); break;
    case 1: rv_seen = ms.p(marpa_m_object, args[0]); break;
    case 2: rv_seen = ms.p(marpa_m_object, args[0], args[1]); break;
    case 3: rv_seen = ms.p(marpa_m_object, args[0], args[1], args[2]); break;
    case 5: rv_seen = ms.p(marpa_m_object, args[0], args[1], args[2], args[3], args[4]); break;
    default:
      printf("No method yet for argument spec %s(%s), count %d.\n", name, ms.as, curr_arg_ix);
      exit(1);
  }

  rv_wanted = va_arg(va_args, int);

  /* success wanted, except for methods returning NULL on failure */
  if ( rv_wanted >= 0 && strcmp(name, "marpa_b_new") != 0 )
  {
    /* failure seen */
    if ( rv_seen < 0 )
    {
      ok(0, "%s() unexpectedly returned %d, error code: %d", name, rv_seen, marpa_g_error(g, NULL));
    }
    /* success seen */
    else {
      /* append message, if any, for methods returning error codes directly,
         e.g. marpa_r_alternative(). Passing anything except char* dumps core. */
      char *msg = va_arg(va_args, char *);
      if ((unsigned int *)msg != ARGS_END)
        is_int( rv_wanted, rv_seen, "%s(): %s", name, msg );
      else
        is_int( rv_wanted, rv_seen, "%s()", name );
    }
  }
  /* a Marpa method, e.g. marpa_g_rule_rank() and marpa_g_rule_rank_set()
   * may return a negative value, which can be distinguished from failure
   * by checking that marpa_g_error() returns MARPA_ERR_NONE.
   */
  else if ( marpa_g_error(g, NULL) == MARPA_ERR_NONE )
  {
    is_int( rv_wanted, rv_seen, "%s() succeeded", name );
  }
  /* failure wanted, except if it marks arguments end */
  else if (rv_wanted != ARGS_END)
  {
    /* return value */
    err_wanted = va_arg(va_args, int);
    is_int( rv_wanted, rv_seen, "%s() failed, returned %d", name, rv_seen );

    /* error code */
    err_seen = marpa_g_error(g, NULL);

    if (err_seen == MARPA_ERR_NONE && rv_seen < 0)
    {
      ok(0, "%s(): marpa_g_error() returned MARPA_ERR_NONE, but return value was %d.", name, rv_seen);
    }
    /* test error code */
    else
    {
      is_int( err_wanted, err_seen,
        "%s() error is: %s", name, marpa_m_error_message(err_seen) );
    }
  }
  else{
    printf("Test of %s(), argspec: '%s', argcount: %d, rv_wanted: %d, rv_seen: %d not defined yet.\n",
      name, ms.as, curr_arg_ix, rv_wanted, rv_seen);
    exit(1);
  }

  va_end(va_args);
}

#define MSG_MAX 120
#define MSG_BUFLEN (MSG_MAX+2+1)

/* [Semicolon] SEParated MesSaGe
 */
char *sep_msg(char *msg) {
   static char msg_buffer[MSG_BUFLEN];
   if (!msg) return "";
   if (!*msg) return "";
   strcpy(msg_buffer, "; ");
   strncat(msg_buffer, msg, MSG_MAX);
   return msg_buffer;
}

/* Report success/failure on "normal" return value.
 * "Normal" means negative is failure,
 * non-negative is success
 */
void rv_std_report(API_test_data* td, char *name, int rv_wanted, Marpa_Error_Code err_wanted)
{
   int rv_seen = td->rv_seen.int_rv;
   int err_seen = marpa_g_error(td->g, NULL);
   char* err_msg  = marpa_m_error_message(err_seen);
   char* msg = sep_msg(td->msg);

   if (rv_seen >= 0) {
      if (rv_wanted >= 0) {
	if (rv_wanted == rv_seen) {
	  ok(1, "%s() success as expected%s", name, msg);
	} else {
	  ok(0, "%s() success as expected but wrong value, wanted = %d, got %d%s", name, rv_wanted, rv_seen, msg);
	}
	return;
      }
      ok(0, "%s() unexpected success; value wanted = %d, got %d%s", name, rv_wanted, rv_seen, msg);
      return;
   }
   /* If here, the call failed */
   if (rv_wanted >= 0) {
     ok(0, "%s() unexpected failure; got return %d, expected %d; error = %s%s", name,
       rv_seen, rv_wanted, err_msg, msg);
     return;
   }
   /* If here, the call failed and was expected to fail */
   if (rv_wanted == rv_seen) {
     ok(1, "%s() expected failure; value = %d as expected%s", name, rv_seen, msg);
   } else {
     ok(0, "%s() expected failure; value wanted = %d, but got %d%s", name, rv_wanted, rv_seen, msg);
   }
   if (err_wanted == err_seen) {
     ok(1, "%s() expected failure; error as expected: '%s'%s", name, err_msg, msg);
   } else {
     char* wanted_err_msg  = marpa_m_error_message(err_wanted);
     ok(0, "%s() expected failure but unexpected error code: got '%s', expected '%s'%s",
       name, err_msg, wanted_err_msg, msg);
   }
}

/* Report success/failure on "hidden" return value.
 * "Hidden" means that the return value does not
 * unambiguously indicate an error, and the error
 * code must always be consulted as well.
 */
void rv_hidden_report(API_test_data* td, char *name, int rv_wanted, Marpa_Error_Code err_wanted)
{
   int rv_seen = td->rv_seen.int_rv;
   int err_seen = marpa_g_error(td->g, NULL);
   char* err_msg  = marpa_m_error_message(err_seen);
   char* msg = sep_msg(td->msg);
   int success = err_seen == MARPA_ERR_NONE;
   int success_wanted = err_wanted == MARPA_ERR_NONE;

   if (success) {
      if (success_wanted) {
	if (rv_wanted == rv_seen) {
	  ok(1, "%s() success as expected%s", name, msg);
	} else {
	  ok(0, "%s() success as expected, but wrong value, wanted = %d, got %d%s", name, rv_wanted, rv_seen, msg);
	}
	return;
      }
      ok(0, "%s() unexpected success; value wanted = %d, got %d%s", name, rv_wanted, rv_seen, msg);
      return;
   }
   /* If here, the call failed */
   if (success_wanted) {
     ok(0, "%s() unexpected failure; got return %d, expected %d; error = %s%s", name,
       rv_seen, rv_wanted, err_msg, msg);
     ok(0, "%s() unexpected failure; error code: '%s'%s",
       name, err_msg, msg);
     return;
   }
   /* If here, the call failed and was expected to fail */
   if (rv_wanted == rv_seen) {
     ok(1, "%s() expected failure; value = %d as expected%s", name, rv_seen, msg);
   } else {
     ok(0, "%s() expected failure; value wanted = %d, but got %d%s", name, rv_wanted, rv_seen, msg);
   }
   if (err_wanted == err_seen) {
     ok(1, "%s() expected failure; error as expected: '%s'%s", name, err_msg, msg);
   } else {
     char* wanted_err_msg  = marpa_m_error_message(err_wanted);
     ok(0, "%s() expected failure but unexpected error code: got '%s', expected '%s'%s",
       name, err_msg, wanted_err_msg, msg);
   }
}

/* Report success/failure when the return value is
 * an error code.
 */
void rv_code_report(API_test_data* td, char *name,
  Marpa_Error_Code err_seen, Marpa_Error_Code err_wanted)
{
   char* err_msg  = marpa_m_error_message(err_seen);
   char* msg = sep_msg(td->msg);

   if (err_seen == MARPA_ERR_NONE) {
      if (err_wanted == MARPA_ERR_NONE ) {
	  ok(1, "%s() success as expected%s", name, msg);
	} else {
	  ok(0, "%s() unexpected success; wanted error '%s'%d", name, err_msg, msg);
	}
      return;
   }
   /* If here, the call failed */
   if (err_wanted == MARPA_ERR_NONE) {
     ok(0, "%s() unexpected failure; error = %s%s", name,
       err_msg, msg);
     return;
   }
   /* If here, the call failed and was expected to fail */
   if (err_wanted == err_seen) {
     ok(1, "%s() expected failure; error as expected: '%s'%s", name, err_msg, msg);
   } else {
     char* wanted_err_msg  = marpa_m_error_message(err_wanted);
     ok(0, "%s() expected failure but unexpected error code: got '%s', expected '%s'%s",
       name, err_msg, wanted_err_msg, msg);
   }
}

/* Report success/failure on pointer return value.
 * NULL pointer indicates an error.
 */
void rv_ptr_report(API_test_data* td, char *name, Marpa_Error_Code err_wanted)
{
   void* rv_seen = td->rv_seen.ptr_rv;
   int err_seen = marpa_g_error(td->g, NULL);
   char* err_msg  = marpa_m_error_message(err_seen);
   char* wanted_err_msg  = marpa_m_error_message(err_wanted);
   char* msg = sep_msg(td->msg);
   int success = rv_seen ? 1 : 0;
   int success_wanted = err_wanted == MARPA_ERR_NONE;

   if (success) {
      if (success_wanted) {
	ok(1, "%s() success as expected%s", name, msg);
	return;
      }
      ok(0, "%s() unexpected success; error wanted = %s%s", name, wanted_err_msg, msg);
      return;
   }
   /* If here, the call failed */
   if (success_wanted) {
     ok(0, "%s() unexpected failure; error = %s%s", name, err_msg, msg);
     return;
   }
   /* If here, the call failed and was expected to fail */
   ok(1, "%s() expected failure%s", name, msg);
   if (err_wanted == err_seen) {
     ok(1, "%s() error as expected: '%s'", name, err_msg);
   } else {
     ok(0, "%s() unexpected error code: got '%s', expected '%s'",
       name, err_msg, wanted_err_msg);
   }
}

