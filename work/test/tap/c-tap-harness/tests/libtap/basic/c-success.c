/*
 * Calls libtap with only successful tests.
 *
 * This test exists to test the atexit output from libtap in a case of only
 * successful tests and more than one.
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
    skip("second");

    return 0;
}
