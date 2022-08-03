/*
 * Test locating files with test_file_path().
 *
 * Written by Russ Allbery <eagle@eyrie.org>
 * Copyright 2016 Russ Allbery <eagle@eyrie.org>
 * Copyright 2010, 2012
 *     The Board of Trustees of the Leland Stanford Junior University
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdio.h>
#include <stdlib.h>

#include <tests/tap/basic.h>


int
main(void)
{
    char *path;
    FILE *output;

    output = fopen("c-file.output", "w");
    if (output == NULL)
        sysbail("cannot create c-file.output");
    fprintf(output, "Path to c-file: %s/libtap/basic/c-file\n",
            getenv("C_TAP_BUILD"));
    fprintf(output, "Path to c-basic.output: %s/libtap/basic/c-basic.output\n",
            getenv("C_TAP_SOURCE"));
    fclose(output);
    path = test_file_path("libtap/basic/c-file");
    printf("Path to c-file: %s\n", path);
    test_file_path_free(path);
    path = test_file_path("libtap/basic/c-basic.output");
    printf("Path to c-basic.output: %s\n", path);
    test_file_path_free(path);

    return 0;
}
