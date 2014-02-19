-- Rings and weather
-- justmao945@gmail.com
-- Changelog:
-- Ver 20111107: Original release
-- Ver 20111109: Fix bugs when no network available
-- Ver 20111111: Deprecate drawing png with cairo (without anti-alias)

require 'cairo'

Rings = {
  _Settings = 
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
      X = 35,
      Y = 430,
      Radius = 25,
      Thickness = 4,
      StartAngle = -180,
      EndAngle = 90,
    },
    { -- CPU2
      Name = 'cpu',
      Arg = 'cpu2',
      Max = 100,
      BgColor = {1, 1, 1, 0.2},
      FgColor = {0.2, 0.6, 0, 0.8},
      X = 35,
      Y = 430,
      Radius = 20,
      Thickness = 4,
      StartAngle = -180,
      EndAngle = 90,
    },
    { -- Memory
      Name = 'memperc',
      Arg = '',
      Max = 100,
      BgColor = {1, 1, 1, 0.2},
      FgColor = {0.2, 0.6, 0, 0.8},
      X = 35,
      Y = 490,
      Radius = 25,
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
      X = 35,
      Y = 490,
      Radius = 20,
      Thickness = 4,
      StartAngle = -180,
      EndAngle = 90,
    },
  },
    
  _IsInit = false,

  _DegToRad = function(self, d)
    return d * math.pi / 180 
  end,
  
  _Init = function (self)
    -- Init for _Settings
    for i, Ring in ipairs(self._Settings) do
      Ring.StartAngle = self:_DegToRad(Ring.StartAngle)
      Ring.EndAngle = self:_DegToRad(Ring.EndAngle)
      
      if Ring.Name == "time" and Ring.Arg == "%d" then -- Day reset
        local _Month = tonumber(os.date("%m"))
        if Month == 2 then  -- Feb
          local _Year = tonumber(os.date("%Y"))
          if (_Year % 4 == 0 and _Year % 100 ~= 0) or _Year % 400 == 0 then
            Ring.Max = 29
          else
            Ring.Max = 28
          end
        elseif _Month == 4 or _Month == 6 or _Month == 9 or _Month == 11 then
          Ring.Max = 30
        else
          Ring.Max = 31
        end
      end
    end
  end,
  
  Draw = function (self, Cairo)
    if not self._IsInit then  -- Not initialized
      self:_Init()
      self._IsInit = true
    end
    cairo_save(Cairo)
    
    for i, Ring in ipairs(self._Settings) do
      cairo_set_line_cap(Cairo, CAIRO_LINE_CAP_BUTT)
      cairo_set_line_width(Cairo, Ring.Thickness)
      -- Draw background ring
      cairo_arc(Cairo, Ring.X, Ring.Y,
        Ring.Radius, Ring.StartAngle, Ring.EndAngle)
      cairo_set_source_rgba(Cairo, 
          Ring.BgColor[1], Ring.BgColor[2], Ring.BgColor[3], Ring.BgColor[4])
      cairo_stroke(Cairo)

      -- Draw indicator ring
      local _Percent = nil
      if Ring.Name == "time" then
        _Percent = os.date(Ring.Arg) / Ring.Max
      else
        _Percent = (tonumber(conky_parse(string.format("${%s %s}",
            Ring.Name, Ring.Arg))) or 0) / Ring.Max
      end

      cairo_arc(Cairo, Ring.X, Ring.Y, Ring.Radius, Ring.StartAngle,
          Ring.StartAngle + _Percent * (Ring.EndAngle - Ring.StartAngle))
      cairo_set_source_rgba(Cairo,
          Ring.FgColor[1], Ring.FgColor[2], Ring.FgColor[3], Ring.FgColor[4])
      cairo_stroke(Cairo)
      
      -- Draw clock hands
      cairo_set_line_cap(Cairo, CAIRO_LINE_CAP_ROUND)
      if Ring.DrawHand == true then
        local Angle = nil
        if Ring.Name == "time" then
          Angle = os.date(Ring.Arg) / Ring.Max * 2 * math.pi - math.pi/2
        else
          Angle = (tonumber(conky_parse(string.format("${%s %s}",
              Ring.Name, Ring.Arg))) or 0) / Ring.Max * 2 * math.pi - math.pi/2
        end

        cairo_set_source_rgba(Cairo, Ring.HandColor[1],
            Ring.HandColor[2],Ring.HandColor[3],Ring.HandColor[4])
        cairo_set_line_width(Cairo, Ring.HandThickness)
        cairo_move_to(Cairo, Ring.X, Ring.Y)
        cairo_line_to(Cairo,
            Ring.X + Ring.Radius * math.cos(Angle),
            Ring.Y + Ring.Radius * math.sin(Angle))
        cairo_stroke(Cairo)
      end
    end
    cairo_restore(Cairo)
  end,
}



