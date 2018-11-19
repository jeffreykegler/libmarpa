/*
 * Copyright 2018 Jeffrey Kegler
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

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "marpa.h"

/* Scan to the location  past a JSON number.
 * */
static const unsigned char *
scan_number (const unsigned char *s, const unsigned char *end)
{
  unsigned char dig;

  if (*s == '-' || *s == '+')
    s++;
  if (s == end)
    return s;
  if (*s == '0')
    {
      s++;
      if (s == end)
        return s;
    }
  else
    {
      while ((unsigned char) (*s - '0') < 10)
        {
          s++;
          if (s == end)
            return s;
        }
    }
  if (*s == '.')
    {
      s++;
      if (s == end)
        return s;
      while ((unsigned) (*s - '0') < 10)
        {
          s++;
          if (s == end)
            return s;
        }
    }
  if (*s != 'e' && *s != 'E')
    return s;
  if (*s == '-' || *s == '+')
    s++;
  if (s == end)
    return s;
  while ((unsigned) (*s - '0') < 10)
    {
      s++;
      if (s == end)
        return s;
    }
  return s;
}

/* Scan to location past a JSON string.
 * Assumes we are pointing an initial double quote.
 * */
static const unsigned char *
scan_string (const unsigned char *s, const unsigned char *end)
{
  s++;
  if (s == end)
    return s;
  while (*s != '"')
    {
      /* We are just looking for an unescaped double quote --
       * we don't try to deal with hex chars at this point.
       */
      if (*s == '\\')
        {
          s++;
          if (s == end)
            return s;
        }
      s++;
      if (s == end)
        return s;
    }
  s++;
  return s;
}

/* Scan to location past a literal constant
 * */
