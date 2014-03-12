

local io = require("io")
local naughty = require("naughty")

local pacmd = {}

--
-- Result format 
-- {
--   alsa_output.pci-0000_00_1b.0.analog-stereo :{
--     volume: "0x5ad5",
--     mute: "no"
--   },
--   ... 
-- }
--
pacmd.GetVolume = function(self)
  local fd = io.popen("pacmd dump")
  local dump = fd:read("*all")
  fd:close()

  local info, src, key, val
  local volumes = {}
  for line in dump:gmatch("[^\n]+") do
    _,_, key, src, val = line:find("^set%-sink%-(%w+) ([_%.%-%d%w]+) ([%d%w]+)$")
    if key and src and val then
      info = volumes[src]
      if info == nil then
        info = {}
        volumes[src] = info
      end
      info[key] = val
    end
  end
  return volumes
end

--
-- Set volume
-- @relative, [-0x10000, 0x10000], e.g 655 or -655
--
--
pacmd.SetVolume = function(self, relative)
  local vols = self:GetVolume(), cmd, vol
  for k,v in pairs(vols) do
    vol = tonumber(v.volume) + relative
    if vol > 0x10000 then
      vol = 0x10000
    elseif vol < 0 then
      vol = 0
    end
    io.popen("pacmd set-sink-volume ".. k .. " " .. vol):close()
  end
end

--
-- Toggle mute
pacmd.Mute = function(self)
  local vols = self:GetVolume()
  local mute = " yes"
  for k,v in pairs(vols) do
    if v["mute"] == "yes"  then
      mute = " no"
    end
    io.popen("pacmd set-sink-mute "..k..mute):close()
  end
end


-- Show results on notification by calling func.
-- @... Args passed to func.
pacmd.Notify = function(self, func, ...)
  func(self, ...)
  local text = ""
  for k, v in pairs( self:GetVolume() ) do
    text = text .. string.format("<span> Volume:[%.0f] Mute:[%s] </span>\n",
        tonumber(v.volume)/0x10000*100, v.mute)
  end
  self.nofityId = naughty.notify({
    title = "PulseAudio",
    text = text,
    opacity = 0.8,
    replaces_id = self.nofityId,
  }).id
end

return pacmd

