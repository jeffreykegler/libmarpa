/*
 * Test of the libtap skip_all function without a reason.
 *
 * Copyright 2009, 2015 Russ Allbery <eagle@eyrie.org>
 *
 * SPDX-License-Identifier: MIT
 */

#include <assert.h>
#include <stdio.h>

#include <tests/tap/basic.h>

int
main(void)
{
    skip_all(NULL);
    assert(0);

    return 0;
}
