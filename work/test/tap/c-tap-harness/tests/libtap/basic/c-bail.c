/*
 * Test of the libtap bail function.
 *
 * Copyright 2009, 2015 Russ Allbery <eagle@eyrie.org>
 *
 * SPDX-License-Identifier: MIT
 */

#include <assert.h>

#include <tests/tap/basic.h>

int
main(void)
{
    plan(2);
    ok(1, "first test");
    bail("some error message");
    assert(0);

    return 0;
}
