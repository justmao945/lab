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
#include <stdlib.h>
#include <string.h>
#include <yacl/heap.h>
#include <yacl/utils.h>

/* heap impl */

static heap_t*
_init_(heap_t* self, const objcmp_z* Objcmp, cmp_f* lt)
{
  Vector._init_((vector_t*)self, (object_z*)Objcmp);
  if(lt == NULL) self->lt = Objcmp->_lt_; else self->lt = lt;
  assert(self->lt);
  return self;
}

static heap_t*
_initcp_(heap_t* self, const heap_t* other)
{
  Vector._initcp_((vector_t*)self, (vector_t*)other);
  self->lt = other->lt;
  return self;
}

static heap_t*
_del_(heap_t* self)
{
  Vector._del_((vector_t*)self);
  self->lt = (cmp_f*)NULL;
  return self;
}

static bool
_empty(const heap_t* self)
{
  return vector_empty(self);
}

static objcmp_t*
_top(const heap_t* self)
{
  assert(self);
  assert( ! _empty(self) );
  return self->beg;
}

static size_t
_size(const heap_t* self)
{
  assert(self);
  return vector_size(self);
}

static size_t
_capacity(const heap_t* self)
{
  assert(self);
  return vector_capacity(self);
}

static heap_t*
_reserve(heap_t* self, size_t n)
{
  return (heap_t*)Vector.reserve((vector_t*)self, n);
}

static heap_t*
_shrink(heap_t* self)
{
  return (heap_t*)Vector.shrink((vector_t*)self);
}

static heap_t*
_clear(heap_t* self)
{
  return (heap_t*)Vector.clear((vector_t*)self);
}

static heap_t*
_push(heap_t* self, const objcmp_t* value)
{
  Vector.push_back((vector_t*)self, value);
  push_heap(Vector.begin((vector_t*)self), Vector.end((vector_t*)self),
            (objcmp_z*)self->Object, self->lt);
  return self;
}

/* copy elements from `other`. Not move... */
static heap_t*
_merge(heap_t* self, const heap_t* other)
{
  _reserve(self, _size(self) + _size(other));
  Vector.append((vector_t*)self, (vector_t*)other);

  make_heap(Vector.begin((vector_t*)self), Vector.end((vector_t*)self),
            (objcmp_z*)self->Object, self->lt);
  return self;
}

static heap_t*
_push_n(heap_t* self, size_t n, const objcmp_t* data)
{
  ref_t first, last, last0;

  _reserve(self, _size(self) + n);
  first = Vector.begin((vector_t*)self);
  last0 = Vector.end((vector_t*)self) + self->Object->_sizeof_;

  Vector.append_n((vector_t*)self, n, data);
  last = Vector.end((vector_t*)self);

  while( last0 < last ){
    push_heap(first, last0, (objcmp_z*)self->Object, self->lt);
    last0 += self->Object->_sizeof_;
  }
  return self;
}

static heap_t*
_push_r(heap_t* self, const ref_z* Ref, ref_t first, ref_t last)
{
  ptrdiff_t dist;
  ref_t _first, _last, _last0;

  assert(Ref);
  if( (dist = Ref->dist(first, last)) <= 0 )
    return self;

  _reserve(self, _size(self) + dist);
  _first = Vector.begin((vector_t*)self);
  _last0 = Vector.end((vector_t*)self) + self->Object->_sizeof_;

  Vector.append_r((vector_t*)self, Ref, first, last);
  _last = Vector.end((vector_t*)self);

  while( _last0 < _last ){
    push_heap(_first, _last0, (objcmp_z*)self->Object, self->lt);
    _last0 += self->Object->_sizeof_;
  }

  return self;
}

static heap_t*
_pop(heap_t* self)
{
  assert(self);
  pop_heap(self->beg, self->end, (objcmp_z*)self->Object, self->lt);
  Vector.pop_back((vector_t*)self);
  return self;
}

static heap_t*
_pop_n(heap_t* self, size_t n)
{
  vector_ref_t* end;
  assert(self);

  end = self->end;
  while(n-- > 0){
    pop_heap(self->beg, end, (objcmp_z*)self->Object, self->lt);
    end -= self->Object->_sizeof_;
  }
  Vector.erase_back((vector_t*)self, end);
  return self;
}

static heap_t*
_swap(heap_t *self, heap_t* other)
{
  heap_t t;

  assert(self);
  assert(other);
  
  t = *self;
  *self = *other;
  *other = t;

  return self;
}

const heap_z Heap = 
{
  ._init_   = _init_,
  ._initcp_ = _initcp_,
  ._del_    = _del_,
  .top      = _top,
  .empty    = _empty,
  .size     = _size,
  .capacity = _capacity,
  .reserve  = _reserve,
  .shrink   = _shrink,
  .clear    = _clear,
  .push     = _push,
  .merge    = _merge,
  .push_n   = _push_n,
  .push_r   = _push_r,
  .pop      = _pop,
  .pop_n    = _pop_n,
  .swap     = _swap,
};

/* 
 * origin data:
 *    |0|1|2|3|...
 *     ^ beg
 * rebased data:
 *  |0|1|2|3|4|...
 *   ^ ^beg
 *  base
 */

#undef  __me /* myself */
#define __me(ix)   (base + (ix) * Objcmp->_sizeof_)

#undef  __lch  /* left child */
#define __lch(ix)  (base + ((ix) << 1) * Objcmp->_sizeof_)

#undef  __rch  /* right child */
#define __rch(ix)  (__lch(ix) + Objcmp->_sizeof_)

