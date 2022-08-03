/*
 * Test of the libtap test_cleanup_register function with bail.
 *
 * Copyright 2013, 2014, 2015 Russ Allbery <eagle@eyrie.org>
 *
 * SPDX-License-Identifier: MIT
 */

#include <assert.h>
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
    plan_lazy();
    bail("aborting");
    assert(0);
    return 0;
}
