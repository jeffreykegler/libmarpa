/*
 * Test of the libtap sysbail and sysdiag functions.
 *
 * Copyright 2009, 2013, 2014, 2015 Russ Allbery <eagle@eyrie.org>
 *
 * SPDX-License-Identifier: MIT
 */

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <tests/tap/basic.h>

int
main(void)
{
    FILE *output;
    int status;

    output = fopen("c-sysbail.output", "w");
    if (output == NULL)
        sysbail("cannot create c-sysbail.output");
    fprintf(output, "1..2\n# diag: %s\nok 1\nBail out! error: %s\n",
            strerror(EPERM), strerror(EPERM));
    fclose(output);
    plan(2);
    errno = EPERM;
    status = sysdiag("diag");
    is_int(1, status, NULL);
    errno = EPERM;
    sysbail("error");
    assert(0);

    return 0;
}
