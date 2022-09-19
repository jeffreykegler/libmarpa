export LUA_PATH='../lib/?.lua;../lib_orig/?.lua'
lua gen_error_table.lua error_codes.table > error_table.md
lua gen_event_table.lua events.table > event_table.md
lua gen_step_table.lua steps.table > step_table.md
lua miranda error_table.md event_table.md step_table.md kollos.md main=kollos.lua
lua miranda error_table.md event_table.md step_table.md kollos.md kollos_c=kollos.c
lua miranda error_table.md event_table.md step_table.md kollos.md kollos_h=kollos.new.h

