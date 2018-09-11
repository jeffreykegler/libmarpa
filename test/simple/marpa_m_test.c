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
  { "marpa_g_symbol_is_terminal",  &marpa_g_symbol_is_terminal, "%s" },
  { "marpa_g_symbol_is_accessible", &marpa_g_symbol_is_accessible, "%s" },
  { "marpa_g_symbol_is_nullable", &marpa_g_symbol_is_nullable, "%s" },
  { "marpa_g_symbol_is_nulling", &marpa_g_symbol_is_nulling, "%s" },
  { "marpa_g_symbol_is_productive", &marpa_g_symbol_is_productive, "%s" },
  { "marpa_g_rule_is_proper_separation", &marpa_g_rule_is_proper_separation, "%r" },
  { "marpa_g_sequence_min", &marpa_g_sequence_min, "%r" },
  { "marpa_g_sequence_separator", &marpa_g_sequence_separator, "%r" },
  { "marpa_g_symbol_is_counted", &marpa_g_symbol_is_counted, "%s" },
  { "marpa_g_rule_is_nullable", &marpa_g_rule_is_nullable, "%r" },
  { "marpa_g_rule_is_nulling", &marpa_g_rule_is_nulling, "%r" },
  { "marpa_g_rule_is_loop", &marpa_g_rule_is_loop, "%r" },
  { "marpa_g_rule_is_accessible", &marpa_g_rule_is_accessible, "%r" },
  { "marpa_g_rule_is_nullable", &marpa_g_rule_is_nullable, "%r" },
  { "marpa_g_rule_is_nulling", &marpa_g_rule_is_nulling, "%r" },
  { "marpa_g_rule_is_loop", &marpa_g_rule_is_loop, "%r" },
  { "marpa_g_rule_is_productive", &marpa_g_rule_is_productive, "%r" },
  { "marpa_g_rule_length", &marpa_g_rule_length, "%r" },
  { "marpa_g_rule_lhs", &marpa_g_rule_lhs, "%r" },

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

struct error_description_s
{
  int error_code;
  const char *name;
  const char *suggested;
};

/* This is hard-copied in.  It really should be generated from
 * the Libmarpa source files to catch new error codes.
 */
