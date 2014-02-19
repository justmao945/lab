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
#include <yacl/vector.h>
#include <yacl/utils.h>


static void
__copy_init(object_t* dest, const object_t* first, const object_t* last,
    const object_z* Object)
{
  while( first < last ){
    Object->_initcp_(dest, first);
    dest  += Object->_sizeof_;
    first += Object->_sizeof_;
  }
}

static void
__del(object_t* first, object_t* last, const object_z* Object)
{
  if( Object->_del_ ){
    while(first < last){
      Object->_del_(first);
      first += Object->_sizeof_;
    }
  }
}


static vector_t*
_init_(vector_t* self, const object_z* Object)
{
  assert(self);
  assert(Object);
  assert(Object->_sizeof_ > 0);
  assert(Object->_initcp_);

  self->Object = Object;
  self->beg = self->end = self->mem_end = NULL;
  self->Alloc  = &Alloc;
  self->alloc  = &_alloc_;
  return self;
}

static bool 
_empty(const vector_t* self)
{
  assert(self);
  return self->beg == self->end;
}

static size_t
_size(const vector_t* self)
{
  assert(self);
  return (self->end - self->beg)  / self->Object->_sizeof_;
}

static size_t
_capacity(const vector_t* self)
{
  assert(self);
  return (self->mem_end - self->beg)  / self->Object->_sizeof_;
}

static vector_t*
_reserve(vector_t* self, size_t n)
{
  assert(self);

  if( _capacity(self) < n ){
    object_t* beg = self->Alloc->malloc(self->alloc, n * self->Object->_sizeof_);
    if(beg == NULL){
      fprintf(stderr, "Vector.reserve(): malloc failed.\n");
      return self;
    }
    __copy_init(beg, self->beg, self->end, self->Object);
    __del(self->beg, self->end, self->Object);
    self->Alloc->free(self->alloc, self->beg);
    self->end = beg + (self->end - self->beg);
    self->beg = beg;
    self->mem_end = beg + n * self->Object->_sizeof_;
  }
  return self;
}

static vector_t*
_initcp_(vector_t* self, const vector_t* other)
{
  assert(self);
  assert(other);

  if(self == other)
    return self;

  self->Object = other->Object;
  self->Alloc = other->Alloc;
  self->alloc = other->alloc;

  self->beg = self->end = self->mem_end = NULL;
  _reserve(self, _size(other)); /* nothing inited. */
  __copy_init(self->beg, other->beg, other->end, self->Object);
  self->end = self->beg + (other->end - other->beg);
  return self;
}

static vector_t*
_swap(vector_t* self, vector_t* other)
{
  vector_t t;

  assert(self);
  assert(other);

  t = *self;
  *self = *other;
  *other = t;

  return self;
}

static vector_t*
_del_(vector_t* self)
{
  assert(self);

  __del(self->beg, self->end, self->Object);
  self->Alloc->free(self->alloc, self->beg);
  return (vector_t*)memset(self, 0, sizeof(vector_t));
}

static object_t*
_get(const vector_t* self, ptrdiff_t i)
{
  object_t* value;

  assert(self);
  if( i > 0 ){
    value = self->beg + i * (ptrdiff_t)self->Object->_sizeof_;
    return value < self->end ? value  : NULL;
  } else {
    value = self->end + i * (ptrdiff_t)self->Object->_sizeof_;
    return -i - 1 < _size(self) ? value  : NULL;
  }
}

static vector_t*
_set(vector_t* self, ptrdiff_t i, const object_t* value)
{
  object_t* data;

  assert(self);
  assert(value);

  if( (data = _get(self, i)) != NULL ){
    if(self->Object->_del_) self->Object->_del_(data);
    self->Object->_initcp_(data, value);
  }

  return self;
}


static object_t*
_front(const vector_t* self)
{
  assert(self);
  assert( ! _empty(self) );

  return self->beg;
}

static object_t*
_back(const vector_t* self)
{
  assert(self);
  assert( ! _empty(self) );

  return self->end - self->Object->_sizeof_;
}

static vector_ref_t
_begin(const vector_t* self)
{
  assert(self);
  return (vector_ref_t)self->beg;
}

static vector_ref_t
_end(const vector_t* self)
{
  assert(self);
  return (vector_ref_t)self->end;
}

static vector_ref_t
_rbegin(const vector_t* self)
{
  assert(self);
  return (vector_ref_t)(self->end - self->Object->_sizeof_);
}

static vector_ref_t
_rend(const vector_t* self)
{
  assert(self);
  return (vector_ref_t)(self->beg - self->Object->_sizeof_);
}

static vector_ref_t
_find(const vector_t* self, cmp_f* eq, const object_t* value)
{
  vector_ref_t ref;

  assert(self);
  assert(eq);
  
  for( ref = self->beg; ref < self->end; ref += self->Object->_sizeof_ ){
    if( eq((object_t*)ref, value) )
      return ref;
  }
  return self->end;
}

