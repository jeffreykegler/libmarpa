-- Copyright 2018 Jeffrey Kegler
-- Permission is hereby granted, free of charge, to any person obtaining a
-- copy of this software and associated documentation files (the "Software"),
-- to deal in the Software without restriction, including without limitation
-- the rights to use, copy, modify, merge, publish, distribute, sublicense,
-- and/or sell copies of the Software, and to permit persons to whom the
-- Software is furnished to do so, subject to the following conditions:
--
-- The above copyright notice and this permission notice shall be included
-- in all copies or substantial portions of the Software.
--
-- THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
-- IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
-- FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
-- THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
-- OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
-- ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
-- OTHER DEALINGS IN THE SOFTWARE.
--
-- [ MIT license: http://www.opensource.org/licenses/mit-license.php ]

--[[
  Convert a file (interpreted as a string) to a .h file
  with that string as a string constant
]]

require 'strict'
require 'inspect'

local prog_name = "hex2h.lua"

if #arg < 1 or #arg > 3 then
    error("Bad # of args" .. #arg .. "\nusage: hex.lua name")
end

local string_name = arg[1]
if not string_name then
    error("Null chunk name\nusage: hex.lua name")
end
if string_name:find("[^%a%d_-]") then
    error("Bad characters in chunk name\nusage: hex.lua name")
end

local input_fh = io.stdin
local input_name = arg[2] or "-"
if input_name ~= "-" then
    local fh, msg = io.open(input_name)
    if not fh then
        error("Cannot open " .. input_name .. " in " .. prog_name .. ": " .. msg)
    end
    input_fh = fh
end
   
local output_fh = io.stdout
local output_name = arg[3] or "-"
if output_name ~= "-" then
    local fh, msg = io.open(output_name, "w")
    if not fh then
        error("Cannot open " .. output_name .. " in " .. prog_name .. ": " .. msg)
    end
    output_fh = fh
end
   

local header = [[
/*
** Permission is hereby granted, free of charge, to any person obtaining
** a copy of this software and associated documentation files (the
** "Software"), to deal in the Software without restriction, including
** without limitation the rights to use, copy, modify, merge, publish,
** distribute, sublicense, and/or sell copies of the Software, and to
** permit persons to whom the Software is furnished to do so, subject to
** the following conditions:
**
** The above copyright notice and this permission notice shall be
** included in all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
** CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
** TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
** SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**
** [ MIT license: http://www.opensource.org/licenses/mit-license.php ]
*/

/* EDITS IN THIS FILE WILL BE LOST
 * This file is auto-generated.
 */

#include "lua.h"
#include "kollos.h"

]]

function write_hex_line(piece)
    local line_out = piece:gsub('.',
        function (str)
            return string.format("\\x%02x", str:byte())
        end
    )
    output_fh:write(string.format('  "%s"\n', line_out))
end

output_fh:write(header)
output_fh:write(string.format("static const char loader[] =\n", string_name))
-- write_quoted_line(string.format("-- %q loaded by string2h\n", string_name))
local loader_length = 0
while true do
    local piece = input_fh:read(16)
    if not piece then break end
    loader_length = loader_length + #piece
    write_hex_line(piece)
end
output_fh:write("  ;\n")
output_fh:write(string.format(
    "  static const size_t loader_length = %d;\n", loader_length))
output_fh:write(string.format(
    [[  const struct kollos_chunk_data kollos_chunk_%s = {loader, loader_length, "%s"};]],
        string_name, string_name))
output_fh:write("\n")

-- vim: expandtab shiftwidth=4:
