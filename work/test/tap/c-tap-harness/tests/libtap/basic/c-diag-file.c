/*
 * Test of the libtap diag_file function.
 *
 * Copyright 2013, 2014 Russ Allbery <eagle@eyrie.org>
 *
 * SPDX-License-Identifier: MIT
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <tests/tap/basic.h>
#include <tests/tap/macros.h>

/*
 * A cleanup handler to remove our other stray log file.  This also tests that
 * the cleanup handlers run after the last check of diag_files.
 */
static void
cleanup_log(int success UNUSED, int primary)
{
    if (primary)
        unlink("c-diag-file.log1");
}


int
main(void)
{
    FILE *out1, *out2;

    /* Create the log files that will be interleaved. */
    out1 = fopen("c-diag-file.log1", "w");
    if (out1 == NULL)
        sysbail("cannot create c-diag-file.log1");
    out2 = fopen("c-diag-file.log2", "w");
    if (out2 == NULL)
        sysbail("cannot create c-diag-file.log2");

    /* Register a cleanup function to remove c-diag-file.log1. */
    test_cleanup_register(cleanup_log);

    /* Add one file before the plan. */
    diag_file_add("c-diag-file.log1");

    /* Output the plan. */
    plan(4);

    /* This output should show up as diagnostics before the next message. */
    fprintf(out1, "log1 line 1\n");
    fflush(out1);

    /* Add output to the second file before we add it. */
    fprintf(out2, "log2 line 1\n");
    fflush(out2);

    /* This should flush out the first diagnostic. */
    ok(1, NULL);

    /* Add the second file and then force it to flush out. */
    diag_file_add("c-diag-file.log2");
    diag("main diag");

    /* Add some data to the first file without a newline. */
    fprintf(out1, "log1 line 2");
    fflush(out1);
    is_int(1, 1, NULL);

    /* Now add the newline.  Only then should the output appear. */
    fprintf(out1, "\n");
    fflush(out1);
    fprintf(out2, "log2 line 2\nlog2 line 3\n");
    fflush(out2);
    is_string("foo", "foo", "some comment");

    /*
     * Remove the second diag file, after which nothing more in that file
     * should be added as diagnostics.
     */
    diag_file_remove("c-diag-file.log2");
    fprintf(out2, "log2 line 4\n");
    fflush(out2);
    fprintf(out1, "log1 line 3\n");
    fflush(out1);
    skip("some skipped test");

    /* Final output should appear before the test summary. */
    fprintf(out1, "log1 line 4\n");
    fclose(out1);
    fclose(out2);
    unlink("c-diag-file.log2");

    /* c-diag-file.log1 has to be removed in the harness. */
    return 0;
}