static const struct error_description_s error_description[] = {
  { 0, "MARPA_ERR_NONE", "No error" },
  { 1, "MARPA_ERR_AHFA_IX_NEGATIVE", "MARPA_ERR_AHFA_IX_NEGATIVE" },
  { 2, "MARPA_ERR_AHFA_IX_OOB", "MARPA_ERR_AHFA_IX_OOB" },
  { 3, "MARPA_ERR_ANDID_NEGATIVE", "MARPA_ERR_ANDID_NEGATIVE" },
  { 4, "MARPA_ERR_ANDID_NOT_IN_OR", "MARPA_ERR_ANDID_NOT_IN_OR" },
  { 5, "MARPA_ERR_ANDIX_NEGATIVE", "MARPA_ERR_ANDIX_NEGATIVE" },
  { 6, "MARPA_ERR_BAD_SEPARATOR", "Separator has invalid symbol ID" },
  { 7, "MARPA_ERR_BOCAGE_ITERATION_EXHAUSTED", "MARPA_ERR_BOCAGE_ITERATION_EXHAUSTED" },
  { 8, "MARPA_ERR_COUNTED_NULLABLE", "Nullable symbol on RHS of a sequence rule" },
  { 9, "MARPA_ERR_DEVELOPMENT", "Development error, see string" },
  { 10, "MARPA_ERR_DUPLICATE_AND_NODE", "MARPA_ERR_DUPLICATE_AND_NODE" },
  { 11, "MARPA_ERR_DUPLICATE_RULE", "Duplicate rule" },
  { 12, "MARPA_ERR_DUPLICATE_TOKEN", "Duplicate token" },
  { 13, "MARPA_ERR_YIM_COUNT", "Maximum number of Earley items exceeded" },
  { 14, "MARPA_ERR_YIM_ID_INVALID", "MARPA_ERR_YIM_ID_INVALID" },
  { 15, "MARPA_ERR_EVENT_IX_NEGATIVE", "Negative event index" },
  { 16, "MARPA_ERR_EVENT_IX_OOB", "No event at that index" },
  { 17, "MARPA_ERR_GRAMMAR_HAS_CYCLE", "Grammar has cycle" },
  { 18, "MARPA_ERR_INACCESSIBLE_TOKEN", "Token symbol is inaccessible" },
  { 19, "MARPA_ERR_INTERNAL", "MARPA_ERR_INTERNAL" },
  { 20, "MARPA_ERR_INVALID_AHFA_ID", "MARPA_ERR_INVALID_AHFA_ID" },
  { 21, "MARPA_ERR_INVALID_AIMID", "MARPA_ERR_INVALID_AIMID" },
  { 22, "MARPA_ERR_INVALID_BOOLEAN", "Argument is not boolean" },
  { 23, "MARPA_ERR_INVALID_IRLID", "MARPA_ERR_INVALID_IRLID" },
  { 24, "MARPA_ERR_INVALID_NSYID", "MARPA_ERR_INVALID_NSYID" },
  { 25, "MARPA_ERR_INVALID_LOCATION", "Location is not valid" },
  { 26, "MARPA_ERR_INVALID_RULE_ID", "Rule ID is malformed" },
  { 27, "MARPA_ERR_INVALID_START_SYMBOL", "Specified start symbol is not valid" },
  { 28, "MARPA_ERR_INVALID_SYMBOL_ID", "Symbol ID is malformed" },
  { 29, "MARPA_ERR_I_AM_NOT_OK", "Marpa is in a not OK state" },
  { 30, "MARPA_ERR_MAJOR_VERSION_MISMATCH", "Libmarpa major version number is a mismatch" },
  { 31, "MARPA_ERR_MICRO_VERSION_MISMATCH", "Libmarpa micro version number is a mismatch" },
  { 32, "MARPA_ERR_MINOR_VERSION_MISMATCH", "Libmarpa minor version number is a mismatch" },
  { 33, "MARPA_ERR_NOOKID_NEGATIVE", "MARPA_ERR_NOOKID_NEGATIVE" },
  { 34, "MARPA_ERR_NOT_PRECOMPUTED", "This grammar is not precomputed" },
  { 35, "MARPA_ERR_NOT_TRACING_COMPLETION_LINKS", "MARPA_ERR_NOT_TRACING_COMPLETION_LINKS" },
  { 36, "MARPA_ERR_NOT_TRACING_LEO_LINKS", "MARPA_ERR_NOT_TRACING_LEO_LINKS" },
  { 37, "MARPA_ERR_NOT_TRACING_TOKEN_LINKS", "MARPA_ERR_NOT_TRACING_TOKEN_LINKS" },
  { 38, "MARPA_ERR_NO_AND_NODES", "MARPA_ERR_NO_AND_NODES" },
  { 39, "MARPA_ERR_NO_EARLEY_SET_AT_LOCATION", "Earley set ID is after latest Earley set" },
  { 40, "MARPA_ERR_NO_OR_NODES", "MARPA_ERR_NO_OR_NODES" },
  { 41, "MARPA_ERR_NO_PARSE", "No parse" },
  { 42, "MARPA_ERR_NO_RULES", "This grammar does not have any rules" },
  { 43, "MARPA_ERR_NO_START_SYMBOL", "This grammar has no start symbol" },
  { 44, "MARPA_ERR_NO_TOKEN_EXPECTED_HERE", "No token is expected at this earleme location" },
  { 45, "MARPA_ERR_NO_TRACE_YIM", "MARPA_ERR_NO_TRACE_YIM" },
  { 46, "MARPA_ERR_NO_TRACE_YS", "MARPA_ERR_NO_TRACE_YS" },
  { 47, "MARPA_ERR_NO_TRACE_PIM", "MARPA_ERR_NO_TRACE_PIM" },
  { 48, "MARPA_ERR_NO_TRACE_SRCL", "MARPA_ERR_NO_TRACE_SRCL" },
  { 49, "MARPA_ERR_NULLING_TERMINAL", "A symbol is both terminal and nulling" },
  { 50, "MARPA_ERR_ORDER_FROZEN", "The ordering is frozen" },
  { 51, "MARPA_ERR_ORID_NEGATIVE", "MARPA_ERR_ORID_NEGATIVE" },
  { 52, "MARPA_ERR_OR_ALREADY_ORDERED", "MARPA_ERR_OR_ALREADY_ORDERED" },
  { 53, "MARPA_ERR_PARSE_EXHAUSTED", "The parse is exhausted" },
  { 54, "MARPA_ERR_PARSE_TOO_LONG", "This input would make the parse too long" },
  { 55, "MARPA_ERR_PIM_IS_NOT_LIM", "MARPA_ERR_PIM_IS_NOT_LIM" },
  { 56, "MARPA_ERR_POINTER_ARG_NULL", "An argument is null when it should not be" },
  { 57, "MARPA_ERR_PRECOMPUTED", "This grammar is precomputed" },
  { 58, "MARPA_ERR_PROGRESS_REPORT_EXHAUSTED", "The progress report is exhausted" },
  { 59, "MARPA_ERR_PROGRESS_REPORT_NOT_STARTED", "No progress report has been started" },
  { 60, "MARPA_ERR_RECCE_NOT_ACCEPTING_INPUT", "The recognizer is not accepting input" },
  { 61, "MARPA_ERR_RECCE_NOT_STARTED", "The recognizer has not been started" },
  { 62, "MARPA_ERR_RECCE_STARTED", "The recognizer has been started" },
  { 63, "MARPA_ERR_RHS_IX_NEGATIVE", "RHS index cannot be negative" },
  { 64, "MARPA_ERR_RHS_IX_OOB", "RHS index must be less than rule length" },
  { 65, "MARPA_ERR_RHS_TOO_LONG", "The RHS is too long" },
  { 66, "MARPA_ERR_SEQUENCE_LHS_NOT_UNIQUE", "LHS of sequence rule would not be unique" },
  { 67, "MARPA_ERR_SOURCE_TYPE_IS_AMBIGUOUS", "MARPA_ERR_SOURCE_TYPE_IS_AMBIGUOUS" },
  { 68, "MARPA_ERR_SOURCE_TYPE_IS_COMPLETION", "MARPA_ERR_SOURCE_TYPE_IS_COMPLETION" },
  { 69, "MARPA_ERR_SOURCE_TYPE_IS_LEO", "MARPA_ERR_SOURCE_TYPE_IS_LEO" },
  { 70, "MARPA_ERR_SOURCE_TYPE_IS_NONE", "MARPA_ERR_SOURCE_TYPE_IS_NONE" },
  { 71, "MARPA_ERR_SOURCE_TYPE_IS_TOKEN", "MARPA_ERR_SOURCE_TYPE_IS_TOKEN" },
  { 72, "MARPA_ERR_SOURCE_TYPE_IS_UNKNOWN", "MARPA_ERR_SOURCE_TYPE_IS_UNKNOWN" },
  { 73, "MARPA_ERR_START_NOT_LHS", "Start symbol not on LHS of any rule" },
  { 74, "MARPA_ERR_SYMBOL_VALUED_CONFLICT", "Symbol is treated both as valued and unvalued" },
  { 75, "MARPA_ERR_TERMINAL_IS_LOCKED", "The terminal status of the symbol is locked" },
  { 76, "MARPA_ERR_TOKEN_IS_NOT_TERMINAL", "Token symbol must be a terminal" },
  { 77, "MARPA_ERR_TOKEN_LENGTH_LE_ZERO", "Token length must greater than zero" },
  { 78, "MARPA_ERR_TOKEN_TOO_LONG", "Token is too long" },
  { 79, "MARPA_ERR_TREE_EXHAUSTED", "Tree iterator is exhausted" },
  { 80, "MARPA_ERR_TREE_PAUSED", "Tree iterator is paused" },
  { 81, "MARPA_ERR_UNEXPECTED_TOKEN_ID", "Unexpected token" },
  { 82, "MARPA_ERR_UNPRODUCTIVE_START", "Unproductive start symbol" },
  { 83, "MARPA_ERR_VALUATOR_INACTIVE", "Valuator inactive" },
  { 84, "MARPA_ERR_VALUED_IS_LOCKED", "The valued status of the symbol is locked" },
  { 85, "MARPA_ERR_RANK_TOO_LOW", "Rule or symbol rank too low" },
  { 86, "MARPA_ERR_RANK_TOO_HIGH", "Rule or symbol rank too high" },
  { 87, "MARPA_ERR_SYMBOL_IS_NULLING", "Symbol is nulling" },
  { 88, "MARPA_ERR_SYMBOL_IS_UNUSED", "Symbol is not used" },
  { 89, "MARPA_ERR_NO_SUCH_RULE_ID", "No rule with this ID exists" },
  { 90, "MARPA_ERR_NO_SUCH_SYMBOL_ID", "No symbol with this ID exists" },
  { 91, "MARPA_ERR_BEFORE_FIRST_TREE", "Tree iterator is before first tree" },
  { 92, "MARPA_ERR_SYMBOL_IS_NOT_COMPLETION_EVENT", "Symbol is not set up for completion events" },
  { 93, "MARPA_ERR_SYMBOL_IS_NOT_NULLED_EVENT", "Symbol is not set up for nulled events" },
  { 94, "MARPA_ERR_SYMBOL_IS_NOT_PREDICTION_EVENT", "Symbol is not set up for prediction events" },
  { 95, "MARPA_ERR_RECCE_IS_INCONSISTENT", "MARPA_ERR_RECCE_IS_INCONSISTENT" },
  { 96, "MARPA_ERR_INVALID_ASSERTION_ID", "Assertion ID is malformed" },
  { 97, "MARPA_ERR_NO_SUCH_ASSERTION_ID", "No assertion with this ID exists" },
  { 98, "MARPA_ERR_HEADERS_DO_NOT_MATCH", "Internal error: Libmarpa was built incorrectly" },
  { 99, "MARPA_ERR_NOT_A_SEQUENCE", "Rule is not a sequence" },
};

const char *marpa_m_error_message (Marpa_Error_Code error_code)
{
  int i;
  for (i = 0; i < sizeof(error_description) / sizeof(Marpa_Method_Error); i++)
    if ( error_code == error_description[i].error_code )
      return error_description[i].suggested;
  printf("No message yet for Marpa error code %d %s.\n", error_code,
      error_description[i].name);
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
   const char* err_msg  = marpa_m_error_message(err_seen);
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
     const char* wanted_err_msg  = marpa_m_error_message(err_wanted);
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
   const char* err_msg  = marpa_m_error_message(err_seen);
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
     const char* wanted_err_msg  = marpa_m_error_message(err_wanted);
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
   const char* err_msg  = marpa_m_error_message(err_seen);
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
     const char* wanted_err_msg  = marpa_m_error_message(err_wanted);
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
   const char* err_msg  = marpa_m_error_message(err_seen);
   const char* wanted_err_msg  = marpa_m_error_message(err_wanted);
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

