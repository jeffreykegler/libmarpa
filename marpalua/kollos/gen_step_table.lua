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

require 'strict'
require 'inspect' -- delete after development

local input_file_name = arg[1]

local function c_safe_string (s)
    s = string.gsub(s, '"', '\\034')
    s = string.gsub(s, '\\', '\\092')
    s = string.gsub(s, '\n', '\\n')
    return '"' .. s .. '"'
end

if not input_file_name then
    error("usage: gen_step_table.pl input_file > output_file\n");
end

local f = assert(io.open(input_file_name, "r"))
local code_lines = {}
local code_mnemonics = {}
local max_code = 0
while true do
    local line = f:read()
    if line == nil then break end
    local i,_ = string.find(line, "#")
    local stripped
    if (i == nil) then stripped = line
    else stripped = string.sub(line, 0, i-1)
    end
    if string.find(stripped, "%S") then
	local raw_code
	local mnemonic
	local description
	_, _, raw_code, mnemonic = string.find(stripped, "^(%d+)%s(%S+)$")
	local code = tonumber(raw_code)
	if mnemonic == nil then return nil, "Bad line in step code file ", line end
	if code > max_code then max_code = code end
	code_mnemonics[code] = mnemonic
	code_lines[code] = string.format( '   { %d, %s },',
	    code,
	    c_safe_string(mnemonic)
	    )
    end
end

io.write('```\n');
io.write('  -- miranda: section define step codes\n');
io.write('  #define MARPA_MIN_STEP_CODE 0\n')
io.write('  #define MARPA_MAX_STEP_CODE ' .. max_code .. '\n\n')

for i = 0, max_code do
    local mnemonic = code_mnemonics[i]
    if mnemonic then
	io.write(
	       string.format('  #define %s %d\n', mnemonic, i)
       )
   end
end
io.write('\n')
io.write('  struct s_libmarpa_step_code marpa_step_codes[MARPA_MAX_STEP_CODE-MARPA_MIN_STEP_CODE+1] = {\n')
for i = 0, max_code do
    local code_line = code_lines[i]
    if code_line then
       io.write(code_line .. '\n')
    else
       io.write(
	   string.format(
	       '    { %d, "MARPA_STEP_RESERVED", "Unknown Libmarpa step %d" },\n',
	       i, i
	   )
       )
    end
end
io.write('  };\n');
io.write('```\n');

-- print(inspect(code_mnemonics))

local by_code = {}
local by_mnemonic = {}
for code, mnemonic in pairs(code_mnemonics) do
     by_code[#by_code+1] = string.format(
       "    _M.step[%d] = { code = %d, name=%q }\n",
	   code, code, mnemonic)
     by_mnemonic[#by_mnemonic+1] = string.format("    _M.step[%q] = %d\n",
	 tostring(mnemonic), code)
end

io.write('```\n');
io.write('    -- miranda: section define Lua step codes\n');
io.write('    _M.step = {}\n')
io.write(table.concat(by_code, ''))
io.write(table.concat(by_mnemonic, ''))
io.write('\n');
io.write('```\n');
