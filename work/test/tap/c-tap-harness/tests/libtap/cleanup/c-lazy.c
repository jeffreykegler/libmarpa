/*
 * Test of the libtap test_cleanup_register function with lazy planning.
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
    plan_lazy();
    ok(1, "foo");
    test_cleanup_register(test);
    return 0;
}
