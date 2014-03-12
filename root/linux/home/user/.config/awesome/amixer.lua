--[[
--  AMixer is forked from https://github.com/gorlowski/couth.
--
--  This only works with ALSA (not OSS).
--  You must have amixer(part of alsa-utils) on your path for this to work.
--  Author <justmao945@gmail.com>
--
--  Use: put amixer.lua into ~/.config/awesome/ and add following code to rc.lua
--    local amixer = require("amixer")
--
--    awful.key({}, "XF86AudioMute",
--              function() amixer:Notify(amixer.SetVolume, "Master", "toggle") end),
--    awful.key({}, "XF86AudioLowerVolume",
--              function() amixer:Notify(amixer.SetVolume, "Master", "10%-") end),
--    awful.key({}, "XF86AudioRaiseVolume",
--              function() amixer:Notify(amixer.SetVolume, "Master", "10%+") end),
--]]

local io      = require("io")
local naughty = require("naughty")

-- Public interface.
local amixer = {}

-- Get volume.
-- The result looks like `{ {chan:"Font Left", vol:"29", sw:"on"}, ... }`.
-- @scntl <SCONTROL> of amixer, run `amixer scontrols` to list.
-- @return Volumes for every channel.
amixer.GetVolume = function(self, scntl)
  scntl = scntl or "Master"
  local fd = io.popen("amixer sget " .. scntl)
  local sinfo = fd:read("*all")
  fd:close()
  
  local volumes = {}, chan, vol, sw
  for line in sinfo:gmatch("[^\n]+") do
    _,_,chan, vol, sw = line:find("(.*): Playback %d+ %[(%d+)%%%].*%[(o[nf]+)%]")
    if chan and vol and sw then
      table.insert(volumes, {chan= chan, vol= vol, sw= sw})
    end
  end
  return volumes
end


-- Set volume.
-- @scntl <SCONTROL> of amixer, run `amixer scontrols` to list.
-- @param see man amixer. (e.g. 10%[+-], toggle etc.).
-- @return Volumes for every channel.
amixer.SetVolume = function(self, scntl, param)
  scntl = scntl or "Master"
  param = param or "toggle"
  io.popen("amixer -q sset " .. scntl .. ' ' .. param):close()
  return self:GetVolume(scntl)
end


-- Show results on notification by calling func.
-- @func Function to call, must return GetVolume(...)
-- @... Args passed to func.
amixer.Notify = function(self, func, ...)
  local volumes = func(self, ...)
  local text = ""
  for i, v in ipairs( volumes ) do
    text = text .. string.format("<span>%s:\t[%s] [%s] </span>\n", v.chan, v.vol, v.sw)
  end
  self.nofityId = naughty.notify({
    title = "Volume",
    text = text,
    opacity = 0.8,
    replaces_id = self.nofityId,
  }).id
end

return amixer

