#include <stdio.h>
#include "marpa.h"

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
  printf ("marpa_g_rule_new returned %d\n\n", R_new);
  
  // precompute
  rc = marpa_g_start_symbol_set(g, S_lhs);
  ((rc >= 0) || err("marpa_g_start_symbol_set", g));

  rc = marpa_g_precompute(g);
  printf ("marpa_g_precompute returned %d\n", rc);
  ((rc >= 0) || err("marpa_g_sequence_separator", g));
  
  rc = marpa_g_is_precomputed(g);
  printf ("marpa_g_is_precomputed returned %d\n\n", rc);
  ((rc >= 0) || err("marpa_g_is_precomputed", g));

  // rule accessors
  rc = marpa_g_rule_lhs (g, R_new); 
  printf ("marpa_g_rule_lhs returned %d\n", rc);
  ((rc >= 0) || err("marpa_g_rule_lhs", g));
  
  return 0;
}
