-- My revision of strict.lua
-- checks uses of undeclared global variables
-- All global variables must be 'declared' through a regular assignment
-- (even assigning nil will do) in a main chunk before being used
-- anywhere or assigned to inside a function.
-- distributed under the Lua license: http://www.lua.org/license.html

local getinfo, error, rawset, rawget = debug.getinfo, error, rawset, rawget

local mt = getmetatable(_G)
local strict_mt = {}
-- Initialize with "strict off"
-- if mt == nil then
  -- mt = {}
  -- setmetatable(_G, mt)
-- end

local declared = {
   _G = true,
   _M = true,
   strict = true,
}

local function what ()
  local d = getinfo(3, "S")
  return d and d.what or "C"
end

strict_mt.__newindex = function (t, n, v)
  if not declared[n] then
    error("assign to undeclared variable '"..n.."'", 2)
  end
  rawset(t, n, v)
end

strict_mt.__index = function (t, n)
  if not declared[n] then
    error("variable '"..n.."' is not declared", 2)
  end
  return rawget(t, n)
end

local function strict_on()
    local G_mt = getmetatable(_G)
    if G_mt == nil then
      setmetatable(_G, strict_mt)
    end
end

local function strict_off()
    local G_mt = getmetatable(_G)
    if G_mt == strict_mt then
      setmetatable(_G, nil)
    end
end

local function strict_declare(name, boolean)
    declared[name] = boolean
end

return {
    on = strict_on,
    off = strict_off,
    declare = strict_declare,
    declared = declared
}

