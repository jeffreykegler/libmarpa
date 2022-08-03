/*
 * Calls libtap with more tests than planned.
 *
 * This test exists to test the atexit output from libtap in a case of more
 * tests than planned and a plan greater than 1.
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

    ok(1, "first");
    ok(0, "second");
    ok(1, "third");

    return 0;
}