-- Weather for conky based on http://developer.yahoo.com/weather/
--
Weather = 
{
  _Settings = 
  {
    WOEID = '2132574',
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
        X = 65,
        Y = 115,
        Width = 70,
        Height = 70,
      },
      forecast1 = 
      { -- Another today =,=
        X = 27,
        Y = 257,
        Width = 54,
        Height = 54,
      },
      forecast2 = 
      { -- Tomorrow
        X = 127,
        Y = 257,
        Width = 54,
        Height = 54,
      },
    },
  },

  Items = {},
  _ForecastCount = 0,
  _GetXmlCount = 0,

  _XMLParse = function (self, s)
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
  
  _CastToTop = function(self, Me, Parent)
    -- Only left items's tag have a semicolon ':', and cut
    -- string before ':'
    if Parent == nil then  -- top level of Me and init tables
      self.Items = {}
      self._ForecastCount = 0
    end
    for i, v in pairs(Me) do
      if i == 'label' then
        local _Pos = string.find(v, ':')
        if _Pos ~= nil then
          v = string.sub(v, _Pos + 1)
          if v == 'forecast' then
            self._ForecastCount = self._ForecastCount + 1
            self.Items[v..tostring(self._ForecastCount)] = Me.xarg
          else
            self.Items[v] = Me.xarg
          end
        end
      end
      if type(v) == 'table' then
        self:_CastToTop(v, Me)
      end
    end
  end,

  _GetUri = function(self)
    local _s = self._Settings
    os.execute(string.format("%s -q -x -T %s -O %s \"%s%s\" &",
      _s.Wget, _s.UriTimeout, _s.Xml, _s.Uri, _s.WOEID) )
  end,

  _GetXml = function(self)
    local _File = io.open(self._Settings.Xml, "r")
    local _Xml = nil
    if _File == nil then
      self._GetXmlCount = 0 -- reset
      return
    else
      _Xml = _File:read("*all") or ''
      if #_Xml == 0 then --reset
        self._GetXmlCount = 0
        io.close(_File)
        return
      end
    end
    self._GetXmlCount = self._GetXmlCount + 1
    self:_CastToTop(self:_XMLParse(_Xml))
    -- cast all items to the top of table
    io.close(_File)
  end,

  Refresh = function(self)
    local _Update = tonumber(conky_parse('${updates}')) or 0
    local _S = self._Settings
    if (self._GetXmlCount == 0 and _Update % _S.UriTimeout == 1)
          or _Update % _S.UpdateUriRatio == 1 then
      self:_GetUri()
    end
    if self._GetXmlCount == 0 or _Update % _S.UpdateXmlRatio == 1  then
      self:_GetXml()
    end
  end,
  
  IsInit = function(self)
    return self._GetXmlCount ~= 0
  end,

  Test = function(self)
    Weather:_GetUri()
    os.execute("sleep 5")
    Weather:_GetXml()
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

  local _CairoSurface = cairo_xlib_surface_create(
      conky_window.display,
      conky_window.drawable,
      conky_window.visual,
      conky_window.width,
      conky_window.height)

  local _Cairo = cairo_create(_CairoSurface)
  cairo_surface_destroy(_CairoSurface)

  Rings:Draw(_Cairo)
  
  cairo_destroy(_Cairo)
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
  local _Item = Weather.Items[Day]
  if  Weather:IsInit() and _Item then
    local _Img = Weather._Settings.Images
    local _ImgPath =string.format("%s/%s.png",_Img.Dir,_Item.code)
    _Img = _Img[Day]
    if _Img then
      return string.format("${image %s -p %d,%d -s %dx%d}",
          _ImgPath, _Img.X, _Img.Y, _Img.Width, _Img.Height)
    end
  end
  return ''
end
