/*
 * Calls libtap with lazy planning.
 *
 * This test exists to test the atexit output from libtap in a case of only
 * successful tests, more than one, and lazy planning.
 *
 * Copyright 2010 Russ Allbery <eagle@eyrie.org>
 *
 * SPDX-License-Identifier: MIT
 */

#include <tests/tap/basic.h>

int
main(void)
{
    plan_lazy();

    ok(1, "first");
    skip("second");

    return 0;
}
