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

# Inspired from /usr/share/autoconf/autoconf/c.m4
#
# Note: __forceinline also exist, but is not recommended if the C compiler
#       think it is not worth inlining
#
FOREACH(KEYWORD "inline" "__inline__" "inline__" "__inline")
   IF(NOT DEFINED C_INLINE)
     MESSAGE("-- Looking for ${KEYWORD}")
     TRY_COMPILE(C_HAS_${KEYWORD} ${CMAKE_CURRENT_BINARY_DIR}
       ${CMAKE_CURRENT_SOURCE_DIR}/modules/inline.c
       COMPILE_DEFINITIONS "-DC_INLINE=${KEYWORD}")
     IF(C_HAS_${KEYWORD})
       MESSAGE("-- Looking for ${KEYWORD} - found")
       SET(C_INLINE TRUE)
       ADD_DEFINITIONS("-DC_INLINE=${KEYWORD}")
     ELSE(C_HAS_${KEYWORD})
       MESSAGE("-- Looking for ${KEYWORD} - not found")
     ENDIF(C_HAS_${KEYWORD})
   ENDIF(NOT DEFINED C_INLINE)
ENDFOREACH(KEYWORD)
IF(NOT DEFINED C_INLINE)
   ADD_DEFINITIONS("-DC_INLINE=")
ENDIF(NOT DEFINED C_INLINE)
