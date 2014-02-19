#!/usr/bin/env python2
# -*- coding:utf-8 -*-

'''
Here We use the Floyd Steinberg transforming matrix: 
              |    *  7 |
        1/16  |         |
              | 3  5  1 |
to produce the error diffusion. Where the star (*) indicates current pixel
and other elements represent the weight if pixels. See error_diffusion
function for more infomation.

References:
<1> http://en.wikipedia.org/wiki/Error_diffusion
<2> Purdue University:Digital Image Processing Laboratory Image Halftoning.

'''


import Image
import sys


def error_diffusion( pixel, size=(1,1) ):
  """ Diffuse on a single channel, using Floyd-Steinberg kerenl.
      @param pixel PIL PixelAccess object.
      @param size A tuple to represent the size of pixel.
  """

  for y in xrange(0, size[1]-1):
    for x in xrange(1, size[0]-1):
      oldpixel = pixel[x, y]
      pixel[x, y] = 255 if oldpixel > 127 else 0
      quant_error = oldpixel - pixel[x, y]
      pixel[x+1, y  ] = pixel[x+1, y  ] + 7/16.0 * quant_error
      pixel[x-1, y+1] = pixel[x-1, y+1] + 3/16.0 * quant_error
      pixel[x,   y+1] = pixel[x,   y+1] + 5/16.0 * quant_error
      pixel[x+1, y+1] = pixel[x+1, y+1] + 1/16.0 * quant_error


if __name__ == "__main__":
  if len(sys.argv) != 2:
    print "Usage: %s <image file>" % sys.argv[0]
    exit(-1)

  im = Image.open(sys.argv[1]).convert('L')
  error_diffusion( im.load(), im.size )
  #im.show()
  im.save( "ed-%s.png" % sys.argv[1].split('.')[0] )

