#!@CMAKE_RUNTIME_OUTPUT_DIRECTORY@/marpalua

require 'Test.More'
plan(2)

ok(1, "Trivial test")
is(_VERSION,"Lua 5.4","Lua version is 5.4")
