/*
 * Test bstrndup to duplicate strings.
 *
 * Copyright 2012, 2019 Russ Allbery <eagle@eyrie.org>
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <tests/tap/basic.h>


int
main(void)
{
    char *copy, *data;
    const char string[] = "Test string";

    copy = bstrndup(string, 100);
    printf("%s\n", copy);
    free(copy);
    copy = bstrndup(string, strlen(string));
    printf("%s\n", copy);
    free(copy);
    copy = bstrndup(string, 4);
    printf("%s\n", copy);
    free(copy);
    data = bcalloc_type(5, char);
    memset(data, 'a', 5);
    copy = bstrndup(data, 5);
    printf("%s\n", copy);
    free(data);
    free(copy);

    return 0;
}
