/*
 * Copyright (c) 2012, Mao Jianjun <justmao945@gmail.com>
 * 
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN 
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <yacl/hash.h>

const size_t hash_prime_list[] = 
{
  53UL,         97UL,         193UL,       389UL,       769UL,
  1543UL,       3079UL,       6151UL,      12289UL,     24593UL,
  49157UL,      98317UL,      196613UL,    393241UL,    786433UL,
  1572869UL,    3145739UL,    6291469UL,   12582917UL,  25165843UL,
  50331653UL,   100663319UL,  201326611UL, 402653189UL, 805306457UL, 
  1610612741UL, 3221225473UL, /* 27 */
#ifdef YACL_64
  6442450967UL,         12884901893UL,        25769803799UL,        51539607599UL,
  103079215153UL,       206158430293UL,       412316860583UL,       824633721091UL,
  1649267442203UL,      3298534884361UL,      6597069768761UL,      13194139537429UL,
  26388279074827UL,     52776558149633UL,     105553116299303UL,    211106232598561UL,
  422212465197101UL,    844424930394187UL,    1688849860788229UL,   3377699721576461UL,
  6755399443152931UL,   13510798886305801UL,  27021597772611643UL,  54043195545223217UL,
  108086391090446389UL, 216172782180892721UL, 432345564361785353UL, 864691128723570697UL,
  1729382257447141433UL,3458764514894282849UL,6917529029788565509UL,13835058059577131107UL,
  /* 32 */
#else
  4294967291UL, /* 1 */
#endif
};


/**
 * MurmurHash function.
 * @see https://sites.google.com/site/murmurhash
 */
#ifdef YACL_64 /* 64-bit */
size_t hash_murmur2(const void* key, size_t len)
{
  const size_t m = 0xc6a4a7935bd1e995;
  const int r = 47;

  size_t h = len * m;

  const size_t * data = (const size_t *)key;
  const size_t * end = data + (len/8);

  while(data != end) {
    size_t k = *data++;

    k *= m; 
    k ^= k >> r; 
    k *= m; 
    
    h ^= k;
    h *= m; 
  }

  const unsigned char * data2 = (const unsigned char*)data;

  switch(len & 7) {
  case 7: h ^= (size_t)data2[6] << 48;
  case 6: h ^= (size_t)data2[5] << 40;
  case 5: h ^= (size_t)data2[4] << 32;
  case 4: h ^= (size_t)data2[3] << 24;
  case 3: h ^= (size_t)data2[2] << 16;
  case 2: h ^= (size_t)data2[1] << 8;
  case 1: h ^= (size_t)data2[0];
          h *= m;
  };
 
  h ^= h >> r;
  h *= m;
  h ^= h >> r;

  return h;
}
#else /* 32-bit */
size_t hash_murmur2(const void* key, size_t len)
{
  const size_t m = 0x5bd1e995;
  const size_t r = 24;
  size_t h = len;
  const unsigned char* data = (const unsigned char*)key;
  size_t k;
  for (; len >= 4; data += 4, len -= 4) {
    k = *(const size_t*)data;
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
#endif /* - YACL_64 - */

