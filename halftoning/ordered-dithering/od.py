#!/usr/bin/env python2
# -*- coding:utf-8 -*-
'''
Here we use the base kernel
              |  1  2  |
  I\2(i, j) = |  3  0  |

to produce larger kernels by this transformation:
               |  4 * I\n + 1    4 * I\n + 2  |
  I\2n(i, j) = |  4 * I\n + 3    4 * I\n      |

References:
<1> http://en.wikipedia.org/wiki/Ordered_dithering
<2> Purdue University: Digital Image Processing Laboratory Image Halftoning.
'''

import Image
import sys


def gen_matrix( e ):
  ''' Generating new matrix.
      @param e The width and height of the matrix is 2^e.
      @return New 2x2 to 2^e x 2^e matrix list.
  '''
  if e < 1: return None
  m_list = [ [[1,2],[3,0]] ]
  _b = m_list[0]
  for n in xrange(1, e):
    m = m_list[ n - 1 ]
    m_list.append( [
      [4*i+_b[0][0] for i in m[0]] + [4*i+_b[0][1] for i in m[0]],
      [4*i+_b[0][0] for i in m[1]] + [4*i+_b[0][1] for i in m[1]],
      [4*i+_b[1][0] for i in m[0]] + [4*i+_b[1][1] for i in m[0]],
      [4*i+_b[1][0] for i in m[1]] + [4*i+_b[1][1] for i in m[1]],
    ] )
  return m_list


def ordered_dithering( pixel, size, matrix ):
  """ Dithering on a single channel.
      @param pixel PIL PixelAccess object.
      @param size A tuple to represent the size of pixel.
      @param matrix Must be NxN, and N == 2^e where e>=1
  """
  X, Y = size
  N = len(matrix)
  
  T = [[255*(matrix[x][y]+0.5)/N/N for x in xrange(N)] for y in xrange(N)]
  for y in xrange(0, Y):
    for x in xrange(0, X):
      pixel[x,y] = 255 if pixel[x,y] > T[x%N][y%N] else 0


if __name__ == "__main__":
  if len(sys.argv) < 2:
    print "Usage: %s <image file> [e]" % sys.argv[0]
    exit(-1)

  im = Image.open(sys.argv[1]).convert('L')
  e = int(sys.argv[2])
  ordered_dithering(im.load(), im.size, gen_matrix(e)[e-1])
  im.save( "od-%s-%i.png" % (sys.argv[1], e))

