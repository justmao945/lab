#ifndef CONFIG_H
#define CONFIG_H

#if defined(__MACOSX__) || defined(__APPLE__)
// Disable display features, default OSX 10.9 has no X11 installed.
# define cimg_display 0
#endif

// Enable basic file types.
#define cimg_use_png    // libpng


#endif // end of CONFIG_H
