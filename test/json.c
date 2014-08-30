/*
 * Copyright 2014 Jeffrey Kegler
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

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "marpa.h"

/* Scan to the location  past a JSON number.
 * */
static unsigned char*
scan_number (const unsigned char *s)
{
      unsigned char dig;
      if (*s == '-' || *s == '+') s++;
      if (*s == '0') {
	  s++;
      } else {
	while ((*s - '0') < 10) s++;
      }
      if (*s == '.') {
	      s++;
	while ((*s - '0') < 10) s++;
      }
	if (*s != 'e' && *s != 'E') return s;
      if (*s == '-' || *s == '+') s++;
	while ((*s - '0') < 10) s++;
    return s;
}

/* Scan to location past a JSON string.
 * Assumes we are pointing an initial double quote.
 * */
static unsigned char*
scan_string (unsigned char *s)
{
	s++;
	while (*s != '"') {
		 /* We are just looking for an unescaped double quote --
		  * we don't try to deal with hex chars at this point.
		  */
		 if (*s == '\\') {
		    s++;
		 }
		 s++;
	}
    return s;
}


static int
fail (const char *s, Marpa_Grammar g)
{
  const char *error_string;
  Marpa_Error_Code errcode = marpa_g_error (g, &error_string);
  printf ("%s returned %d: %s", s, errcode, error_string);
  exit (1);
}

/* Names follow RFC 7159 as much as possible */