static const unsigned char *
scan_constant (const unsigned char *target, const unsigned char *s,
               const unsigned char *end)
{
  const unsigned char *t = target + 1;
  s++;
  if (s == end)
    return s;
  while (*t)
    {
      if (*s != *t)
        return s;
      s++;
      if (s == end)
        return s;
      t++;
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
Marpa_Symbol_ID S_object_contents;
Marpa_Symbol_ID S_array_contents;

/* For fatal error messages */
char error_buffer[80];

/* Names follow RFC 7159 as much as possible */
char *
symbol_name (Marpa_Symbol_ID id)
{
  if (id == S_begin_array)
    return "begin_array";
  if (id == S_begin_object)
    return "begin_object";
  if (id == S_end_array)
    return "end_array";
  if (id == S_end_object)
    return "end_object";
  if (id == S_name_separator)
    return "name_separator";
  if (id == S_value_separator)
    return "value_separator";
  if (id == S_member)
    return "member";
  if (id == S_value)
    return "value";
  if (id == S_false)
    return "false";
  if (id == S_null)
    return "null";
  if (id == S_true)
    return "true";
  if (id == S_object)
    return "object";
  if (id == S_array)
    return "array";
  if (id == S_number)
    return "number";
  if (id == S_string)
    return "string";
  if (id == S_object_contents)
    return "object_contents";
  if (id == S_array_contents)
    return "array_contents";
  sprintf (error_buffer, "no such symbol: %d", id);
  return error_buffer;
};

int
main (int argc, char *argv[])
{
  const unsigned char *p, *eof;
  int i;
  const char *error_string;
  struct stat sb;

  Marpa_Config marpa_configuration;

  Marpa_Grammar g;
  Marpa_Recognizer r;
  /* Longest rule is 4 symbols */
  Marpa_Symbol_ID rhs[4];

  int fd = open (argv[1], O_RDONLY);
  //initialize a stat for getting the filesize
  if (fstat (fd, &sb) == -1)
    {
      perror ("fstat");
      return 1;
    }
  //do the actual mmap, and keep pointer to the first element
  p = (unsigned char *) mmap (0, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
  //something went wrong
  if (p == MAP_FAILED)
    {
      perror ("mmap");
      return 1;
    }

  marpa_c_init (&marpa_configuration);
  g = marpa_g_new (&marpa_configuration);
  if (!g)
    {
      Marpa_Error_Code errcode =
        marpa_c_error (&marpa_configuration, &error_string);
      printf ("marpa_g_new returned %d: %s", errcode, error_string);
      exit (1);
    }

  ((S_begin_array = marpa_g_symbol_new (g)) >= 0)
    || fail ("marpa_g_symbol_new", g);
  ((S_begin_object = marpa_g_symbol_new (g)) >= 0)
    || fail ("marpa_g_symbol_new", g);
  ((S_end_array = marpa_g_symbol_new (g)) >= 0)
    || fail ("marpa_g_symbol_new", g);
  ((S_end_object = marpa_g_symbol_new (g)) >= 0)
    || fail ("marpa_g_symbol_new", g);
  ((S_name_separator = marpa_g_symbol_new (g)) >= 0)
    || fail ("marpa_g_symbol_new", g);
  ((S_value_separator = marpa_g_symbol_new (g)) >= 0)
    || fail ("marpa_g_symbol_new", g);
  ((S_member = marpa_g_symbol_new (g)) >= 0)
    || fail ("marpa_g_symbol_new", g);
  ((S_value = marpa_g_symbol_new (g)) >= 0) || fail ("marpa_g_symbol_new", g);
  ((S_false = marpa_g_symbol_new (g)) >= 0) || fail ("marpa_g_symbol_new", g);
  ((S_null = marpa_g_symbol_new (g)) >= 0) || fail ("marpa_g_symbol_new", g);
  ((S_true = marpa_g_symbol_new (g)) >= 0) || fail ("marpa_g_symbol_new", g);
  ((S_object = marpa_g_symbol_new (g)) >= 0)
    || fail ("marpa_g_symbol_new", g);
  ((S_array = marpa_g_symbol_new (g)) >= 0) || fail ("marpa_g_symbol_new", g);
  ((S_number = marpa_g_symbol_new (g)) >= 0)
    || fail ("marpa_g_symbol_new", g);
  ((S_string = marpa_g_symbol_new (g)) >= 0)
    || fail ("marpa_g_symbol_new", g);
  ((S_object_contents = marpa_g_symbol_new (g)) >= 0)
    || fail ("marpa_g_symbol_new", g);
  ((S_array_contents = marpa_g_symbol_new (g)) >= 0)
    || fail ("marpa_g_symbol_new", g);

  rhs[0] = S_false;
  (marpa_g_rule_new (g, S_value, rhs, 1) >= 0)
    || fail ("marpa_g_rule_new", g);
  rhs[0] = S_null;
  (marpa_g_rule_new (g, S_value, rhs, 1) >= 0)
    || fail ("marpa_g_rule_new", g);
  rhs[0] = S_true;
  (marpa_g_rule_new (g, S_value, rhs, 1) >= 0)
    || fail ("marpa_g_rule_new", g);
  rhs[0] = S_object;
  (marpa_g_rule_new (g, S_value, rhs, 1) >= 0)
    || fail ("marpa_g_rule_new", g);
  rhs[0] = S_array;
  (marpa_g_rule_new (g, S_value, rhs, 1) >= 0)
    || fail ("marpa_g_rule_new", g);
  rhs[0] = S_number;
  (marpa_g_rule_new (g, S_value, rhs, 1) >= 0)
    || fail ("marpa_g_rule_new", g);
  rhs[0] = S_string;
  (marpa_g_rule_new (g, S_value, rhs, 1) >= 0)
    || fail ("marpa_g_rule_new", g);

  rhs[0] = S_begin_array;
  rhs[1] = S_array_contents;
  rhs[2] = S_end_array;
  (marpa_g_rule_new (g, S_array, rhs, 3) >= 0)
    || fail ("marpa_g_rule_new", g);

  rhs[0] = S_begin_object;
  rhs[1] = S_object_contents;
  rhs[2] = S_end_object;
  (marpa_g_rule_new (g, S_object, rhs, 3) >= 0)
    || fail ("marpa_g_rule_new", g);

  (marpa_g_sequence_new
   (g, S_array_contents, S_value, S_value_separator, 0,
    MARPA_PROPER_SEPARATION) >= 0) || fail ("marpa_g_sequence_new", g);
  (marpa_g_sequence_new
   (g, S_object_contents, S_member, S_value_separator, 0,
    MARPA_PROPER_SEPARATION) >= 0) || fail ("marpa_g_sequence_new", g);

  rhs[0] = S_string;
  rhs[1] = S_name_separator;
  rhs[2] = S_value;
  (marpa_g_rule_new (g, S_member, rhs, 3) >= 0)
    || fail ("marpa_g_rule_new", g);

  if (0)
    {
      (marpa_g_symbol_is_terminal_set (g, S_value_separator, 1) >= 0) ||
        fail ("marpa_g_symbol_is_terminal", g);
    }

  (marpa_g_start_symbol_set (g, S_value) >= 0)
    || fail ("marpa_g_start_symbol_set", g);
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
  eof = p + sb.st_size;
  while (p + i < eof)
    {
      Marpa_Symbol_ID token;
      const int start_of_token = i;

      switch (p[i])
        {
        case '-':
        case '+':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
          i = scan_number (p + i, eof) - p;
          token = S_number;
          break;
        case '"':
          i = scan_string (p + i, eof) - p;
          token = S_string;
          break;
        case '[':
          token = S_begin_array;
          i++;
          break;
        case ']':
          token = S_end_array;
          i++;
          break;
        case '{':
          token = S_begin_object;
          i++;
          break;
        case '}':
          token = S_end_object;
          i++;
          break;
        case ',':
          token = S_value_separator;
          i++;
          break;
        case ':':
          token = S_name_separator;
          i++;
          break;
        case 'n':
          i = scan_constant ("null", (p + i), eof) - p;
          token = S_null;
          break;
        case ' ':
        case 0x09:
        case 0x0A:
        case 0x0D:
          i++;
          goto NEXT_TOKEN;
        default:
          marpa_g_error (g, &error_string);
          printf ("lexer failed at char %d: '%c'", i, p[i]);
          exit (1);
        }
      /* Token value of zero is not allowed, so we add one */
      if (0)
        fprintf (stderr, "reading token %ld, %s\n",
                 (long) token, symbol_name (token));
      int status = marpa_r_alternative (r, token, start_of_token + 1, 1);
      if (status != MARPA_ERR_NONE)
        {
          Marpa_Symbol_ID expected[20];
          int count_of_expected = marpa_r_terminals_expected (r, expected);
          int i;
          for (i = 0; i < count_of_expected; i++)
            {
              fprintf (stderr, "expecting symbol %ld, %s\n",
                       (long) i, symbol_name (expected[i]));
            }
          marpa_g_error (g, &error_string);
          fprintf (stderr,
                   "marpa_alternative(%p,%ld,%s,%ld,1) returned %d: %s", r,
                   (long) token, symbol_name (token),
                   (long) (start_of_token + 1), status, error_string);
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
    NEXT_TOKEN:;
    }

  {
    Marpa_Bocage bocage;
    Marpa_Order order;
    Marpa_Tree tree;
    bocage = marpa_b_new (r, -1);
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
      int column = 0;
      int tree_status;
      tree_status = marpa_t_next (tree);
      if (tree_status <= -1)
        {
          Marpa_Error_Code errcode = marpa_g_error (g, &error_string);
          printf ("marpa_t_next returned %d: %s", errcode, error_string);
          exit (1);
        }

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
          Marpa_Symbol_ID token;
          if (step_type < 0)
            {
              Marpa_Error_Code errcode = marpa_g_error (g, &error_string);
              printf ("marpa_v_event returned %d: %s", errcode, error_string);
              exit (1);
            }
          if (step_type == MARPA_STEP_INACTIVE)
            {
              if (0)
                printf ("No more events\n");
              break;
            }
          if (step_type != MARPA_STEP_TOKEN)
            continue;
          token = marpa_v_token (value);
          if (1)
            {
              if (column > 60)
                {
                  putchar ('\n');
                  column = 0;
                }
              if (token == S_begin_array)
                {
                  putchar ('[');
                  column++;
                  continue;
                }
              if (token == S_end_array)
                {
                  putchar (']');
                  column++;
                  continue;
                }
              if (token == S_begin_object)
                {
                  putchar ('{');
                  column++;
                  continue;
                }
              if (token == S_end_object)
                {
                  putchar ('}');
                  column++;
                  continue;
                }
              if (token == S_name_separator)
                {
                  putchar (':');
                  column++;
                  continue;
                }
              if (token == S_value_separator)
                {
                  putchar (',');
                  column++;
                  continue;
                }
              if (token == S_null)
                {
                  fputs ("undef", stdout);
                  column += 5;
                  continue;
                }
              if (token == S_true)
                {
                  putchar ('1');
                  column++;
                  continue;
                }
              if (token == S_false)
                {
                  putchar ('0');
                  column++;
                  continue;
                }
              if (token == S_number)
                {
                  /* We added one to avoid zero
                   * Now we must subtract it
                   */
                  int i;
                  const int start_of_number = marpa_v_token_value (value) - 1;
                  const int end_of_number =
                    scan_number (p + start_of_number, eof) - p;
                  column += 2 + (end_of_number - start_of_number);

                  /* We output numbers as Perl strings */
                  putchar ('"');
                  for (i = start_of_number; i < end_of_number; i++)
                    {
                      putchar (p[i]);
                    }
                  putchar ('"');
                  continue;
                }
              if (token == S_string)
                {
                  /* We added one to avoid zero
                   * Now we must subtract it, but we also
                   * add one for the initial double quote
                   */
                  int i;
                  const int start_of_string = marpa_v_token_value (value);
                  /* Subtract one for the final double quote */
                  const int end_of_string =
                    (scan_string (p + start_of_string, eof) - p) - 1;

                  /* We add back the inital and final double quotes,
                   * and increment the column accordingly.
                   */
                  column += 2;
                  putchar ('"');
                  i = start_of_string;
                  while (i < end_of_string)
                    {
                      const unsigned char ch0 = p[i++];
                      if (ch0 == '\\')
                        {
                          const unsigned char ch1 = p[i++];
                          switch (ch1)
                            {
                            case '\\':
                            case '/':
                            case '"':
                            case 'b':
                            case 'f':
                            case 'n':
                            case 'r':
                            case 't':
                              /* explicit non-hex JSON escapes are the same
                               * as the Perl escapes */
                              column += 2;
                              putchar ('\\');
                              putchar (ch1);
                              continue;
                            case 'u':
                              {
                                int digit;
                                putchar ('x');
                                putchar ('{');
                                for (digit = 0; digit < 4; digit++)
                                  {
                                    const unsigned char hex_ch = p[i + digit];
                                    if ((hex_ch >= 'a' && hex_ch <= 'f')
                                        || (hex_ch >= 'A' && hex_ch <= 'F')
                                        || (hex_ch >= '0' && hex_ch <= '9'))
                                      {
                                        printf
                                          ("illegal char in JSON hex number at location %d (0x%x): '%c' ",
                                           i, hex_ch, hex_ch);
                                        exit (1);
                                      }
                                    putchar (hex_ch);
                                  }
                                putchar ('}');
                                column += 7;
                                i += 4;
                              }
                              continue;
                            default:
                              printf
                                ("illegal escaped char in JSON input (0x%x):'%c' ",
                                 i, p[i]);
                              exit (1);
                            }
                        }

                      /* An unescaped JSON char, one that does not need Perl escaping */
                      if (ch0 == '_' || (ch0 >= '0' && ch0 <= '9')
                          || (ch0 >= 'a' && ch0 <= 'z') || (ch0 >= 'A'
                                                            && ch0 <= 'Z'))
                        {
                          putchar (ch0);
                          column++;
                          continue;
                        }
                      /* An unescaped JSON char,
                       * but one which quotemeta would escape for Perl */
                      putchar ('\\');
                      putchar (ch0);
                      column += 2;
                      continue;
                    }
                  putchar ('"');
                  continue;
                }
              fprintf (stderr, "Unknown symbol %s at %d",
                       symbol_name (token), marpa_v_token_value (value) - 1);
              exit (1);
            }
        }
      if (column > 60)
        {
          putchar ('\n');
          column = 0;
        }
    }
  }

  return 0;
}
