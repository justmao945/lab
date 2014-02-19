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

#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <yacl/utils.h>


void* m_swap(void* lhs, void* rhs, size_t size)
{
  register size_t i;
  assert(lhs);
  assert(rhs);
  
  if( lhs == rhs ) return lhs;

  i = size / sizeof(long);
  while(i--){
    *(long*)lhs ^= *(long*)rhs ^= *(long*)lhs ^= *(long*)rhs;
    lhs += sizeof(long);
    rhs += sizeof(long);
  }

  i = size % sizeof(long);
  while(i--){
    *(char*)lhs ^= *(char*)rhs ^= *(char*)lhs ^= *(char*)rhs;
    lhs += sizeof(char);
    rhs += sizeof(char);
  }
  return lhs;
}


void* m_reverse(void* base, size_t n, size_t size)
{
  void *b, *e;

  assert(base);
  assert(base > (void*)size);
  assert(size > 0);

  b = base;
  e = base + n * size - size;

  while( b < e ){
    m_swap(b, e, size);
    b += size;
    e -= size;
  }

  return base;

}

static void
__inplace_qsort(void* base, size_t n, size_t size,
    bool(*lt)(const void*,const void*,size_t) )
{
#define __s_push(top, low, high) ((top)->lo = (low), (top)->hi = (high), ++(top))
#define __s_pop(top, low, high)  (--(top), (low) = (top)->lo, (high) = (top)->hi)

#define __isort(base, end) \
  { \
    void *i, *j; \
    for(i=base; i<=end; i+=size) \
      for(j=i+size; j<=end; j+=size)  \
        if( lt(j, i, size) ) m_swap(i, j, size); \
  }

  typedef struct{
    void* lo;
    void* hi;
  }range_t;
 
  const size_t max_thresh = 4, max_thresh_size = max_thresh * size;

  if(n == 0)
    return;

  if(n > max_thresh){ /* quick sort */
    void* lo = base;
    void* hi = base + size * n - size;
    range_t stack[8 * sizeof(size_t)];  /* 32-bit: 2^32, 64-bit: 2^64. */
    range_t *top = stack;
    
    __s_push(top, NULL, NULL);

    while( stack < top ){ /* stack based iteration */
      void *l, *r, *m;
      m = lo + size * ((hi - lo) / size >> 1);

      /* set lo < m < hi */
      if( lt(m,  lo, size) ) m_swap(m,  lo, size);
      if( lt(hi, lo, size) ) m_swap(hi, lo, size);
      if( lt(hi, m,  size) ) m_swap(hi, m,  size);

      m_swap(m, hi - size, size); /* hide pivot to `hi -size` */
      l = lo; m = r = hi - size;

      while( 1 ){ /* 'collapse the walls' section of quicksort */
        while( l < hi && lt(l += size, m, size) );
        while( r > lo && lt(m, r -= size, size) );
        if( l < r ) m_swap(l, r, size);
        else break;
      }
      m_swap(l, m, size); /* restore pivot to `l`: | lo |...| r | l(p) |...|hi| */

      /* next iteration */
      if( (size_t)(l - lo) <= max_thresh_size ){
        if( (size_t)(hi - l) <= max_thresh_size ){ /*  ignore both small partitions */
          __isort(lo, hi);
          __s_pop(top, lo, hi);
        } else { /* ignore samll left partition */
          __isort(lo, l - size);
          lo = l + size;
        }
      } else if( (size_t)(hi - l) <= max_thresh_size ){ /* ignore small right partition */
        __isort(l + size, hi);
        hi = l - size;
      } else if( l - lo > hi - l ){ /* push larger left partition */
        __s_push(top, lo, l - size);
        lo = l + size;
      } else { /* push larger right partition */
        __s_push(top, l + size, hi);
        hi = l - size;
      }
    }
    assert(top == stack); /* make sure the right calling. */
  } else {
    __isort(base, base + n * size - size);
  }

#undef __isort
#undef __s_pop
#undef __s_push
}


void* m_qsort(void* base, size_t n, size_t size, void* lt)
{
  assert(base);
  assert(size > 0);
  assert(lt);

  /* TODO: Indirecting sort: only sorting on pointers. */

  __inplace_qsort(base, n, size, lt);
  return base;
}

