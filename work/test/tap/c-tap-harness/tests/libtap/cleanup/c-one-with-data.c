/*
 * Test of the libtap test_cleanup_register_with_data function.
 *
 * Copyright 2018-2019 Russ Allbery <eagle@eyrie.org>
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdio.h>
#include <stdlib.h>

#include <tests/tap/basic.h>


/*
 * The test function to call during cleanup.
 */
static void
test(int success, int primary, void *data)
{
    int *value = (int *) data;

    printf("Called cleanup with %d %d %d\n", success, primary, *value);
    free(value);
}


int
main(void)
{
    int *data = bcalloc_type(1, int);

    *data = 99;
    test_cleanup_register_with_data(test, data);
    plan(1);
    ok(1, "some test");
    return 0;
}