int
main (int argc, char *argv[])
{
  int i;
  const char *error_string;
  struct stat sb;

  Marpa_Config marpa_configuration;
  /* From RFC 7159 */
  Marpa_Symbol_ID S_begin_array;
  Marpa_Symbol_ID S_begin_object;
  Marpa_Symbol_ID S_end_array;
  Marpa_Symbol_ID S_end_object;
  Marpa_Symbol_ID S_name_separator;
  Marpa_Symbol_ID S_value_separator;
  Marpa_Symbol_ID S_member;
  Marpa_Symbol_ID S_value;
  Marpa_Symbol_ID S_false;
  Marpa_Symbol_ID S_null;
  Marpa_Symbol_ID S_true;
  Marpa_Symbol_ID S_object;
  Marpa_Symbol_ID S_array;
  Marpa_Symbol_ID S_number;
  Marpa_Symbol_ID S_string;

  /* Additional */
  Marpa_Symbol_ID S_hash_contents;
  Marpa_Symbol_ID S_array_contents;

  Marpa_Grammar g;
  Marpa_Recognizer r;
  /* Longest rule is 4 symbols */
  Marpa_Symbol_ID rhs[4];

  int fd = open (argv[1], O_RDONLY);
  //initialize a stat for getting the filesize
  if (fstat (fd, &sb) == -1) {
    perror ("fstat");
    return 1;
  }
  //do the actual mmap, and keep pointer to the first element
  p =(char *) mmap (0, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
  q=p;
  //something went wrong
  if (p == MAP_FAILED) {
    perror ("mmap");
    return 1;
  }

  marpa_c_init (&marpa_configuration);
  g = marpa_g_new (&marpa_configuration);
  if (!g) {
    Marpa_Error_Code errcode = marpa_c_error (&marpa_configuration, &error_string);
    printf ("marpa_g_new returned %d: %s", errcode, error_string);
    exit (1);
  }

  ((S_begin_array = marpa_g_symbol_new(g)) >= 0) || fail("marpa_g_symbol_new", g);
  ((S_begin_object = marpa_g_symbol_new(g)) >= 0) || fail("marpa_g_symbol_new", g);
  ((S_end_array = marpa_g_symbol_new(g)) >= 0) || fail("marpa_g_symbol_new", g);
  ((S_end_object = marpa_g_symbol_new(g)) >= 0) || fail("marpa_g_symbol_new", g);
  ((S_name_separator = marpa_g_symbol_new(g)) >= 0) || fail("marpa_g_symbol_new", g);
  ((S_value_separator = marpa_g_symbol_new(g)) >= 0) || fail("marpa_g_symbol_new", g);
  ((S_member = marpa_g_symbol_new(g)) >= 0) || fail("marpa_g_symbol_new", g);
  ((S_value = marpa_g_symbol_new(g)) >= 0) || fail("marpa_g_symbol_new", g);
  ((S_false = marpa_g_symbol_new(g)) >= 0) || fail("marpa_g_symbol_new", g);
  ((S_null = marpa_g_symbol_new(g)) >= 0) || fail("marpa_g_symbol_new", g);
  ((S_true = marpa_g_symbol_new(g)) >= 0) || fail("marpa_g_symbol_new", g);
  ((S_object = marpa_g_symbol_new(g)) >= 0) || fail("marpa_g_symbol_new", g);
  ((S_array = marpa_g_symbol_new(g)) >= 0) || fail("marpa_g_symbol_new", g);
  ((S_number = marpa_g_symbol_new(g)) >= 0) || fail("marpa_g_symbol_new", g);
  ((S_string = marpa_g_symbol_new(g)) >= 0) || fail("marpa_g_symbol_new", g);
  ((S_hash_contents = marpa_g_symbol_new(g)) >= 0) || fail("marpa_g_symbol_new", g);
  ((S_array_contents = marpa_g_symbol_new(g)) >= 0) || fail("marpa_g_symbol_new", g);

  rhs[0] = S_false;
  (marpa_g_rule_new (g, S_value, rhs, 1) >= 0) || fail ("marpa_g_rule_new", g);
  rhs[0] = S_null;
  (marpa_g_rule_new (g, S_value, rhs, 1) >= 0) || fail ("marpa_g_rule_new", g);
  rhs[0] = S_true;
  (marpa_g_rule_new (g, S_value, rhs, 1) >= 0) || fail ("marpa_g_rule_new", g);
  rhs[0] = S_object;
  (marpa_g_rule_new (g, S_value, rhs, 1) >= 0) || fail ("marpa_g_rule_new", g);
  rhs[0] = S_array;
  (marpa_g_rule_new (g, S_value, rhs, 1) >= 0) || fail ("marpa_g_rule_new", g);
  rhs[0] = S_number;
  (marpa_g_rule_new (g, S_value, rhs, 1) >= 0) || fail ("marpa_g_rule_new", g);
  rhs[0] = S_string;
  (marpa_g_rule_new (g, S_value, rhs, 1) >= 0) || fail ("marpa_g_rule_new", g);

  rhs[0] = S_begin_array;
  rhs[1] = S_array_contents;
  rhs[2] = S_end_array;
  (marpa_g_rule_new (g, S_array, rhs, 3) >= 0) || fail ("marpa_g_rule_new", g);

  rhs[0] = S_begin_hash;
  rhs[1] = S_hash_contents;
  rhs[2] = S_end_hash;
  (marpa_g_rule_new (g, S_object, rhs, 3) >= 0) || fail ("marpa_g_rule_new", g);

  (marpa_g_sequence_new (g, S_array_contents, S_value, S_comma, MARPA_PROPER_SEPARATION) >= 0)
	  || fail ("marpa_g_sequence_new", g);
  (marpa_g_sequence_new (g, S_hash_contents, S_member, S_comma, MARPA_PROPER_SEPARATION) >= 0)
	  || fail ("marpa_g_sequence_new", g);

  rhs[0] = S_string;
  rhs[1] = S_name_separator;
  rhs[2] = S_value;
  (marpa_g_rule_new (g, S_member, rhs, 3) >= 0) || fail ("marpa_g_rule_new", g);

  if (0) {(marpa_g_symbol_is_terminal_set (g, a, 1) >= 0) ||
    fail ("marpa_g_symbol_is_terminal", g);
  }

  (marpa_g_start_symbol_set (g, S_value) >= 0) || fail ("marpa_g_start_symbol_set", g);
  if (marpa_g_precompute (g) < 0)
    {
      marpa_g_error (g, &error_string);
      puts (error_string);
      exit (1);
    }
  r = marpa_r_new (g);
  if (!r)
    {
      marpa_g_error (g, &error_string);
      puts (error_string);
      exit (1);
    }
  if (!marpa_r_start_input (r))
    {
      marpa_g_error (g, &error_string);
      puts (error_string);
      exit (1);
    }
  i = 0;
  while ( i < sb.st_size) 
    {
      Marpa_Symbol token;
      for (;;) {
      switch (p[i]) {
	      case '-': 
	      case '+': 
	      case '0': case '1': case '2': case '3': case '4':
	      case '5': case '6': case '7': case '8': case '9':
		      i = scan_number(p+i) - p;
		      token = S_number;
			 break;
	      case '"'
		      i = scan_string(p+i) - p;
		      token = S_string;
		      break;
	      case '[': token = S_begin_array; i++; break;
	      case ']': token = S_end_array; i++; break;
	      case '{': token = S_begin_object; i++; break;
	      case '}': token = S_end_object; i++; break;
	      case ',': token = S_value_separator; i++; break;
	      case ':': token = S_name_separator; i++; break;
	      case ' ': case 0x09: case 0x0A: case 0x0D: i++ continue;
	      default:
	  marpa_g_error (g, &error_string);
	  printf ("lexer failed at char %d: %c", i, p[i]);
	  exit (1);
      }
      }
      /* Token value of zero is not allowed, so we add one */
      int status = marpa_r_alternative (r, token, i+1, 1);
      if (status != MARPA_ERR_NONE)
	{
	  marpa_g_error (g, &error_string);
	  printf ("marpa_alternative returned %d: %s", status, error_string);
	  exit (1);
	}
      status = marpa_r_earleme_complete (r);
      if (status < 0)
	{
	  marpa_g_error (g, &error_string);
	  printf ("marpa_earleme_complete returned %d: %s", status,
		  error_string);
	  exit (1);
	}
      }
    }

    {
      Marpa_Bocage bocage;
      Marpa_Order order;
      Marpa_Tree tree;
      bocage = marpa_b_new (r, i);
      if (!bocage)
	{
	  int errcode = marpa_g_error (g, &error_string);
	  printf ("marpa_bocage_new returned %d: %s", errcode, error_string);
	  exit (1);
	}
      order = marpa_o_new (bocage);
      if (!order)
	{
	  int errcode = marpa_g_error (g, &error_string);
	  printf ("marpa_order_new returned %d: %s", errcode, error_string);
	  exit (1);
	}
      tree = marpa_t_new (order);
      if (!tree)
	{
	  Marpa_Error_Code errcode = marpa_g_error (g, &error_string);
	  printf ("marpa_t_new returned %d: %s", errcode, error_string);
	  exit (1);
	}
	{
	  Marpa_Value value = NULL;
	  int tree_status;
	  tree_status = marpa_t_next (tree);
	  if (tree_status < -1)
	    {
	      Marpa_Error_Code errcode = marpa_g_error (g, &error_string);
	      printf ("marpa_t_next returned %d: %s", errcode, error_string);
	      exit (1);
	    }
	  if (tree_status == -1)
	    break;
	  fprintf (stdout, "Tree #%d for length %d\n", tree_ordinal, i);
	  value = marpa_v_new (tree);
	  if (!value)
	    {
	      Marpa_Error_Code errcode = marpa_g_error (g, &error_string);
	      printf ("marpa_v_new returned %d: %s", errcode, error_string);
	      exit (1);
	    }
	  while (1)
	    {
	      Marpa_Step_Type step_type = marpa_v_step (value);
	      if (step_type < 0)
		{
		  Marpa_Error_Code errcode = marpa_g_error (g, &error_string);
		  printf ("marpa_v_event returned %d: %s", errcode,
			  error_string);
		  exit (1);
		}
	      if (step_type == MARPA_STEP_INACTIVE)
		{
		  printf ("No more events\n");
		  break;
		}
	      fprintf (stdout, "Step: %d %d %d %d %d\n",
		       marpa_v_token (value),
		       marpa_v_token_value (value),
		       marpa_v_rule (value),
		       marpa_v_arg_0 (value), marpa_v_arg_n (value));
	    }
	  /* if (value) marpa_v_unref (value); */
	}
      /* marpa_t_unref (tree); */
      /* marpa_o_unref (order); */
      /* marpa_b_unref (bocage); */
    }
  /* marpa_r_unref (r); */
  /* marpa_g_unref (g); */
  return 0;
}
