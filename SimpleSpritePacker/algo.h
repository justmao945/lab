#ifndef ALGO_H
#define ALGO_H

namespace ssp {

/// Round up to next higher power of 2 (return x if it's already a power of 2).
inline int pow2roundup(int x) {
    if (x < 0)
        return 0;
    --x;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return x+1;
}

} // end of ssp

#endif // end of ALGO_H
