/*
 * Test of the libtap test_cleanup_register function with failure.
 *
 * Copyright 2013, 2014 Russ Allbery <eagle@eyrie.org>
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdio.h>

#include <tests/tap/basic.h>


/*
 * The first test function to call during cleanup.
 */
static void
one(int success, int primary)
{
    printf("Called cleanup with %d %d\n", success, primary);
}


/*
 * The second test function to call during cleanup.
 */
static void
two(int success, int primary)
{
    printf("Second cleanup (%d %d)\n", success, primary);
}


int
main(void)
{
    test_cleanup_register(one);
    plan(3);
    test_cleanup_register(two);
    test_cleanup_register(one);
    ok(1, "some test");
    test_cleanup_register(two);
    ok(0, "another test");
    ok(1, "third test");
    test_cleanup_register(one);
    return 0;
}
