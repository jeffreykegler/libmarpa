/*
 * Calls libtap basic functions for testing.
 *
 * Written by Russ Allbery <eagle@eyrie.org>
 * Copyright 2015-2019 Russ Allbery <eagle@eyrie.org>
 * Copyright 2008-2009, 2014
 *     The Board of Trustees of the Leland Stanford Junior University
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdlib.h>
#include <string.h>

#include <tests/tap/basic.h>
#include <tests/tap/float.h>

/*
 * Test okv(), which requires having a separate stdargs function that
 * generates the va_list.
 */
static void __attribute__((__format__(printf, 1, 2)))
test_okv(const char *format, ...)
{
    va_list args;
    int status;

    va_start(args, format);
    status = okv(1, format, args);
    va_end(args);
    is_int(1, status, "okv result is 1");
}


int
main(void)
{
    int status;
    void *p;
    char *s;
    const unsigned char d1[] = {0x00, 0x10, 0x20, 0xff, 0xfe};
    const unsigned char d2[] = {0x00, 0x10, 0x20, 0xff, 0xfe};
    const unsigned char d3[] = {0x01, 0x10, 0x20, 0xfa, 0xef};

    plan(62);

    /*
     * Call the memory allocation functions just to be sure the prototypes are
     * present, they link properly, and nothing obviously segfaults.  Do this
     * first so that, if we corrupt the heap, we increase the chances of a
     * later segfault.
     */
    p = bmalloc(10);
    memset(p, 1, 10);
    free(p);
    p = bcalloc(5, 10);
    memset(p, 1, 5 * 10);
    p = brealloc(p, 20);
    memset(p, 2, 20);
    p = breallocarray(p, 6, 10);
    memset(p, 3, 6 * 10);
    free(p);
    s = bstrdup("testing");
    free(s);
    p = brealloc(NULL, 10);
    memset(p, 4, 10);
    free(p);
    p = breallocarray(NULL, 7, 10);
    memset(p, 5, 7 * 10);
    free(p);

    /* Likewise with the macros. */
    s = bcalloc_type(10, char);
    s = breallocarray_type(s, 20, char);
    free(s);

    /* Test ok, is_*, and skip functions. */
    status = ok(1, NULL);
    is_int(1, status, "ok result is 1");
    diag("a diagnostic");
    status = ok(0, NULL);
    is_int(0, status, "ok result is 0");
    status = is_int(0, 0, NULL);
    is_int(1, status, "is_int result is 1");
    is_int(1, 1, "an integer test");
    is_int(-1, -1, "comparing %d and %d", -1, -1);
    status = is_int(-1, 1, NULL);
    is_int(0, status, "is_int result is 0");
    status = is_double(0, 0, 0, NULL);
    is_int(1, status, "is_double result is 1");
    is_double(0.1, 0.11, 0.02, "a double test");
    status = is_double(0.1, -0.1, 0.1, "a failing double test");
    is_int(0, status, "is_double result is 0");
    is_double(0, -0, 0.1, NULL);
    is_double(1.7e45, 1.7e45, 0.1, NULL);
    is_double(strtod("NAN", NULL), strtod("NAN", NULL), 0, "NaN");
    is_double(strtod("INF", NULL), strtod("INF", NULL), 0, "inf");
    is_double(strtod("-INF", NULL), strtod("-INF", NULL), 0, "-inf");
    is_double(strtod("INF", NULL), strtod("-INF", NULL), 0, "inf and -inf");
    is_string("", "", NULL);
    status = is_string("yes", "yes", "a string test");
    is_int(1, status, "is_string result is 1");
    status = is_string("yes", "yes no", "%s", "");
    is_int(0, status, "is_string result is 0");
    is_string(NULL, NULL, "null null");
    is_string("yes", NULL, NULL);
    is_string(NULL, "yes", NULL);
    skip("testing skip %s", "foo");
    status = ok_block(2, 1, NULL);
    is_int(1, status, "ok_block result is 1");
    status = ok_block(4, 0, "some %s", "block");
    is_int(0, status, "ok_block result is 0");
    skip_block(4, "testing skip block");
    skip(NULL);
    skip_block(2, NULL);
    status = is_hex(2147483649UL, 2147483649UL, "2^31 + 1 correct");
    is_int(1, status, "is_hex result is 1");
    status = is_hex(2147483649UL, 0, "2^31 + 1 incorrect");
    is_int(0, status, "is_hex result is 0");
    is_hex(0, 0, NULL);
    test_okv("testing %s", "okv");
    is_bool(0, 0, "is_bool on %d", 0);
    is_bool(1, 4, "is_bool on 1 and 4");
    is_bool(4, 0, "is_bool display output");
    is_bool(0, -5, "is_bool more display output");
    status = is_blob(d1, d2, 5, "is_blob %d", 1);
    is_int(1, status, "is_blob returned 1");
    status = is_blob(d1, d3, 5, "is_blob %d", 2);
    is_int(0, status, "is_blob returned 0");

    /* Test return status of diag. */
    status = diag("testing diag");
    is_int(1, status, "diag returns 1");

    /* Test that (null) does not compare equal to NULL. */
    is_string("(null)", NULL, "(null) and NULL");

    return 0;
}
