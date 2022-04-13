#include <stdio.h>
#include "marpa.h"

#include "tap/basic.h"

static void
err (const char *s, Marpa_Grammar g)
{
    Marpa_Error_Code errcode = marpa_g_error (g, NULL);
    printf ("%s: Error %d\n\n", s, errcode);
    marpa_g_error_clear (g);
}

static void
code_fail (const char *s, Marpa_Grammar g)
{
    err (s, g);
    exit (1);
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

    plan (4);

    marpa_c_init (&marpa_configuration);
    g = marpa_g_new (&marpa_configuration);
    if (!g) {
        Marpa_Error_Code errcode =
            marpa_c_error (&marpa_configuration, NULL);
        printf ("marpa_g_new: error %d", errcode);
        exit (1);
    }

    /* Symbols */
    if ((S_lhs = marpa_g_symbol_new (g)) < 0) {
        code_fail ("marpa_g_symbol_new", g);
    }
    if ((S_rhs = marpa_g_symbol_new (g)) < 0) {
        code_fail ("marpa_g_symbol_new", g);
    }

    /* Rule */
    rhs[0] = S_rhs;
    if ((R_new = marpa_g_rule_new (g, S_lhs, rhs, 1)) < 0) {
        code_fail ("marpa_g_rule_new", g);
    }
    ok ((R_new == 0), "marpa_g_rule_new returns 0");

    /* Precompute */
    rc = marpa_g_start_symbol_set (g, S_lhs);
    if (rc < 0) {
        code_fail ("marpa_g_start_symbol_set", g);
    }

    rc = marpa_g_precompute (g);
    ok ((rc == 0), "marpa_g_precompute returned 0");
    if (rc < 0) {
        code_fail ("marpa_g_sequence_separator", g);
    }

    rc = marpa_g_is_precomputed (g);
    ok ((rc == 1), "marpa_g_is_precomputed returned 1");
    if (rc < 0) {
        code_fail ("marpa_g_is_precomputed", g);
    }

    /* Rule accessors */
    rc = marpa_g_rule_lhs (g, R_new);
    ok ((rc == 0), "marpa_g_rule_lhs(%d) returned 0", R_new);
    if (rc < 0) {
        code_fail ("marpa_g_rule_lhs", g);
    }

    marpa_g_unref(g);

    return 0;
}
