-- Rings and weather
-- justmao945@gmail.com
-- Changelog:
-- Ver 20111107: Original release
-- Ver 20111109: Fix bugs when no network available
-- Ver 20111111: Deprecate drawing png with cairo (without anti-alias)
--

require "cairo"

Rings = {
  Settings = 
  {
    { -- Hour
      Name = 'time',
      Arg = '%I.%M',
      Max = 12,
      BgColor = {1, 1, 1, 0.1},  -- Red, green, blue and alpha sacled into 0~1
      FgColor = {0.2, 0.6, 0, 0.2},
      X = 100,
      Y = 150,
      Radius = 50,
      Thickness = 5,
      StartAngle = -90,
      EndAngle = 270,
      DrawHand = true,  -- set true to draw hand
      HandColor = {1,1,1,0.8},  -- Hour hand color
      HandThickness = 4,  -- Hour hand thickness
    },
    { -- Minute
      Name = 'time',
      Arg = '%M.%S',
      Max = 60,
      BgColor = {1, 1, 1, 0.1},
      FgColor = {0.2, 0.6, 0, 0.4},
      X = 100,
      Y = 150,
      Radius = 56,
      Thickness = 5,
      StartAngle = -90,
      EndAngle = 270,
      DrawHand = true,
      HandColor = {1,1,1,0.9},
      HandThickness = 3,
    },
    { -- Second
      Name = 'time',
      Arg = '%S',
      Max = 60,
      BgColor = {1, 1, 1, 0.1},
      FgColor = {0.2, 0.6, 0, 0.6},
      X = 100,
      Y = 150,
      Radius = 62,
      Thickness = 5,
      StartAngle = -90,
      EndAngle = 270,
      DrawHand = true,
      HandColor = {1,1,1,1},
      HandThickness = 2,
    },
    { -- Day
      Name = 'time',
      Arg = '%d',
      Max = 31,  -- here may be not good enough
      BgColor = {1, 1, 1, 0.1},
      FgColor = {0.2, 0.6, 0, 0.8},
      X = 100,
      Y = 150,
      Radius = 68,
      Thickness = 5,
      StartAngle = -180,
      EndAngle = 0,
    },
    { -- Month
      Name = 'time',
      Arg = '%m',
      Max = 12,
      BgColor = {1, 1, 1, 0.1},
      FgColor = {0.2, 0.6, 0, 1},
      X = 100,
      Y = 150,
      Radius = 74,
      Thickness = 5,
      StartAngle = -180,
      EndAngle = 0,
    }, 
    { -- CPU1
      Name = 'cpu',
      Arg = 'cpu1',
      Max = 100,
      BgColor = {1, 1, 1, 0.2},
      FgColor = {0.2, 0.6, 0, 0.8},
      X = 45,
      Y = 570,
      Radius = 35,
      Thickness = 2,
      StartAngle = -180,
      EndAngle = 90,
    },
    { -- CPU2
      Name = 'cpu',
      Arg = 'cpu2',
      Max = 100,
      BgColor = {1, 1, 1, 0.2},
      FgColor = {0.2, 0.6, 0, 0.8},
      X = 45,
      Y = 570,
      Radius = 32,
      Thickness = 2,
      StartAngle = -180,
      EndAngle = 90,
    },
    { -- CPU3
      Name = 'cpu',
      Arg = 'cpu3',
      Max = 100,
      BgColor = {1, 1, 1, 0.2},
      FgColor = {0.2, 0.6, 0, 0.8},
      X = 45,
      Y = 570,
      Radius = 29,
      Thickness = 2,
      StartAngle = -180,
      EndAngle = 90,
    },
    { -- CPU4
      Name = 'cpu',
      Arg = 'cpu4',
      Max = 100,
      BgColor = {1, 1, 1, 0.2},
      FgColor = {0.2, 0.6, 0, 0.8},
      X = 45,
      Y = 570,
      Radius = 26,
      Thickness = 2,
      StartAngle = -180,
      EndAngle = 90,
    },
    { -- CPU5
      Name = 'cpu',
      Arg = 'cpu5',
      Max = 100,
      BgColor = {1, 1, 1, 0.2},
      FgColor = {0.2, 0.6, 0, 0.8},
      X = 45,
      Y = 570,
      Radius = 23,
      Thickness = 2,
      StartAngle = -180,
      EndAngle = 90,
    },
    { -- CPU6
      Name = 'cpu',
      Arg = 'cpu6',
      Max = 100,
      BgColor = {1, 1, 1, 0.2},
      FgColor = {0.2, 0.6, 0, 0.8},
      X = 45,
      Y = 570,
      Radius = 20,
      Thickness = 2,
      StartAngle = -180,
      EndAngle = 90,
    },
    { -- CPU7
      Name = 'cpu',
      Arg = 'cpu7',
      Max = 100,
      BgColor = {1, 1, 1, 0.2},
      FgColor = {0.2, 0.6, 0, 0.8},
      X = 45,
      Y = 570,
      Radius = 17,
      Thickness = 2,
      StartAngle = -180,
      EndAngle = 90,
    },
    { -- CPU8
      Name = 'cpu',
      Arg = 'cpu8',
      Max = 100,
      BgColor = {1, 1, 1, 0.2},
      FgColor = {0.2, 0.6, 0, 0.8},
      X = 45,
      Y = 570,
      Radius = 14,
      Thickness = 2,
      StartAngle = -180,
      EndAngle = 90,
    },
    { -- Memory
      Name = 'memperc',
      Arg = '',
      Max = 100,
      BgColor = {1, 1, 1, 0.2},
      FgColor = {0.2, 0.6, 0, 0.8},
      X = 125,
      Y = 570,
      Radius = 33,
      Thickness = 4,
      StartAngle = -180,
      EndAngle = 90,
    },
    { -- Swap 
      Name = 'swapperc',
      Arg = '',
      Max = 100,
      BgColor = {1, 1, 1, 0.2},
      FgColor = {0.2, 0.6, 0, 0.8},
      X = 125,
      Y = 570,
      Radius = 28,
      Thickness = 4,
      StartAngle = -180,
      EndAngle = 90,
    },
  },
    
  isInit = false,

  degToRad = function(self, d)
    return d * math.pi / 180 
  end,
  
  init = function (self)
    -- Init for Settings
    for i, Ring in ipairs(self.Settings) do
      Ring.StartAngle = self:degToRad(Ring.StartAngle)
      Ring.EndAngle = self:degToRad(Ring.EndAngle)
      
      if Ring.Name == "time" and Ring.Arg == "%d" then -- Day reset
        local month = tonumber(os.date("%m"))
        if Month == 2 then  -- Feb
          local year = tonumber(os.date("%Y"))
          if (year % 4 == 0 and year % 100 ~= 0) or year % 400 == 0 then
            Ring.Max = 29
          else
            Ring.Max = 28
          end
        elseif month == 4 or month == 6 or month == 9 or month == 11 then
          Ring.Max = 30
        else
          Ring.Max = 31
        end
      end
    end
  end,
  
  Draw = function (self, ctx)
    if not self.isInit then  -- Not initialized
      self:init()
      self.isInit = true
    end
    cairo_save(ctx)
    
    for i, Ring in ipairs(self.Settings) do
      cairo_set_line_cap(ctx, CAIRO_LINE_CAP_BUTT)
      cairo_set_line_width(ctx, Ring.Thickness)
      -- Draw background ring
      cairo_arc(ctx, Ring.X, Ring.Y,
        Ring.Radius, Ring.StartAngle, Ring.EndAngle)
      cairo_set_source_rgba(ctx, 
          Ring.BgColor[1], Ring.BgColor[2], Ring.BgColor[3], Ring.BgColor[4])
      cairo_stroke(ctx)

      -- Draw indicator ring
      local percent = nil
      if Ring.Name == "time" then
        percent = os.date(Ring.Arg) / Ring.Max
      else
        percent = (tonumber(conky_parse(string.format("${%s %s}",
            Ring.Name, Ring.Arg))) or 0) / Ring.Max
      end

      cairo_arc(ctx, Ring.X, Ring.Y, Ring.Radius, Ring.StartAngle,
          Ring.StartAngle + percent * (Ring.EndAngle - Ring.StartAngle))
      cairo_set_source_rgba(ctx,
          Ring.FgColor[1], Ring.FgColor[2], Ring.FgColor[3], Ring.FgColor[4])
      cairo_stroke(ctx)
      
      -- Draw clock hands
      cairo_set_line_cap(ctx, CAIRO_LINE_CAP_ROUND)
      if Ring.DrawHand == true then
        local Angle = nil
        if Ring.Name == "time" then
          Angle = os.date(Ring.Arg) / Ring.Max * 2 * math.pi - math.pi/2
        else
          Angle = (tonumber(conky_parse(string.format("${%s %s}",
              Ring.Name, Ring.Arg))) or 0) / Ring.Max * 2 * math.pi - math.pi/2
        end

        cairo_set_source_rgba(ctx, Ring.HandColor[1],
            Ring.HandColor[2],Ring.HandColor[3],Ring.HandColor[4])
        cairo_set_line_width(ctx, Ring.HandThickness)
        cairo_move_to(ctx, Ring.X, Ring.Y)
        cairo_line_to(ctx,
            Ring.X + Ring.Radius * math.cos(Angle),
            Ring.Y + Ring.Radius * math.sin(Angle))
        cairo_stroke(ctx)
      end
    end
    cairo_restore(ctx)
  end,
}



