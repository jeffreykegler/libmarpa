/*
 * Copyright 2015 Jeffrey Kegler
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

/* Libmarpa method test interface -- marpa_m_test */

#ifndef MARPA_M_TEST_H
#define MARPA_M_TEST_H 1

#include <stdio.h>
#include "marpa.h"

#include "tap/basic.h"

#define MARPA_M_MAX_ARG 5

extern Marpa_Symbol_ID S_invalid, S_no_such;
extern Marpa_Rule_ID R_invalid, R_no_such;

/* Marpa method test interface */

typedef int (*marpa_m_pointer)();

typedef char *marpa_m_argspec;
typedef char *marpa_m_name;

struct marpa_method_spec {
  marpa_m_name n;
  marpa_m_pointer p;
  marpa_m_argspec as;
};

typedef struct marpa_method_spec Marpa_Method_Spec;

extern const Marpa_Method_Spec methspec[];

typedef char *marpa_m_errmsg;
struct marpa_m_error {
  Marpa_Error_Code c;
  marpa_m_errmsg m;
};

typedef struct marpa_m_error Marpa_Method_Error;

extern const Marpa_Method_Error errspec[];

int marpa_m_grammar_set(Marpa_Grammar g);
Marpa_Grammar marpa_m_grammar();

char *marpa_m_error_message (Marpa_Error_Code error_code);
#define ARGS_END (unsigned int *)-42424242
#define marpa_m_test(name, ...)  marpa_m_test_func(name, ##__VA_ARGS__, (ARGS_END))
#define marpa_m_tests(names, ...) \
{ \
  int ix;\
  for (ix = 0; ix < sizeof(names) / sizeof(char *); ix++) \
  { \
    marpa_m_test_func(names[ix], ##__VA_ARGS__, (ARGS_END)); \
  } \
} \

int marpa_m_test_func(const char* name, ...);

typedef union {
    void *void_rv;
    int int_rv;
} API_RV;

typedef struct api_test_data {
    Marpa_Grammar g;
    Marpa_Error_Code expected_errcode;
    char *msg;
    API_RV rv_seen;
    Marpa_Error_Code err_seen;
} API_test_data;

#endif /* MARPA_M_TEST_H */
