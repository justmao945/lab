
#ifndef TYPES_H
#define TYPES_H

#include <cassert>
#include <cmath>
#include <string>

namespace ssp {

struct Point {
  Point(): x(0), y(0) {}
  Point(float x_, float y_): x(x_), y(y_) { }
  void zero() { x = y = 0; }
  float x, y;
};

struct Size{
  Size(): width(0), height(0) {}
  Size(float w, float h): width(w), height(h) {}
  void zero() { width = height = 0; }
  float width, height;
};

struct Rect {
  Rect() {}
  Rect(const Point& anchor_, const Size& size_): anchor(anchor_), size(size_) {}
  Rect(float x, float y, float w, float h): anchor( Point(x,y) ), size( Size(w,h) ) {}
  void zero() { anchor.zero(); size.zero();zero(); }
  Point anchor; // left bottom
  Size  size;
};

} // end of ssp

#endif // end of TYPES_H
