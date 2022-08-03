/*
 * Calls libtap with more tests than planned.
 *
 * This test exists to test the atexit output from libtap in a case of more
 * tests than planned and a plan of only a single test.
 *
 * Copyright 2009 Russ Allbery <eagle@eyrie.org>
 *
 * SPDX-License-Identifier: MIT
 */

#include <tests/tap/basic.h>

int
main(void)
{
    plan(1);

    ok(1, "first");
    ok(2, "second");
    ok(3, "third");

    return 0;
}
