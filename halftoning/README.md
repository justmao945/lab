## Files 
  1. ./1.jpg, ./2.jpg: the original images.
  2. ./error-diffusion directory: Error Diffusion halftone stuffs.
          -->1.jpg 2.jpg :  original imgs
          -->color-ed.py : python source file, color mode.
          -->ed.py : python souce file, gray mode.
          -->ed-1.png, ed-2.png: gray output files.
          -->color-ed-1.jpg color-ed-2.jpg: 3-bit color output.

  3. ./ordered-dithering directory: Ordered Dithering halftone stuffs.
          -->1.jpg :  original img
          -->color-od.py : python source file, color mode.
          -->od.py : python souce file, gray mode.
          -->od-1.jpg-x.png: gray output files, with the input e==x.
          -->color-ed-1.jpg-x.png : 3-bit color output, with the input e==x.

## Runtime Dependencies
  1. python 2.7
  2. python-imaging 1.1.7 ( for easier image manipulations )


## Run HOWTO
  1. Run error diffusion
      `cd error-diffusion`
      `python2 ed.py 1.jpg`
      `python2 color-ed.py 2.jpg`

  2. Run ordered dithering
      `cd ordered-dithering`
      `python2 od.py 1.jpg 1`
      `python2 od.py 1.jpg 2`
      `python2 od.py 1.jpg 3`
      `python2 color-od.py 1.jpg 1`
      `python2 color-od.py 1.jpg 2`
      `python2 color-od.py 1.jpg 3`
    Where the last argument is e to generate a matrix. See od.py for more
    information.


## References
  1. http://en.wikipedia.org/wiki/Ordered_dithering
  2. http://en.wikipedia.org/wiki/Error_diffusion
  3. Purdue University: Digital Image Processing Laboratory Image Halftoning