static vector_t*
_shrink(vector_t* self)
{
  assert(self);

  if( _capacity(self) > _size(self) ){
    object_t* data = self->Alloc->malloc(self->alloc, self->end - self->beg);
    if( data == NULL ){
      fprintf(stderr, "Vector.shrink(): malloc failed.\n");
      return self;
    }
    __copy_init(data, self->beg, self->end, self->Object);
    __del(self->beg, self->end, self->Object);
    self->Alloc->free(self->alloc, self->beg);
    self->end = data + (self->end - self->beg);
    self->mem_end = self->end;
    self->beg = data;
  }
  return self;
}

static vector_t*
_clear(vector_t* self)
{
  assert(self);

  __del(self->beg, self->end, self->Object);
  self->end = self->beg;
  return self;
}

static vector_t*
_push_back(vector_t* self, const object_t* value)
{
  assert(self);
  assert(value);
  if( _size(self) >= _capacity(self) )
    _reserve(self, _empty(self) ? 1 : (_capacity(self) << 1));
  self->Object->_initcp_(self->end, value);
  self->end += self->Object->_sizeof_;
  return self;
}

static vector_t*
_pop_back(vector_t* self)
{
  assert(self);
  assert( !_empty(self) );

  self->end -= self->Object->_sizeof_;
  if(self->Object->_del_) self->Object->_del_(self->end);
  return self;
}

static vector_t*
_resize(vector_t* self, size_t n, const object_t* value)
{
  object_t* end;

  assert(self);
  assert(value);
  
  _reserve(self, n);
  end = self->beg + n * self->Object->_sizeof_;
  __del(end, self->end, self->Object);
  while( self->end < end ){
    self->Object->_initcp_(self->end, value);
    self->end += self->Object->_sizeof_;
  }
  self->end = end;
  return self;
}

static vector_t*
_reverse(vector_t* self)
{
  assert(self);
  return m_reverse(self->beg, _size(self), self->Object->_sizeof_);
}

static vector_t*
_sort(vector_t* self, cmp_f* lt)
{
  assert(self);
  assert(lt);
  return m_qsort(self->beg, _size(self), self->Object->_sizeof_, lt);
}

static vector_t*
_append(vector_t* self, const vector_t* other)
{
  assert(self);
  assert(other);
  /* FIXME: assert(self->Object == other->Object) ? */

  if( self != other ){
    _reserve(self, _size(self) + _size(other));
    __copy_init( self->end, other->beg, other->end, self->Object );
    self->end += other->end - other->beg;
  } else if( ! _empty(other) ){
    vector_t t;
    _initcp_(&t, other);
    _append(self, &t);
    _del_(&t);
  }
  return self;
}

static vector_t*
_append_n(vector_t* self, size_t n, const object_t* data)
{
  size_t size;

  assert(self);
  assert(data);
  size = n * self->Object->_sizeof_;

  if( data >= self->end || data + size < self->beg ){
    _reserve(self, _size(self) + n);
    __copy_init( self->end, data, data + size, self->Object );
    self->end += size;
  } else if( n > 0 ){
    vector_t t;
    _initcp_(&t, self);
    _append_n(&t, n, data);
    _swap(&t, self);
    _del_(&t);
  }
  return self;
}

static vector_t*
_append_r(vector_t* self, const ref_z* Ref, ref_t first, ref_t last)
{
  ptrdiff_t size;
    
  assert(self);
  assert(Ref);
   
  size = Ref->dist(first, last);
  /* FIXME: dist(...) < 0 ? */
  if( size <= 0 )
    return self;

  if( first >= self->end || last <= self->beg ){
    _reserve(self, _size(self) + size);
    while( first != last ){
      self->Object->_initcp_(self->end, Ref->value(first));
      first = Ref->next(first);
      self->end += self->Object->_sizeof_;
    }
  } else {
    vector_t t;
    _initcp_(&t, self);
    _append_r(&t, Ref, first, last);
    _swap(&t, self);
    _del_(&t);
  }
  return self;
}

/* erase between `[pos, end)` */
static vector_ref_t 
_erase_back(vector_t* self, vector_ref_t pos)
{
  assert(self);
  if( pos < self->beg || pos >= self->end )
    return pos;

  if(self->Object->_del_){
    while( pos != self->end ){
      self->end -= self->Object->_sizeof_;
      self->Object->_del_(self->end);
    }
    self->Object->_del_(self->end);
  }else{
    self->end = pos;
  }
  return pos;
}

const vector_z Vector = 
{
  ._sizeof_ = sizeof(vector_t),
  ._init_   = _init_,
  ._initcp_ = _initcp_,
  ._del_    = _del_,
  .get      = _get,
  .set      = _set,
  .front    = _front,
  .back     = _back,
  .begin    = _begin,
  .end      = _end,
  .rbegin   = _rbegin,
  .rend     = _rend,
  .find     = _find,
  .empty    = _empty,
  .size     = _size,
  .capacity = _capacity,
  .reserve  = _reserve,
  .shrink   = _shrink,
  .clear    = _clear,
  .push_back = _push_back,
  .pop_back = _pop_back,
  .resize   = _resize,
  .reverse  = _reverse,
  .sort     = _sort,
  .append   = _append,
  .append_n = _append_n,
  .append_r = _append_r,
  .erase_back = _erase_back,
  .swap     = _swap,
};

