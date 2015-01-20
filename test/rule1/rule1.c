#include <stdio.h>
#include "marpa.h"

#include "tap/basic.h"

static int
err (const char *s, Marpa_Grammar g)
{
  Marpa_Error_Code errcode = marpa_g_error (g, NULL);
  printf ("%s: Error %d\n\n", s, errcode);
  marpa_g_error_clear(g);
}

int
main (int argc, char *argv[])
{

  Marpa_Config marpa_configuration;
  Marpa_Grammar g;

  Marpa_Symbol_ID S_lhs, S_rhs;
  Marpa_Symbol_ID rhs[1];

  Marpa_Rule_ID R_new;

  int rc;

  plan(4);

  marpa_c_init (&marpa_configuration);
  g = marpa_g_new (&marpa_configuration);
  if (!g)
    {
      Marpa_Error_Code errcode =
        marpa_c_error (&marpa_configuration, NULL);
      printf ("marpa_g_new: error %d", errcode);
      exit (1);
    }
  
  // symbols
  ((S_lhs = marpa_g_symbol_new (g)) >= 0) || err ("marpa_g_symbol_new", g);
  ((S_rhs = marpa_g_symbol_new (g)) >= 0) || err ("marpa_g_symbol_new", g);
  
  // rule
  rhs[0] = S_rhs;
  (((R_new = marpa_g_rule_new (g, S_lhs, rhs, 1)) >= 0)
    || err ("marpa_g_rule_new", g));
  ok ((R_new == 0),"marpa_g_rule_new returns 0");
  
  // precompute
  rc = marpa_g_start_symbol_set(g, S_lhs);
  ((rc >= 0) || err("marpa_g_start_symbol_set", g));

  rc = marpa_g_precompute(g);
  ok ((rc == 0), "marpa_g_precompute returned 0");
  ((rc >= 0) || err("marpa_g_sequence_separator", g));
  
  rc = marpa_g_is_precomputed(g);
  ok ((rc == 1), "marpa_g_is_precomputed returned 1");
  ((rc >= 0) || err("marpa_g_is_precomputed", g));

  // rule accessors
  rc = marpa_g_rule_lhs (g, R_new); 
  ok ((rc == 0), "marpa_g_rule_lhs(%d) returned 0", R_new);
  ((rc >= 0) || err("marpa_g_rule_lhs", g));
  
  return 0;
}
