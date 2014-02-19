Install conky-lua
==================
1. Get conky tar from http://downloads.sourceforge.net/project/conky/conky/1.9.0/conky-1.9.0.tar.gz
2. Install dependences...
3. Configure it, 
  ```sh
  CPPFLAGS="${CXXFLAGS}" LIBS="${LDFLAGS}" LUA_LIBS="-llua5.1" LUA_CFLAGS="-I/usr/include/lua5.1" \
      ./configure \
      --prefix=/usr \
      --sysconfdir=/etc \
      --enable-ibm \
      --enable-curl \
      --enable-rss \
      --enable-weather-xoap \
      --enable-imlib2 \
      --enable-wlan \
      --enable-lua \
      --enable-lua-cairo \
      --enable-lua-imlib2
  ```
      
4. Install it, `sudo make && make install`

Screenshot
==================
![C source](http://justmao945.github.io/static/conky-lua-dock/screenshot.png)


