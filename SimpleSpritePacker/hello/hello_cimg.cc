
// disable display
#define cimg_display 0

// enable png
#define cimg_use_png

#include "../cimg/CImg.h"

using namespace cimg_library;

int main() {
  CImg<unsigned char> image("../cimg/examples/img/lena.pgm");
  image.save("lena.png");
  image.blur(2.5);
  image.save("lena-blur2.5.png");
  return 0;
}

// Compile:
//    clang++  hello_cimg.cc -lpng
