/*
 * main.cc
 *
 *  Created on: Dec 20, 2011
 *      Author: just
 */
#include <cstdio>
#include <new>
#include <string>
#include "psrdrv.h"

using namespace std;
using namespace psr;

int main(int argc, char **argv) {
  Tree t;

  if(argc == 2)
    t.gen(argv[1]);
  else
    t.gen();

  puts(t.toString().c_str());
  return 0;
}
