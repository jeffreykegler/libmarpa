/*
 * Test of the libtap test_cleanup_register function on failure.
 *
 * Copyright 2013, 2014 Russ Allbery <eagle@eyrie.org>
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdio.h>

#include <tests/tap/basic.h>


/*
 * The test function to call during cleanup.
 */
static void
test(int success, int primary)
{
    printf("Called cleanup with %d %d\n", success, primary);
}


int
main(void)
{
    test_cleanup_register(test);
    plan(1);
    ok(0, "some test");
    return 0;
}
