
#ifndef PACKER_H
#define PACKER_H

#include <vector>
#include "types.h"

namespace ssp {

class Packer {
public:

  // call all build and optimizations
  void build();

  // greedy algorithm
  void buildWithBinaryTree();

  // reset all built result and input
  void reset();

private:
};

} // end of ssp

#endif // end of PACKER_H
