
option(LUA_SKIP_TOOLS "Skip installing tools" OFF)

set(LUA_VERSION_TO_BUILD "5.4.4")

set(LUA_SOURCE_DIR "${CMAKE_SOURCE_DIR}/marpalua/src")
set(_LUA_LIB_SRC
  "${LUA_SOURCE_DIR}/lapi.c"
  "${LUA_SOURCE_DIR}/lcode.c"
  "${LUA_SOURCE_DIR}/lctype.c"
  "${LUA_SOURCE_DIR}/ldebug.c"
  "${LUA_SOURCE_DIR}/ldo.c"
  "${LUA_SOURCE_DIR}/ldump.c"
  "${LUA_SOURCE_DIR}/lfunc.c"
  "${LUA_SOURCE_DIR}/lgc.c"
  "${LUA_SOURCE_DIR}/llex.c"
  "${LUA_SOURCE_DIR}/lmem.c"
  "${LUA_SOURCE_DIR}/lobject.c"
  "${LUA_SOURCE_DIR}/lopcodes.c"
  "${LUA_SOURCE_DIR}/lparser.c"
  "${LUA_SOURCE_DIR}/lstate.c"
  "${LUA_SOURCE_DIR}/lstring.c"
  "${LUA_SOURCE_DIR}/ltable.c"
  "${LUA_SOURCE_DIR}/ltm.c"
  "${LUA_SOURCE_DIR}/lundump.c"
  "${LUA_SOURCE_DIR}/lvm.c"
  "${LUA_SOURCE_DIR}/lzio.c"
  "${LUA_SOURCE_DIR}/lauxlib.c"
  "${LUA_SOURCE_DIR}/lbaselib.c"
  "${LUA_SOURCE_DIR}/lcorolib.c"
  "${LUA_SOURCE_DIR}/ldblib.c"
  "${LUA_SOURCE_DIR}/liolib.c"
  "${LUA_SOURCE_DIR}/lmathlib.c"
  "${LUA_SOURCE_DIR}/loadlib.c"
  "${LUA_SOURCE_DIR}/loslib.c"
  "${LUA_SOURCE_DIR}/lstrlib.c"
  "${LUA_SOURCE_DIR}/ltablib.c"
  "${LUA_SOURCE_DIR}/lutf8lib.c"
  "${LUA_SOURCE_DIR}/linit.c"
)

set(_LUA_COMPILER_SRC ${LUA_SOURCE_DIR}/luac.c)
set(_LUA_INTERPRETER_SRC ${LUA_SOURCE_DIR}/lua.c)

add_library(lua-obj OBJECT ${_LUA_LIB_SRC})
target_compile_options(lua-obj PUBLIC -Wall -Wextra)
# target_compile_definitions(lua-obj PUBLIC LUA_COMPAT_5_3)

if(BUILD_SHARED_LIBS)
  set_target_properties(lua-obj PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()

set_target_properties(lua-obj PROPERTIES
  C_STANDARD 99
  C_EXTENSIONS OFF
)

if(WIN32)
  target_compile_definitions(lua-obj PUBLIC _CRT_SECURE_NO_WARNINGS)
endif()

if(UNIX)
  if(NOT ANDROID)
    target_compile_definitions(lua-obj PUBLIC LUA_USE_POSIX)
  endif()
  find_library(LIB_MATH NAMES m)
  if(LIB_MATH)
    target_link_libraries(lua-obj PUBLIC ${LIB_MATH})
  endif()
  mark_as_advanced(LIB_MATH)
  if(APPLE)
    target_compile_definitions(lua-obj PRIVATE LUA_USE_DLOPEN)
  else()
    find_library(LIB_DLOPEN NAMES dl)
    if(LIB_DLOPEN)
      target_compile_definitions(lua-obj PRIVATE LUA_USE_DLOPEN)
      target_link_libraries(lua-obj PRIVATE ${LIB_DLOPEN})
    endif()
    mark_as_advanced(LIB_DLOPEN)
  endif()
endif()

add_library(lua $<TARGET_OBJECTS:lua-obj>)
target_compile_options(lua PUBLIC -Wall -Wextra)
target_compile_definitions(lua PUBLIC LUA_COMPAT_5_3)

# for compatibility with the FindLua.cmake module
add_library(Lua::lua ALIAS lua)
target_link_libraries(lua PUBLIC $<BUILD_INTERFACE:lua-obj>)

if(BUILD_SHARED_LIBS AND WIN32)
  target_compile_definitions(lua PUBLIC LUA_BUILD_AS_DLL)
endif()

# install
install(
  FILES
    ${LUA_SOURCE_DIR}/lualib.h
    ${LUA_SOURCE_DIR}/lua.h
    ${LUA_SOURCE_DIR}/luaconf.h
    ${LUA_SOURCE_DIR}/lauxlib.h
  DESTINATION include
  COMPONENT dev
)

if(NOT LUA_SKIP_TOOLS)
  # luac uses non-exported APIs
  add_executable(luac ${_LUA_COMPILER_SRC} $<TARGET_OBJECTS:lua-obj>)
  target_compile_options(luac PUBLIC -Wall -Wextra)
  target_compile_definitions(luac PUBLIC LUA_COMPAT_5_3)
  target_link_libraries(luac PRIVATE $<BUILD_INTERFACE:lua-obj>)

  # interpreter
  add_executable(luai ${_LUA_INTERPRETER_SRC})
  target_compile_options(luai PUBLIC -Wall -Wextra)
  target_compile_definitions(luai PUBLIC LUA_COMPAT_5_3)
  target_link_libraries(luai PRIVATE lua)

  set_target_properties(luai PROPERTIES OUTPUT_NAME lua PDB_NAME luai)
  if(UNIX)
    if(CMAKE_SYSTEM_NAME STREQUAL FreeBSD)
      set(_LIB_READLINE_NAME edit)
    else()
      set(_LIB_READLINE_NAME readline)
    endif()
    find_library(LIB_READLINE NAMES ${_LIB_READLINE_NAME})
    if(LIB_READLINE)
      target_compile_definitions(luai PRIVATE LUA_USE_READLINE)
      target_link_libraries(luai PRIVATE ${LIB_READLINE})
      if(_LIB_READLINE_NAME STREQUAL edit)
        TARGET_INCLUDE_DIRECTORIES (luai PUBLIC /usr/include/edit)
      endif()
    endif()
  endif()
  mark_as_advanced(LIB_READLINE)
  install(
    TARGETS
      luai
      luac 
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/
    COMPONENT bin
  )
endif()
