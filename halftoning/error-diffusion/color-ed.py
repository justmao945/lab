#!/usr/bin/env python2
# -*- coding:utf-8 -*-

'''
Here we firstly convert RGB model to CMYK model to split bands, so that 
the routine can process on every channel. Finally, merge all bands back
and convert back to RGB model. Now we get a colored image.
'''

import Image
import sys
import ed

if __name__ == "__main__":
  if len(sys.argv) != 2:
    print "Usage: %s <image file>" % sys.argv[0]
    exit(-1)

  im = Image.open(sys.argv[1]).convert('CMYK')
  im = im.split()
  for chan in im:
    ed.error_diffusion( chan.load(), chan.size )
  im = Image.merge("CMYK", im).convert("RGB")
  im.save( "color-ed-" + sys.argv[1] )

