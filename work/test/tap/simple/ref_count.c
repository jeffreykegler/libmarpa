/*
 * Copyright 2022 Jeffrey Kegler
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

/* Tests that the "ref" methods, and the constructors that take a
 * reference to a parent object, fail with MARPA_ERR_REF_COUNT_OVERFLOW
 * when a reference count is already INT_MAX, and leave the count unchanged.
 *
 * Each object is tested in a child process, so that the parent's objects
 * keep their counts.  The child reports one bit per check through a pipe.
 */

#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "marpa.h"

#include "tap/basic.h"

static void
fail (const char *s, Marpa_Grammar g)
{
    const char *error_string;
    Marpa_Error_Code errcode = marpa_g_error (g, &error_string);
    printf ("%s returned %d: %s", s, errcode, error_string);
    exit (1);
}

enum object_type {
    OBJ_GRAMMAR, OBJ_RECCE, OBJ_BOCAGE, OBJ_ORDER, OBJ_TREE, OBJ_VALUE
};

static Marpa_Grammar g;
static Marpa_Recognizer r;
static Marpa_Bocage b;
static Marpa_Order o;
static Marpa_Tree t;
static Marpa_Value v;

/* Reference counts of the objects as created in main():
 * the grammar is also held by the recognizer and the bocage,
 * the bocage by the order, the order by the tree,
 * and the tree by the valuator. */
static const int initial_count[] = { 3, 1, 2, 2, 2, 1 };

static void *
object_of (enum object_type type)
{
    switch (type) {
    case OBJ_GRAMMAR: return g;
    case OBJ_RECCE: return r;
    case OBJ_BOCAGE: return b;
    case OBJ_ORDER: return o;
    case OBJ_TREE: return t;
    case OBJ_VALUE: return v;
    }
    return NULL;
}

static void *
take_ref (enum object_type type)
{
    switch (type) {
    case OBJ_GRAMMAR: return marpa_g_ref (g);
    case OBJ_RECCE: return marpa_r_ref (r);
    case OBJ_BOCAGE: return marpa_b_ref (b);
    case OBJ_ORDER: return marpa_o_ref (o);
    case OBJ_TREE: return marpa_t_ref (t);
    case OBJ_VALUE: return marpa_v_ref (v);
    }
    return NULL;
}

static void
drop_ref (enum object_type type)
{
    switch (type) {
    case OBJ_GRAMMAR: marpa_g_unref (g); break;
    case OBJ_RECCE: marpa_r_unref (r); break;
    case OBJ_BOCAGE: marpa_b_unref (b); break;
    case OBJ_ORDER: marpa_o_unref (o); break;
    case OBJ_TREE: marpa_t_unref (t); break;
    case OBJ_VALUE: marpa_v_unref (v); break;
    }
}

static int
overflow_reported (void)
{
    return marpa_g_error (g, NULL) == MARPA_ERR_REF_COUNT_OVERFLOW;
}

static void
clear_error (void)
{
    marpa_g_error_clear (g);
}

/* Constructor checks, run once the count of the object is INT_MAX.
 * Each one must fail with MARPA_ERR_REF_COUNT_OVERFLOW.
 */
#define CHECK_CONSTRUCTOR(call) \
    (clear_error (), (call) == NULL && overflow_reported ())

static unsigned int
child_checks (enum object_type type)
{
    unsigned int bits = 0;
    const long limit = (long) INT_MAX - initial_count[type];
    long refs;

    /* 1: exactly INT_MAX - initial_count refs succeed,
     * and the next one fails with the new error code. */
    clear_error ();
    for (refs = 0; refs < limit; refs++)
        if (take_ref (type) == NULL)
            break;
    if (refs != limit)
        return bits;
    if (take_ref (type) != NULL || !overflow_reported ())
        return bits;
    bits |= 1u << 0;

    /* 2: the failed call did not change the count: after one unref,
     * exactly one more ref succeeds. */
    drop_ref (type);
    clear_error ();
    if (take_ref (type) == object_of (type)) {
        clear_error ();
        if (take_ref (type) == NULL && overflow_reported ())
            bits |= 1u << 1;
    }

    /* 3, 4: constructors that would take a reference to this object. */
    switch (type) {
    case OBJ_GRAMMAR:
        if (CHECK_CONSTRUCTOR (marpa_r_new (g)))
            bits |= 1u << 2;
        if (CHECK_CONSTRUCTOR (marpa_b_new (r, -1)))
            bits |= 1u << 3;
        break;
    case OBJ_BOCAGE:
        if (CHECK_CONSTRUCTOR (marpa_o_new (b)))
            bits |= 1u << 2;
        break;
    case OBJ_ORDER:
        if (CHECK_CONSTRUCTOR (marpa_t_new (o)))
            bits |= 1u << 2;
        break;
    case OBJ_TREE:
        if (CHECK_CONSTRUCTOR (marpa_v_new (t)))
            bits |= 1u << 2;
        /* 4: an exhausted tree takes no reference, so marpa_v_new()
         * still reports MARPA_ERR_TREE_EXHAUSTED at INT_MAX.
         * Destroying the valuator releases its reference to the tree;
         * take it again, then exhaust the tree. */
        marpa_v_unref (v);
        clear_error ();
        if (marpa_t_ref (t) == t && marpa_t_next (t) == -1) {
            clear_error ();
            if (marpa_v_new (t) == NULL
                && marpa_g_error (g, NULL) == MARPA_ERR_TREE_EXHAUSTED)
                bits |= 1u << 3;
        }
        break;
    default:
        break;
    }
    return bits;
}

