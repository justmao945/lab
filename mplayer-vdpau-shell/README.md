## Updated
Now I'm using [mplayer2][1], which works better with vdpau~~

## Drawback of mplayer with VDPAU options
Sometimes starts mplayer with VDPAU options will cause errors.
And there is no SDL window output or only has audio output...
or segmentation fault or... So I wrote a simple wrapper to
recover mplayer from these errors.


## What will MVS(mplayer-vdpau-shell) do?
1. Check configuration at `~/.mplayer/config.vdpau`
2. If not exists then create and read the configuration.
3. Try to start mplayer with vdpau support.
4. Check output of mplayer and find error words(`FATAL` etc.).
5. If found errors then try to kill mplayer and restart mplayer without vdpau.
6. Or still waiting...

## Attentions
1. If you have placed a file named `config.vdpau` in `~/.mplayer`, you'd
better remove it at the first run of MVS. Because MVS will not change 
anything if it is exist.
2. MVS only adds VDPAU options, so if your hardware doesn't support 
this feature, don't install this wrapper.

## Contact
1. [GitHub][2]
2. [Gmail][3]


[1]: http://www.mplayer2.org
[2]: https://github.com/justmao945/mplayer-vdpau-shell
[3]: justmao945@gmail.com
