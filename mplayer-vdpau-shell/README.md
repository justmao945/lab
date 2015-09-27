## Updated
Now I'm using [mplayer2][1], which works better with vdpau~~

## Drawbacks of mplayer with VDPAU
Sometimes start mplayer with VDPAU causes errors: no SDL window, only has audio output, segmentation fault, etc.
So I wrote a simple wrapper to recover mplayer from these errors.

## How does it work ?
1. Check configuration at `~/.mplayer/config.vdpau`
2. If not exist then create and read the pre-defined configuration.
3. Try to start mplayer with vdpau.
4. Check output of mplayer and find error words(`FATAL` etc.).
5. Try to kill mplayer and restart mplayer without vdpau if found errors.
6. Go to step 4.

## Attentions
1. If you have placed a file named `config.vdpau` in `~/.mplayer`,
   you'd better remove it at the first time of using MVS. Because MVS won't change anything if it is exist.
2. MVS only adds VDPAU options, so if your hardware doesn't support this feature, don't install this wrapper.

## Contact
1. [GitHub][2]
2. [Gmail][3]


[1]: http://www.mplayer2.org
[2]: https://github.com/justmao945/mplayer-vdpau-shell
[3]: justmao945@gmail.com
