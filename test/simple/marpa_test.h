/*
 * Copyright 2015 Jeffrey Kegler
 * This file is part of Libmarpa.  Libmarpa is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * Libmarpa is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser
 * General Public License along with Libmarpa.  If not, see
 * http://www.gnu.org/licenses/.
 */

/* Libmarpa method test interface -- marpa_m_test */

#include <stdio.h>
#include "marpa.h"

#include "tap/basic.h"

extern Marpa_Symbol_ID S_invalid, S_no_such;
extern Marpa_Rule_ID R_invalid, R_no_such;

/* Marpa method test interface */

typedef int (*marpa_m_pointer)();

/*
    %s -- Marpa_Symbol_ID
    %r -- Marpa_Rule_ID
    %n -- Marpa_Rank
    ...
*/
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
#define ARGS_END (uintptr_t)-42424242
#define marpa_m_test(name, ...)  marpa_m_test_func(name, ##__VA_ARGS__, (ARGS_END))
int marpa_m_test_func(const char* name, ...);
