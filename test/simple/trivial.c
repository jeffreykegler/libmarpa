/*
 * Copyright 2018 Jeffrey Kegler
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

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "marpa.h"

#include "tap/basic.h"

static int
fail (const char *s, Marpa_Grammar g)
{
  const char *error_string;
  Marpa_Error_Code errcode = marpa_g_error (g, &error_string);
  printf ("%s returned %d: %s", s, errcode, error_string);
  exit (1);
}

Marpa_Symbol_ID S_top;
Marpa_Symbol_ID S_A1;
Marpa_Symbol_ID S_A2;
Marpa_Symbol_ID S_B1;
Marpa_Symbol_ID S_B2;
Marpa_Symbol_ID S_C1;
Marpa_Symbol_ID S_C2;

/* For fatal error messages */
char error_buffer[80];

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
  sprintf (error_buffer, "no such symbol: %d", id);
  return error_buffer;
};

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
};


int
main (int argc, char *argv[])
{
  const unsigned char *p, *eof;
  int i;
  int rc;
  const char *error_string;
  struct stat sb;

  Marpa_Config marpa_configuration;

  Marpa_Grammar g;
  Marpa_Recognizer r;
  /* Longest rule is <= 4 symbols */
  Marpa_Symbol_ID rhs[4];

  plan(13);

  marpa_c_init (&marpa_configuration);
  g = marpa_g_new (&marpa_configuration);
  if (!g)
    {
      Marpa_Error_Code errcode =
	marpa_c_error (&marpa_configuration, &error_string);
      printf ("marpa_g_new returned %d: %s", errcode, error_string);
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
  (marpa_g_rule_new (g, S_top, rhs, 1) >= 0)
    || fail ("marpa_g_rule_new", g);
  rhs[0] = S_A2;
  (marpa_g_rule_new (g, S_top, rhs, 1) >= 0)
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
  (marpa_g_rule_new (g, S_C2, rhs, 0) >= 0)
    || fail ("marpa_g_rule_new", g);

  (marpa_g_start_symbol_set (g, S_top) >= 0)
    || fail ("marpa_g_start_symbol_set", g);
  if (marpa_g_precompute (g) < 0)
    {
      marpa_g_error (g, &error_string);
      puts (error_string);
      exit (1);
    }
  ok(1, "precomputation succeeded");
  r = marpa_r_new (g);
  if (!r)
    {
      marpa_g_error (g, &error_string);
      puts (error_string);
      exit (1);
    }
  rc = marpa_r_start_input (r);
  if (!rc)
    {
      marpa_g_error (g, &error_string);
      puts (error_string);
      exit (1);
    }
  ok((marpa_r_is_exhausted(r)), "exhausted at earleme 0");

  {
    Marpa_Event event;
    const int highest_symbol_id = marpa_g_highest_symbol_id (g);
    int exhausted_event_triggered = 0;
    int spurious_events = 0;
    int spurious_nulled_events = 0;
    int event_ix;
    const int event_count = marpa_g_event_count (g);
    int *nulled_symbols = calloc ((highest_symbol_id + 1), sizeof (int));
    if (!nulled_symbols) abort();
    ok ((event_count == 8), "event count at earleme 0 is %ld",
	(long) event_count);
    for (event_ix = 0; event_ix < event_count; event_ix++)
      {
	int event_type = marpa_g_event (g, &event, event_ix);
	if (event_type == MARPA_EVENT_SYMBOL_NULLED)
	  {
	    const Marpa_Symbol_ID event_symbol_id = marpa_g_event_value(&event);
	    nulled_symbols[event_symbol_id]++;
	    continue;
	  }
	if (event_type == MARPA_EVENT_EXHAUSTED)
	  {
	    exhausted_event_triggered++;
	    continue;
	  }
	printf ("event type is %ld\n", (long) event_type);
	spurious_events++;
      }
    ok ((spurious_events == 0),
	"spurious events triggered: %ld", (long) spurious_events);
    ok (exhausted_event_triggered, "exhausted event triggered");
    for (i = 0; i <= highest_symbol_id; i++)
      {
	if (is_nullable (i))
	  {
	    ok (1, "nulled event triggered for %s", symbol_name (i));
	    continue;
	  }
	spurious_nulled_events++;
      }
    ok ((spurious_nulled_events == 0), "spurious nulled events triggered = %ld",
	(long) spurious_nulled_events);
    free (nulled_symbols);
  }

  return 0;
}
