/*
 * Calls libtap with fewer tests than planned.
 *
 * This test exists to test the atexit output from libtap in a case of
 * fewer tests than planned and a plan greater than 1.
 *
 * Copyright 2009 Russ Allbery <eagle@eyrie.org>
 *
 * SPDX-License-Identifier: MIT
 */

#include <tests/tap/basic.h>

int
main(void)
{
    plan(2);

    ok(0, "first");

    return 0;
}
