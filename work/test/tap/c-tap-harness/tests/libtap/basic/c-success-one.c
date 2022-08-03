/*
 * Calls libtap with only one successful test.
 *
 * This test exists to test the atexit output from libtap in a case of only
 * one successful test.
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

    return 0;
}
