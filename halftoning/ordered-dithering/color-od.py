#!/usr/bin/env python2
# -*- coding:utf-8 -*-
'''
Here we firstly convert RGB model to CMYK model to split bands, so that 
the routine can process on every channel. Finally, merge all bands back
and convert back to RGB model. Now we get a colored image.
'''

import Image
import sys
import od

if __name__ == "__main__":
  if len(sys.argv) < 2:
    print "Usage: %s <image file> [e]" % sys.argv[0]
    exit(-1)

  im = Image.open(sys.argv[1]).convert('CMYK').split()
  e = int(sys.argv[2])
  for ch in im:
    od.ordered_dithering(ch.load(), ch.size, od.gen_matrix(e)[e-1])
  im = Image.merge("CMYK",im).convert("RGB")
  im.save( "color-od-%s-%i.png" % (sys.argv[1], e))