static void
test_ref_limit (enum object_type type, const char *method,
    const char *check3, const char *check4)
{
    int fds[2];
    pid_t pid;
    int status;
    unsigned int bits = 0;
    ssize_t bytes_read;

    if (pipe (fds) < 0)
        sysbail ("pipe");
    fflush (stdout);
    pid = fork ();
    if (pid < 0)
        sysbail ("fork");
    if (pid == 0) {
        unsigned int child_bits;
        close (fds[0]);
        child_bits = child_checks (type);
        if (write (fds[1], &child_bits, sizeof child_bits)
            != sizeof child_bits)
            _exit (2);
        _exit (0);
    }
    close (fds[1]);
    do
        bytes_read = read (fds[0], &bits, sizeof bits);
    while (bytes_read < 0 && errno == EINTR);
    close (fds[0]);
    while (waitpid (pid, &status, 0) < 0)
        if (errno != EINTR)
            sysbail ("waitpid");
    if (bytes_read != sizeof bits || !WIFEXITED (status)
        || WEXITSTATUS (status) != 0)
        bits = 0;
    ok ((bits & (1u << 0)) != 0,
        "%s fails with MARPA_ERR_REF_COUNT_OVERFLOW exactly at INT_MAX",
        method);
    ok ((bits & (1u << 1)) != 0,
        "%s leaves the count unchanged when it fails", method);
    if (check3)
        ok ((bits & (1u << 2)) != 0, "%s", check3);
    if (check4)
        ok ((bits & (1u << 3)) != 0, "%s", check4);
}

int
main (int argc UNUSED, char *argv[] UNUSED)
{
    const char *error_string;
    Marpa_Config marpa_configuration;
    Marpa_Symbol_ID S_top, S_tok;
    Marpa_Symbol_ID rhs[1];

    plan (18);

    marpa_c_init (&marpa_configuration);
    g = marpa_g_new (&marpa_configuration);
    if (!g) {
        Marpa_Error_Code errcode =
            marpa_c_error (&marpa_configuration, &error_string);
        printf ("marpa_g_new returned %d: %s", errcode, error_string);
        exit (1);
    }
    if (marpa_g_force_valued (g) < 0)
        fail ("marpa_g_force_valued", g);
    if ((S_top = marpa_g_symbol_new (g)) < 0)
        fail ("marpa_g_symbol_new", g);
    if ((S_tok = marpa_g_symbol_new (g)) < 0)
        fail ("marpa_g_symbol_new", g);
    rhs[0] = S_tok;
    if (marpa_g_rule_new (g, S_top, rhs, 1) < 0)
        fail ("marpa_g_rule_new", g);
    if (marpa_g_start_symbol_set (g, S_top) < 0)
        fail ("marpa_g_start_symbol_set", g);
    if (marpa_g_precompute (g) < 0)
        fail ("marpa_g_precompute", g);

    r = marpa_r_new (g);
    if (!r)
        fail ("marpa_r_new", g);
    if (marpa_r_start_input (r) < 0)
        fail ("marpa_r_start_input", g);
    if (marpa_r_alternative (r, S_tok, 1, 1) != MARPA_ERR_NONE)
        fail ("marpa_r_alternative", g);
    if (marpa_r_earleme_complete (r) < 0)
        fail ("marpa_r_earleme_complete", g);
    b = marpa_b_new (r, -1);
    if (!b)
        fail ("marpa_b_new", g);
    o = marpa_o_new (b);
    if (!o)
        fail ("marpa_o_new", g);
    t = marpa_t_new (o);
    if (!t)
        fail ("marpa_t_new", g);
    if (marpa_t_next (t) < 0)
        fail ("marpa_t_next", g);
    v = marpa_v_new (t);
    if (!v)
        fail ("marpa_v_new", g);

    test_ref_limit (OBJ_GRAMMAR, "marpa_g_ref",
        "marpa_r_new fails with MARPA_ERR_REF_COUNT_OVERFLOW",
        "marpa_b_new fails with MARPA_ERR_REF_COUNT_OVERFLOW");
    test_ref_limit (OBJ_RECCE, "marpa_r_ref", NULL, NULL);
    test_ref_limit (OBJ_BOCAGE, "marpa_b_ref",
        "marpa_o_new fails with MARPA_ERR_REF_COUNT_OVERFLOW", NULL);
    test_ref_limit (OBJ_ORDER, "marpa_o_ref",
        "marpa_t_new fails with MARPA_ERR_REF_COUNT_OVERFLOW", NULL);
    test_ref_limit (OBJ_TREE, "marpa_t_ref",
        "marpa_v_new fails with MARPA_ERR_REF_COUNT_OVERFLOW",
        "marpa_v_new on an exhausted tree still reports MARPA_ERR_TREE_EXHAUSTED");
    test_ref_limit (OBJ_VALUE, "marpa_v_ref", NULL, NULL);

    marpa_v_unref (v);
    marpa_t_unref (t);
    marpa_o_unref (o);
    marpa_b_unref (b);
    marpa_r_unref (r);
    marpa_g_unref (g);

    return 0;
}
