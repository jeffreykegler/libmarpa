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