-- Weather for conky based on http://developer.yahoo.com/weather/
--
Weather = 
{
  Settings = 
  {
    WOEID = '12712468',
    -- To get the WOEID please visit: http://weather.yahoo.com
    -- And then type your city in the area "Enter city or zip code"
    -- Then can find your WOEID in the web browser address bar
    -- For mine: http://weather.yahoo.com/china/zhejiang/hangzhou-2132574/
    -- So the last seven digit '2132574' is the WOEID
    --
    Wget = "wget",
    Xml = "/tmp/conky-weather.xml",
    Uri = "http://weather.yahooapis.com/forecastrss?u=c&w=",
    
    UpdateUriRatio = 3600,  -- based on conky update rate
    UpdateXmlRatio = 1800,
    UriTimeout = 5,
    -- retry to get uri in UirTimeout seconds if last got is failed
    Images = -- settings for three days's image
    {
      Dir = os.getenv("HOME").."/.conky/img",
      -- the directory to store weather images
      condition = 
      { -- Today
        X = 200,
        Y = 80,
        Width = 140,
        Height = 140,
      },
      forecast2 = 
      { -- Tomorrow
        X = 0,
        Y = 300,
        Width = 100,
        Height = 100,
      },
      forecast3 = 
      { -- the day after tomorrow
        X = 120,
        Y = 300,
        Width = 100,
        Height = 100,
      },
      forecast4 = 
      { -- 
        X = 240,
        Y = 300,
        Width = 100,
        Height = 100,
      },
    },
  },

  Items = {},
  forecastCount = 0,
  getXmlCount = 0,

  parseXML = function (self, s)
    function parseargs(s)
      local arg = {}
      string.gsub(s, "(%w+)=([\"'])(.-)%2", function (w, _, a)
        arg[w] = a
      end)
      return arg
    end

    local stack = {}
    local top = {}
    table.insert(stack, top)
    local ni,c,label,xarg, empty
    local i, j = 1, 1
    while true do
      ni,j,c,label,xarg, empty = string.find(s, "<(%/?)([%w:]+)(.-)(%/?)>", i)
      if not ni then break end
      local text = string.sub(s, i, ni-1)
      if not string.find(text, "^%s*$") then
        table.insert(top, text)
      end
      if empty == "/" then  -- empty element tag
        table.insert(top, {label=label, xarg=parseargs(xarg), empty=1})
      elseif c == "" then   -- start tag
        top = {label=label, xarg=parseargs(xarg)}
        table.insert(stack, top)   -- new level
      else  -- end tag
        local toclose = table.remove(stack)  -- remove top
        top = stack[#stack]
        if #stack < 1 then
          error("nothing to close with "..label)
        end
        if toclose.label ~= label then
          error("trying to close "..toclose.label.." with "..label)
        end
        table.insert(top, toclose)
      end
      i = j+1
    end
    local text = string.sub(s, i)
    if not string.find(text, "^%s*$") then
      table.insert(stack[#stack], text)
    end
    if #stack > 1 then
      error("unclosed "..stack[stack.n].label)
    end
    return stack[1]
  end,
  
  castToTop = function(self, Me, Parent)
    -- Only left items's tag have a semicolon ':', and cut
    -- string before ':'
    if Parent == nil then  -- top level of Me and init tables
      self.Items = {}
      self.forecastCount = 0
    end
    for i, v in pairs(Me) do
      if i == 'label' then
        local pos = string.find(v, ':')
        if pos ~= nil then
          v = string.sub(v, pos + 1)
          if v == 'forecast' then
            self.forecastCount = self.forecastCount + 1
            self.Items[v..tostring(self.forecastCount)] = Me.xarg
          else
            self.Items[v] = Me.xarg
          end
        end
      end
      if type(v) == 'table' then
        self:castToTop(v, Me)
      end
    end
  end,

  getUri = function(self)
    local s = self.Settings
    os.execute(string.format("%s -q -x -T %s -O %s \"%s%s\" &",
      s.Wget, s.UriTimeout, s.Xml, s.Uri, s.WOEID) )
  end,

  getXml = function(self)
    local file = io.open(self.Settings.Xml, "r")
    local xml = nil
    if file == nil then
      self.getXmlCount = 0 -- reset
      return
    else
      xml = file:read("*all") or ''
      if #xml == 0 then --reset
        self.getXmlCount = 0
        io.close(file)
        return
      end
    end
    self.getXmlCount = self.getXmlCount + 1
    self:castToTop(self:parseXML(xml))
    -- cast all items to the top of table
    io.close(file)
  end,

  Refresh = function(self)
    local update = tonumber(conky_parse('${updates}')) or 0
    local s = self.Settings
    if (self.getXmlCount == 0 and update % s.UriTimeout == 1)
          or update % s.UpdateUriRatio == 1 then
      self:getUri()
    end
    if self.getXmlCount == 0 or update % s.UpdateXmlRatio == 1  then
      self:getXml()
    end
  end,
  
  IsInit = function(self)
    return self.getXmlCount ~= 0
  end,

  Test = function(self)
    Weather:getUri()
    os.execute("sleep 5")
    Weather:getXml()
    for i,v in pairs(self.Items) do
      print(i,v)
      for i1, v1 in pairs(v) do
        print('|-- '..i1, v1)
      end
      print('|')
    end
  end,
}

conky_DrawHookPre = function()
  -- put this routine in conkyrc: lua_draw_hook_pre DrawHookPre
  if conky_window == nil then
    return
  end

  local surface = cairo_xlib_surface_create(
      conky_window.display,
      conky_window.drawable,
      conky_window.visual,
      conky_window.width,
      conky_window.height)

  local ctx = cairo_create(surface)
  Rings:Draw(ctx)
  cairo_destroy(ctx)
  cairo_surface_destroy(surface)
end

conky_WeatherRefresh = function()
  Weather:Refresh();
  return '' 
end

conky_WeatherGet = function(ItemKey, SubKey)
  -- E.g. WeatherGet('location', 'city')
  -- Detailed info can see the output of Weather:Test()
  return Weather:IsInit() and Weather.Items[ItemKey][SubKey] or ''
end

conky_WeatherImg = function(Day)
  -- Day can be condition, forecast1 or forecast2
  local item = Weather.Items[Day]
  if  Weather:IsInit() and item then
    local img = Weather.Settings.Images
    local imgPath =string.format("%s/%s.png",img.Dir,item.code)
    img = img[Day]
    if img then
      return string.format("${image %s -p %d,%d -s %dx%d}",
          imgPath, img.X, img.Y, img.Width, img.Height)
    end
  end
  return ''
end

