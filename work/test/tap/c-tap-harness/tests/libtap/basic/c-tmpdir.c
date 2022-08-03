/*
 * Test creating a temporary directory with test_tmpdir().
 *
 * Copyright 2011, 2013, 2016, 2019-2020 Russ Allbery <eagle@eyrie.org>
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <tests/tap/basic.h>


int
main(void)
{
    char *path, *tmp;
    FILE *output;
    const char *build;
    struct stat st;
    size_t length;
    int exists;

    /* Create the expected output file. */
    output = fopen("c-tmpdir.output", "w");
    if (output == NULL)
        sysbail("cannot create c-tmpdir.output");
    fprintf(output, "Path to temporary directory: %s/tmp\n",
            getenv("C_TAP_BUILD"));
    fclose(output);

    /* Check if the path already exists. */
    build = getenv("C_TAP_BUILD");
    length = strlen(build) + strlen("/tmp") + 1;
    path = bcalloc_type(length, char);
    sprintf(path, "%s/tmp", build);
    exists = (access(path, F_OK) == 0);
    free(path);

    /*
     * Check that the directory is created correctly.  This isn't always a
     * good test; if the directory already exists (which will happen with
     * valgrind tests), we only check that the path is correct.  But normally
     * it shouldn't already exist and fixing this is hard, so live with it.
     */
    path = test_tmpdir();
    printf("Path to temporary directory: %s\n", path);
    if (stat(path, &st) < 0)
        sysbail("cannot stat %s", path);
    if (!S_ISDIR(st.st_mode))
        sysbail("%s is not a directory", path);
    tmp = bstrdup(path);
    test_tmpdir_free(path);

    /* If the directory already existed, we may not be able to remove it. */
    if (!exists && stat(tmp, &st) == 0)
        bail("temporary directory not removed");
    free(tmp);

    return 0;
}
