/*
 * Test of the libtap diag and sysdiag functions.
 *
 * Copyright 2010 Russ Allbery <eagle@eyrie.org>
 *
 * SPDX-License-Identifier: MIT
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <tests/tap/basic.h>

int
main(void)
{
    FILE *output;

    output = fopen("c-diag.output", "w");
    if (output == NULL)
        sysbail("cannot create c-diag.output");
    fprintf(output, "1..2\n# foo\nok 1\n# error: %s\nnot ok 2\n",
            strerror(EPERM));
    fprintf(output, "# Looks like you failed 1 test of 2\n");
    fclose(output);
    plan(2);
    diag("foo");
    ok(1, NULL);
    errno = EPERM;
    sysdiag("error");
    ok(0, NULL);

    return 0;
}
