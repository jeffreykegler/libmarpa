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
