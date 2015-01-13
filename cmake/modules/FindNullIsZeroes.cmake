# Copyright 2015 Jeffrey Kegler
# This file is part of Libmarpa.  Libmarpa is free software: you can
# redistribute it and/or modify it under the terms of the GNU Lesser
# General Public License as published by the Free Software Foundation,
# either version 3 of the License, or (at your option) any later version.
#
# Libmarpa is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser
# General Public License along with Libmarpa.  If not, see
# http://www.gnu.org/licenses/.

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
