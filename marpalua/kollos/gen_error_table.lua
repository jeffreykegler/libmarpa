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

inspect = require 'inspect' -- delete after development?

local input_file_name = arg[1]

local function c_safe_string (s)
    s = string.gsub(s, '"', '\\034')
    s = string.gsub(s, '\\', '\\092')
    s = string.gsub(s, '\n', '\\n')
    return '"' .. s .. '"'
end

if not input_file_name then
    error("usage: gen_error_table.pl input_file > output_file\n");
end

local lua_code_data = {}

do
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
	  local raw_mnemonic
	  local description
	  _, _, raw_code, raw_mnemonic, description = string.find(stripped, "^(%d+)%sMARPA_ERR_(%S+)%s(.*)$")
	  local code = tonumber(raw_code)
	  if description == nil then return nil, "Bad line in error code file ", line end
	  if code > max_code then max_code = code end
	  local mnemonic = 'KOLLOS_ERR_' .. raw_mnemonic
	  code_mnemonics[code] = mnemonic
	  code_lines[code] = string.format( '   { %d, %s, %s },',
	      code,
	      c_safe_string(mnemonic),
	      c_safe_string(description)
	      )
	  lua_code_data[code] = { code, raw_mnemonic, description }
	  lua_code_data[raw_mnemonic] = lua_code_data[code]
      end
  end

  io.write('```\n');
  io.write('  -- miranda: section define error codes\n');
  io.write('  #define LIBMARPA_MIN_ERROR_CODE 0\n')
  io.write('  #define LIBMARPA_MAX_ERROR_CODE ' .. max_code .. '\n\n')

  for i = 0, max_code do
      local mnemonic = code_mnemonics[i]
      if mnemonic then
	  io.write(
		 string.format('  #define %s %d\n', mnemonic, i)
	 )
     end
  end
  io.write('\n')
  io.write('  struct s_libmarpa_error_code marpa_error_codes[LIBMARPA_MAX_ERROR_CODE-LIBMARPA_MIN_ERROR_CODE+1] = {\n')
  for i = 0, max_code do
      local code_line = code_lines[i]
      if code_line then
	 io.write(code_line .. '\n')
      else
	 io.write(
	     string.format(
		 '    { %d, "KOLLOS_ERROR_RESERVED", "Unknown Libmarpa error %d" },\n',
		 i, i
	     )
	 )
      end
  end
  io.write('  };\n\n');
  f:close()
end

-- for Kollos's own (that is, non-Libmarpa) error codes
do
  local code_lines = {}
  local code_mnemonics = {}
  local min_code = 200
  local max_code = 200

  -- Should add some checks on the errors, checking for
  -- 1.) duplicate mnenomics
  -- 2.) duplicate error codes

  local function luif_error_add (code, mnemonic, description)
      code_mnemonics[code] = mnemonic
      code_lines[code] = string.format( '   { %d, %s, %s },',
	  code,
	  c_safe_string(mnemonic),
	  c_safe_string(description)
	  )
      lua_code_data[code] = { code, mnemonic, description }
      lua_code_data[mnemonic] = lua_code_data[code]
      if code > max_code then max_code = code end
  end

  -- KOLLOS_ERR_RESERVED_200 is a place-holder , not expected to be actually used
  luif_error_add( 200, "ERR_RESERVED_200", "Unexpected Kollos error: 200")
  luif_error_add( 201, "ERR_LUA_VERSION", "Bad Lua version")
  luif_error_add( 202, "ERR_LIBMARPA_HEADER_VERSION_MISMATCH", "Libmarpa header does not match expected version")
  luif_error_add( 203, "ERR_LIBMARPA_LIBRARY_VERSION_MISMATCH", "Libmarpa library does not match expected version")

  io.write('  #define KOLLOS_MIN_ERROR_CODE ' .. min_code .. '\n')
  io.write('  #define KOLLOS_MAX_ERROR_CODE ' .. max_code .. '\n\n')
  for i = min_code, max_code
  do
      local mnemonic = code_mnemonics[i]
      if mnemonic then
	  io.write(
		 string.format('  #define KOLLOS_%s %d\n', mnemonic, i)
	 )
     end
  end

  io.write('\n')
  io.write('  struct s_libmarpa_error_code marpa_kollos_error_codes[(KOLLOS_MAX_ERROR_CODE-KOLLOS_MIN_ERROR_CODE)+1] = {\n')
  for i = min_code, max_code do
      local code_line = code_lines[i]
      if code_line then
	 io.write(code_line .. '\n')
      else
	 io.write(
	     string.format(
		 '    { %d, "KOLLOS_ERROR_RESERVED", "Unknown Kollos error %d" },\n',
		 i, i
	     )
	 )
      end
  end
  io.write('  };\n\n');
  io.write('```\n');
end

-- print(inspect(lua_code_data))

local by_code = {}
local by_mnemonic = {}
for index, element in pairs(lua_code_data) do
     if type(index) == "number" then
         by_code[#by_code+1] = string.format(
	 "    _M.err[%d] = { code = %d, name=%q,\n        description=%q}\n",
	     index, element[1], element[2], element[3])
	 goto NEXT_INDEX
     end
     by_mnemonic[#by_mnemonic+1] = string.format("    _M.err[%q] = %d\n",
	 tostring(index), element[1])
     ::NEXT_INDEX::
end

io.write('```\n');
io.write('    -- miranda: section define Lua error codes\n');
io.write('    _M.err = {}\n')
io.write(table.concat(by_code, ''))
io.write(table.concat(by_mnemonic, ''))
io.write('\n');
io.write('```\n');
