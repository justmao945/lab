
#include<climits>
#include"map.h"

#undef PEL_ULONG_32
#if ULONG_MAX == 4294967295UL // 32 bit
  #define PEL_ULONG_32
#endif


namespace pel{


/**
 * MurmurHash function.
 * @see https://sites.google.com/site/murmurhash
 */
#if !defined(PEL_ULONG_32) // 64-bit
unsigned long murmur2(const void* key, std::size_t len)
{
  unsigned long m = 0xc6a4a7935bd1e995;
  int r = 47;

  unsigned long h = len * m;

  const unsigned long * data = (const unsigned long *)key;
  const unsigned long * end = data + (len/8);

  while(data != end) {
    unsigned long k = *data++;

    k *= m; 
    k ^= k >> r; 
    k *= m; 
    
    h ^= k;
    h *= m; 
  }

  const unsigned char * data2 = (const unsigned char*)data;

  switch(len & 7) {
  case 7: h ^= (unsigned long)data2[6] << 48;
  case 6: h ^= (unsigned long)data2[5] << 40;
  case 5: h ^= (unsigned long)data2[4] << 32;
  case 4: h ^= (unsigned long)data2[3] << 24;
  case 3: h ^= (unsigned long)data2[2] << 16;
  case 2: h ^= (unsigned long)data2[1] << 8;
  case 1: h ^= (unsigned long)data2[0];
          h *= m;
  };
 
  h ^= h >> r;
  h *= m;
  h ^= h >> r;

  return h;
}
#else /* 32-bit */
unsigned long murmur2(const void* key, std::size_t len)
{
  unsigned long m = 0x5bd1e995;
  unsigned long r = 24;
  unsigned long h = len;
  const unsigned char* data = (const unsigned char*)key;
  unsigned long k;
  for (; len >= 4; data += 4, len -= 4) {
    k = *(const unsigned long*)data;
    k *= m;
    k ^= k >> r;
    k *= m;
    h *= m;
    h ^= k;
  }
  switch (len) {
  case 3:
    h ^= data[2] << 16;
  case 2:
    h ^= data[1] << 8;
  case 1:
    h ^= data[0];
    h *= m;
  }
  h ^= h >> 13;
  h *= m;
  h ^= h >> 15;
  return h;
}
#endif


// Using binary search to round up to a prime number.
unsigned long r2prime(unsigned long x)
{
  const static unsigned long primes[] =
  {
    53UL,         97UL,         193UL,       389UL,       769UL,
    1543UL,       3079UL,       6151UL,      12289UL,     24593UL,
    49157UL,      98317UL,      196613UL,    393241UL,    786433UL,
    1572869UL,    3145739UL,    6291469UL,   12582917UL,  25165843UL,
    50331653UL,   100663319UL,  201326611UL, 402653189UL, 805306457UL,
    1610612741UL, 3221225473UL, /* 27 */

#ifdef PEL_ULONG_32
    4294967291UL, /* 1 */

#else
    6442450967UL,         12884901893UL,        25769803799UL,        51539607599UL,
    103079215153UL,       206158430293UL,       412316860583UL,       824633721091UL,
    1649267442203UL,      3298534884361UL,      6597069768761UL,      13194139537429UL,
    26388279074827UL,     52776558149633UL,     105553116299303UL,    211106232598561UL,
    422212465197101UL,    844424930394187UL,    1688849860788229UL,   3377699721576461UL,
    6755399443152931UL,   13510798886305801UL,  27021597772611643UL,  54043195545223217UL,
    108086391090446389UL, 216172782180892721UL, 432345564361785353UL, 864691128723570697UL,
    1729382257447141433UL,3458764514894282849UL,6917529029788565509UL,13835058059577131107UL,
    /* 32 */
#endif
  };

  unsigned long l, r, m, prime;
  l = 0;
  r = sizeof(primes) / sizeof(unsigned long) - 1;

  while( l < r ){
    m = (l + r) >> 1;
    prime = primes[m];
    if( x > prime ){
      l = m + 1;  /* search right half */
    }else if( x < prime ){
      r = m; /* search left half, can't be `m - 1` as `m` may be `0` */
    }else{
      return x;
    }
  }
  assert(l < sizeof(primes) / sizeof(unsigned long));
  return primes[l];
}



}// ~pel



//---------------------------------------
//  Test map
//--------------------------------------
#include <cstdio>
#include "test.h"

namespace pel{


void TestMap()
{
  TestBegin("Map");
  Map<const char*, int> m;

  m.Add("1", 1);
  m.Add("2", 2);
  m.Add("3", 3);
  m.Add("4", 4);
  m.Add("5", 5);
  
  printf("5 -> %d\n", m.Find("5")->Value());
  printf("4 -> %d\n", m.Find("4")->Value());
  printf("3 -> %d\n", m.Find("3")->Value());
  printf("2 -> %d\n", m.Find("2")->Value());
  printf("1 -> %d\n", m.Find("1")->Value());
  printf("fgg -> %p\n", m.Find("fgg"));

  Map<int, int> im;
  printf("add 100[0,99] new key val...\n");
  for(size_t i=0; i<100; ++i){
    im.Add(i, i);
  }

  printf("34 -> %d\n", im.Find(34)->Value());
  printf("89 -> %d\n", im.Find(89)->Value());
  printf("0  -> %d\n", im.Find(0)->Value());
  printf("64 -> %d\n", im.Find(64)->Value());

  printf("1000 -> %p\n", im.Find(1000));

  Map<int,int>::Box *b = im.Add(10, 100);
  printf("add <10,100> dup %p\n", b);
  printf("dup %d -> %d\n", b->Key(), b->Value());

  TestEnd();
}



} // ~pel
