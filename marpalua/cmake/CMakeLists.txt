
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

add_library(lualib-obj OBJECT ${_LUA_LIB_SRC})
target_compile_options(lualib-obj PUBLIC -Wall -Wextra)
# target_compile_definitions(lualib-obj PUBLIC LUA_COMPAT_5_3)

if(BUILD_SHARED_LIBS)
  set_target_properties(lualib-obj PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()

set_target_properties(lualib-obj PROPERTIES
  C_STANDARD 99
  C_EXTENSIONS OFF
)

if(WIN32)
  target_compile_definitions(lualib-obj PUBLIC _CRT_SECURE_NO_WARNINGS)
endif()

if(UNIX)
  if(NOT ANDROID)
    target_compile_definitions(lualib-obj PUBLIC LUA_USE_POSIX)
  endif()
  find_library(LIB_MATH NAMES m)
  if(LIB_MATH)
    target_link_libraries(lualib-obj PUBLIC ${LIB_MATH})
  endif()
  mark_as_advanced(LIB_MATH)
  if(APPLE)
    target_compile_definitions(lualib-obj PRIVATE LUA_USE_DLOPEN)
  else()
    find_library(LIB_DLOPEN NAMES dl)
    if(LIB_DLOPEN)
      target_compile_definitions(lualib-obj PRIVATE LUA_USE_DLOPEN)
      target_link_libraries(lualib-obj PRIVATE ${LIB_DLOPEN})
    endif()
    mark_as_advanced(LIB_DLOPEN)
  endif()
endif()

add_library(marpalua $<TARGET_OBJECTS:lualib-obj>)
target_compile_options(marpalua PUBLIC -Wall -Wextra)
# target_compile_definitions(marpalua PUBLIC LUA_COMPAT_5_3)
target_link_libraries(marpalua PUBLIC $<BUILD_INTERFACE:lualib-obj>)

if(BUILD_SHARED_LIBS AND WIN32)
  target_compile_definitions(marpalua PUBLIC LUA_BUILD_AS_DLL)
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

# Preliminary versions of lua and luac (without libmarpa compiled in)
# for bootstraping
# luac uses non-exported APIs
add_executable(luac ${_LUA_COMPILER_SRC} $<TARGET_OBJECTS:lualib-obj>)
target_compile_options(luac PUBLIC -Wall -Wextra)
target_compile_definitions(luac PUBLIC LUA_COMPAT_5_3)
target_link_libraries(luac PRIVATE $<BUILD_INTERFACE:lualib-obj>)

# interpreter
add_executable(luai ${_LUA_INTERPRETER_SRC})
target_compile_options(luai PUBLIC -Wall -Wextra)
target_compile_definitions(luai PUBLIC LUA_COMPAT_5_3)
target_link_libraries(luai PRIVATE marpalua)
set_target_properties(luai PROPERTIES OUTPUT_NAME lua PDB_NAME luai)

# luac uses non-exported APIs
add_executable(marpaluac ${_LUA_COMPILER_SRC} $<TARGET_OBJECTS:lualib-obj>)
target_compile_options(marpaluac PUBLIC -Wall -Wextra)
target_compile_definitions(marpaluac PUBLIC LUA_COMPAT_5_3)
target_link_libraries(marpaluac PRIVATE $<BUILD_INTERFACE:lualib-obj>)

# interpreter
add_executable(marpaluai ${_LUA_INTERPRETER_SRC})
target_compile_options(marpaluai PUBLIC -Wall -Wextra)
target_compile_definitions(marpaluai PUBLIC LUA_COMPAT_5_3)
target_compile_definitions(marpaluai PUBLIC PROGNAME=marpalua)
target_link_libraries(marpaluai PRIVATE marpalua)

set_target_properties(marpaluai PROPERTIES OUTPUT_NAME marpalua PDB_NAME marpaluai)
if(UNIX)
  if(CMAKE_SYSTEM_NAME STREQUAL FreeBSD)
    set(_LIB_READLINE_NAME edit)
  else()
    set(_LIB_READLINE_NAME readline)
  endif()
  find_library(LIB_READLINE NAMES ${_LIB_READLINE_NAME})
  if(LIB_READLINE)
    target_compile_definitions(marpaluai PRIVATE LUA_USE_READLINE)
    target_compile_definitions(luai PRIVATE LUA_USE_READLINE)
    target_link_libraries(marpaluai PRIVATE ${LIB_READLINE})
    target_link_libraries(luai PRIVATE ${LIB_READLINE})
    if(_LIB_READLINE_NAME STREQUAL edit)
      TARGET_INCLUDE_DIRECTORIES (marpaluai PUBLIC /usr/include/edit)
      TARGET_INCLUDE_DIRECTORIES (luai PUBLIC /usr/include/edit)
    endif()
  endif()
endif()
mark_as_advanced(LIB_READLINE)
install(
  TARGETS
    marpaluai
    marpaluac
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/
  COMPONENT bin
)
