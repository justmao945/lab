local application = require 'mjolnir.application'
local hotkey = require 'mjolnir.hotkey'
local window = require 'mjolnir.window'
local fnutils = require 'mjolnir.fnutils'

local modkey = {"cmd", "ctrl"}

hotkey.bind(modkey, "H", function()
  local w = window.focusedwindow()
  if w then w:movetounit({x=0, y=0, w=0.5, h=1}) end
end)

hotkey.bind(modkey, "L", function()
  local w = window.focusedwindow()
  if w then w:movetounit({x=0.5, y=0, w=0.5, h=1}) end
end)

-- Maximize the window
hotkey.bind(modkey, "M", function()
  local w = window.focusedwindow()
  if w then w:maximize() end
end)
