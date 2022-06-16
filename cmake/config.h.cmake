/*
 * Copyright 2022 Jeffrey Kegler
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

#ifndef CONFIG_H
#define CONFIG_H

/*
   Name : MARPA_LIB_VERSION
   Usage: Version of @PROJECT_NAME@
*/
#define MARPA_LIB_VERSION "@MARPA_LIB_VERSION@"
#define MARPA_LIB_MAJOR_VERSION @MARPA_LIB_MAJOR_VERSION@
#define MARPA_LIB_MINOR_VERSION @MARPA_LIB_MINOR_VERSION@
#define MARPA_LIB_MICRO_VERSION @MARPA_LIB_MICRO_VERSION@
#define HAVE_STDINT_H @HAVE_STDINT_H@
#define HAVE_INTTYPES_H @HAVE_INTTYPES_H@

#endif /* CONFIG_H */