#undef  __mama /* mother */
#define __mama(ix) (base + ((ix) >> 1) * Objcmp->_sizeof_)

/* max-heap uses `_lt_` */
bool is_heap(const objcmp_t* first, const objcmp_t* last,
    const objcmp_z* Objcmp, cmp_f* lt)
{
  return is_heap_until(first, last, Objcmp, lt) == last;
}

objcmp_t* is_heap_until(const objcmp_t* first, const objcmp_t* last,
    const objcmp_z* Objcmp, cmp_f* lt)
{
  ptrdiff_t i, half;
  const objcmp_t* base;
  
  assert(Objcmp);
  assert(Objcmp->_sizeof_ > 0);
  assert(first > (const objcmp_t*)Objcmp->_sizeof_);
  assert(last);

  if(last - first <= Objcmp->_sizeof_)
    return (objcmp_t*)last;
  
  if(lt == NULL) lt = Objcmp->_lt_;

  base = first - Objcmp->_sizeof_;
  half = (last - first) / Objcmp->_sizeof_ / 2;
  for(i = 1; i < half; i++){
    if( lt(__me(i), __lch(i)) )
      return (objcmp_t*)__lch(i);
    else if( lt(__me(i), __rch(i)) )
      return (objcmp_t*)__rch(i);
  }

  if( lt(__me(i), __lch(i)) )
    return (objcmp_t*)__lch(i);
  else if( __rch(i) < last && lt(__me(i), __rch(i)) )
    return (objcmp_t*)__rch(i);

  return (objcmp_t*)last;
}

void make_heap(objcmp_t* first, objcmp_t* last, const objcmp_z* Objcmp, cmp_f* lt)
{
  assert(Objcmp);
  assert(Objcmp->_sizeof_);

  if( last - first > Objcmp->_sizeof_ ){ /* at least 2 elements */
    size_t i = (last - first) / Objcmp->_sizeof_ / 2; /* i >= 1 */
    objcmp_t *base = first - Objcmp->_sizeof_;

    if(lt == NULL)
      lt = Objcmp->_lt_;
    
   if( __rch(i) < last && lt(__lch(i), __rch(i)) ) /* l < r --> swap(l, r) */
     m_swap(__lch(i), __rch(i), Objcmp->_sizeof_);
   if( lt(__me(i), __lch(i)) ) /* i < l --> swap(i, l) */
     m_swap(__me(i), __lch(i), Objcmp->_sizeof_);

    for(--i; i >= 1; --i){
      if( lt(__rch(i), __lch(i)) ) /* r < l --> swap(r, l) */
        m_swap(__rch(i), __lch(i), Objcmp->_sizeof_);
      if( lt(__me(i), __rch(i)) ) /* i < r --> swap(i, r) */
        m_swap(__me(i), __rch(i), Objcmp->_sizeof_);
    }
  }
}

void push_heap(objcmp_t* first, objcmp_t* last, const objcmp_z* Objcmp, cmp_f* lt)
{
  assert(Objcmp);
  assert(Objcmp->_sizeof_ > 0);
  assert(first > (objcmp_t*)Objcmp->_sizeof_);
  assert(last);

  if( last - first > Objcmp->_sizeof_ ){
    objcmp_t* base = first - Objcmp->_sizeof_;
    size_t i = (last - Objcmp->_sizeof_ - base) / Objcmp->_sizeof_;
    if(lt == NULL) lt = Objcmp->_lt_;
    while( i > 1 ){
      if( lt(__mama(i), __me(i)) )
        m_swap(__mama(i), __me(i), Objcmp->_sizeof_);
      i >>= 1;
    }
  }
}

void pop_heap(objcmp_t* first, objcmp_t* last, const objcmp_z* Objcmp, cmp_f* lt)
{
  assert(Objcmp);
  assert(Objcmp->_sizeof_ > 0);
  assert(first > (objcmp_t*)Objcmp->_sizeof_);
  assert(last);

  if( last - first > Objcmp->_sizeof_ ){
    objcmp_t* base = first - Objcmp->_sizeof_, *b_last;
    size_t i = 1, half = (last - first) / Objcmp->_sizeof_ / 2;
    if(lt == NULL) lt = Objcmp->_lt_;
    while( i < half ){
      /* 
       * FIXME: How to determine `_eq_(__lch(i), __rch(i))` ? 
       * Because mama should be swapped to her right child if two children are equal.
       */
      if( ! lt(__rch(i), __lch(i)) ){ /* rch >= lch */
        m_swap(__rch(i), __me(i), Objcmp->_sizeof_);
        i = (i<<1) + 1;
      } else {
        m_swap(__lch(i), __me(i), Objcmp->_sizeof_);
        i <<= 1;
      }
    }
    if( __rch(i) < last && ! lt(__rch(i), __lch(i)) ){ /* rch >= lch */
      m_swap(__rch(i), __me(i), Objcmp->_sizeof_);
    } else if( __lch(i) < last ){
      m_swap(__lch(i), __me(i), Objcmp->_sizeof_);
    }

    b_last = last - Objcmp->_sizeof_;
    if( lt(b_last, __me(i)) ) /* consider the one before last. */
      m_swap(b_last, __me(i), Objcmp->_sizeof_);
  }
}

void sort_heap(objcmp_t* first, objcmp_t* last, const objcmp_z* Objcmp, cmp_f* lt)
{
  assert(Objcmp);
  assert(Objcmp->_sizeof_);
  for(; last > first; last -= Objcmp->_sizeof_){
    pop_heap(first, last, Objcmp, lt);
  }
}

