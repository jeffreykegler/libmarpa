# Copyright 2018 Jeffrey Kegler
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
# OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
# ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.

MACRO(NULL_IS_ZEROES)
  write_file("${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeTmp/cmake_try_compile.c"
    "
    #include <stdlib.h>
    #include <string.h>
    int main() {
      char s[sizeof(void *)];
      char *p = NULL;
      int i;
      for (i = 0; i < sizeof(void *); i++) {
        s[i] = 0;
      }
      if (memcmp(&p, s, sizeof(void *)) != 0) {
        exit(1);
      }
      return 0;
    }
    ")
  MESSAGE("-- Checking if NULL == 0")
  try_compile(HAVE_NULL_IS_ZEROES ${CMAKE_BINARY_DIR} ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeTmp/cmake_try_compile.c)
  if(HAVE_NULL_IS_ZEROES)
    MESSAGE("-- Checking if NULL == 0 - yes")
    SET(NULL_IS_ZEROES 1 CACHE STRING "NULL is a sequence of zeroes")
  else()
    MESSAGE("-- Checking if NULL == 0 - no")
  endif()
ENDMACRO()
NULL_IS_ZEROES()
