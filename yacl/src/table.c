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
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <yacl/table.h>
#include <yacl/hash.h> /* prime list */

/* table reference */
static table_ref_t*
_ref_initcp_(table_ref_t *self, const table_ref_t *other)
{
  assert(self);
  assert(other);
  *self = *other;
  return self;
}

static object_t*
_ref_value(table_ref_t ref)
{
  return (object_t*)ref->value;
}

static table_ref_t
_ref_next(table_ref_t ref)
{
  assert(ref);
  return ref->next;
}

static table_ref_t
_ref_next_n(table_ref_t ref, ptrdiff_t n)
{
  while(n-- > 0 && ref != NULL)
    ref = ref->next;
  return ref;
}

static ptrdiff_t
_ref_dist(table_ref_t first, table_ref_t last)
{
  register ptrdiff_t n = 0;
  while( first != last ){
    first = first->next;
    n++;
  }
  return n;
}

const table_ref_z TableRef =
{
  ._sizeof_ = sizeof(table_ref_t),
  ._init_   = NULL,
  ._initcp_ = _ref_initcp_,
  ._del_    = NULL,
  .value    = _ref_value,
  .next     = _ref_next,
  .next_n   = _ref_next_n,
  .dist     = _ref_dist,
};



/* - table - */

/*  self:( table_t* ) */
#undef  __node_size
#define __node_size(self) (sizeof(struct __table_ref_t) + self->Key->_sizeof_)

/* Using binary search to get the ceilling primer of `x` in the list. */
static size_t __ceil_prime(size_t x)
{
  size_t l = 0,
         r = sizeof(hash_prime_list) / sizeof(size_t) - 1,
         m, prime;

  while( l < r ){
    m = (l + r) >> 1;
    prime = hash_prime_list[m];
    if( x > prime )
      l = m + 1;  /* search right half */
    else if( x < prime )
      r = m; /* search left half, can't be `m - 1` as `m` may be `0` */
    else
      return x;
  }
  return hash_prime_list[l];
}

static table_t*
_init_(table_t* self, const objcmp_z* Key, hash_f* key_hash, cmp_f* key_eq)
{
  assert(self);
  assert(Key);
  assert(Key->_initcp_);
  
  self->Alloc = &Alloc;
  self->alloc = &_alloc_;
  self->bkt = NULL;
  self->bkt_size = 0;
  self->Key = Key;

  if( key_hash == NULL ){
    assert(Key->_hash_);
    self->key_hash = Key->_hash_;
  } else {
    self->key_hash = key_hash;
  }

  if( key_eq == NULL ){
    assert(Key->_eq_);
    self->key_eq = Key->_eq_;
  } else {
    self->key_eq = key_eq;
  }

  self->size = 0;
  self->max_load_factor = 1.f;
  return self;
}

static bool
_empty(const table_t* self)
{
  assert(self);
  return self->size == 0;
}

static size_t
_size(const table_t* self)
{
  assert(self);
  return self->size;
}

static size_t
_bucket_count(const table_t* self)
{
  assert(self);
  return self->bkt_size;
}

static float
_load_factor(const table_t* self)
{
  assert(self);
  return self->bkt_size == 0 ? 0.f : (float)self->size / self->bkt_size;
}

static float
_max_load_factor(table_t* self, float max_load_factor)
{
  assert(self);
  if( max_load_factor <= 0.f )
    return self->max_load_factor;
  else
    return self->max_load_factor = max_load_factor;
}

static table_t*
_rehash(table_t* self, size_t n_buckets)
{
  assert(self);
  if(n_buckets > self->bkt_size){
    size_t bkt_size = __ceil_prime(n_buckets);
    table_ref_t* bkt = self->Alloc->calloc(self->alloc, bkt_size, sizeof(table_ref_t));
    if( bkt == NULL ){
      fprintf(stderr, "Table.rehash() calloc failed.\n");
      return self;
    }
  {
    table_ref_t *beg = self->bkt, *end = self->bkt + self->bkt_size, *_ref;
    table_ref_t ref, next;
    while( beg < end ){ /* traverse vector. */
      ref = *beg;
      while( ref != NULL ){ /* tarverse list. */
        next = ref->next;
        _ref = bkt + self->key_hash(ref->value) % bkt_size;
        ref->next = *_ref;
        *_ref = ref; /* move `ref` to `*_ref` until `NULL` */
        ref = next;
      }
      ++beg;
    }
    self->Alloc->free(self->alloc, self->bkt);
    self->bkt = bkt;
    self->bkt_size = bkt_size;
  }
  }
  return self;
}

static table_t*
_reserve(table_t* self, size_t n_elements)
{
  return _rehash(self, (size_t)ceil(n_elements / self->max_load_factor));
}

static table_t*
_initcp_(table_t* self, const table_t* other)
{
  assert(self);
  assert(other);
  *self = *other; 
  self->bkt = self->Alloc->calloc(self->alloc, self->bkt_size, sizeof(table_ref_t));
  if( self->bkt == NULL ){
    fprintf(stderr, "Table._initcp_() calloc failed.\n");
    return self;
  }
  
  {
    table_ref_t *beg = other->bkt, *end = other->bkt + other->bkt_size, *_ref;
    table_ref_t ref, n_ref;
    self->size = 0;
    while(beg < end){ /* traverse vector */
      if( (ref = *beg) != NULL ){
        _ref = self->bkt + ( beg - other->bkt );
        while( ref != NULL ){ /* traverse list */
          n_ref = self->Alloc->malloc(self->alloc, __node_size(self));
          if(n_ref == NULL){
            fprintf(stderr, "Table._initcp_() malloc failed.\n");
            continue;
          }
          self->Key->_initcp_(n_ref->value, ref->value);
          n_ref->next = (*_ref);
          (*_ref) = n_ref;  /* insert to list head */
          ++self->size;
          ref = ref->next;
        }
      }
      ++beg;
    }
  }
  return self;
}

static table_t*
_swap(table_t* self, table_t* other)
{
  table_t t;
  assert(self);
  assert(other);

  t = *self;
  *self = *other;
  *other = t;
  return self;
}

static table_t*
_clear(table_t* self)
{
  table_ref_t *beg, *end, ref, next;

  assert(self);
  
  beg = self->bkt;
  end = self->bkt + self->bkt_size;
  if( self->Key->_del_ ){
    while( beg < end ){
      ref = *beg;
      while( ref != NULL ){
        next = ref->next;
        self->Key->_del_(ref->value);
        self->Alloc->free(self->alloc, ref);
        ref = next;
      }
      *beg = NULL;
      ++beg;
    }
  }else{
    while( beg < end ){
      ref = *beg;
      while( ref != NULL ){
        next = ref->next;
        self->Alloc->free(self->alloc, ref);
        ref = next;
      }
      *beg = NULL;
      ++beg;
    }
  }
  self->size = 0;
  return self;
}

static table_t*
_del_(table_t* self)
{
  assert(self);
  _clear(self);
  self->Alloc->free(self->alloc, self->bkt);
  return (table_t*)memset(self, 0, sizeof(table_t));
}

static table_ref_t
_begin(const table_t* self, size_t bucket_ix)
{
  assert(self);
  assert(bucket_ix < self->bkt_size);
  return self->bkt[bucket_ix];
}

static table_ref_t
_end(const table_t* self, size_t bucket_ix)
{
  return NULL;
}

/* failed return `NULL` */
static table_ref_t
_insert(table_t* self, const objcmp_t* key)
{
  table_ref_t ref, *_ref;

  assert(self);
  assert(key);
  
  _reserve(self, self->size + 1);
  _ref = self->bkt + self->key_hash(key) % self->bkt_size;
  for(ref = *_ref; ref != NULL; ref = ref->next){
    if( self->key_eq(ref->value, key) )
      return NULL;  /* duplicated */
  }
  ref = self->Alloc->malloc(self->alloc, __node_size(self));
  if(ref == NULL){
    fprintf(stderr, "Table.insert() malloc failed.\n");
    return NULL;
  }
  self->Key->_initcp_(ref->value, key);
  ref->next = *_ref;
  *_ref = ref;
  ++self->size;
  return ref;
}

static table_ref_t
_insert_n(table_t* self, size_t n, const objcmp_t* keys)
{
  table_ref_t ret;
  const objcmp_t *beg, *end;

  assert(self);
  assert(keys);

  beg = keys;
  end = keys + n * self->Key->_sizeof_;
  _reserve(self, self->size + n);
  while(beg < end){
    ret = _insert(self, beg);
    beg += self->Key->_sizeof_;
  }
  return ret;
}

static table_ref_t
_insert_r(table_t* self, const ref_z *Ref, ref_t first, ref_t last)
{
  table_ref_t ret;
  ptrdiff_t dist;

  assert(self);
  assert(Ref);
  assert(Ref->next);
  assert(Ref->value);
  
  dist = Ref->dist(first, last);
  if( dist < 0 ) return first;

  _reserve(self, self->size + dist);
  while(first != last){
    ret = _insert(self, Ref->value(first));
    first = Ref->next(first);
  }
  return ret;
}

static table_ref_t
_erase(table_t* self, const objcmp_t* key)
{
  table_ref_t ref, next, *_ref;
  assert(self);
  assert(key);

  if( self->bkt_size == 0 )
    return NULL;

  _ref = self->bkt + self->key_hash(key) % self->bkt_size;
  ref  = *_ref;
  if( ref != NULL && self->key_eq(ref->value, key) ){
    next = ref->next;
    if(self->Key->_del_) self->Key->_del_(ref->value);
    self->Alloc->free(self->alloc, ref);
    --self->size;
    return *_ref = next;
  }

  while(ref->next != NULL && ! self->key_eq(ref->next->value, key))
    ref = ref->next;

  if( ref->next != NULL ){
    next = ref->next->next;
    if(self->Key->_del_) self->Key->_del_(ref->next->value);
    self->Alloc->free(self->alloc, ref->next);
    ref->next = next;
    --self->size;
  }
  return ref->next;
}

static table_ref_t
_find(const table_t* self, const objcmp_t* key)
{
  table_ref_t ref;
  assert(self);
  assert(key);

  if( self->bkt_size == 0 )
    return NULL;

  ref = self->bkt[self->key_hash(key) % self->bkt_size];
  while(ref != NULL && ! self->key_eq(ref->value, key))
    ref = ref->next;
  return ref;
}

/* Not found return (size_t)(-1) */
static size_t
_bucket(const table_t* self, const objcmp_t* key)
{
  assert(self);
  assert(key);
  return self->bkt_size == 0 ? -1 : self->key_hash(key) % self->bkt_size;
}

/* move elements from `other`. */
static table_t*
_merge(table_t* self, table_t* other)
{
  table_ref_t *beg, *end, ref, next, *_ref;

  assert(self);
  assert(other);
  
  _reserve(self, self->size + other->size);

  beg = other->bkt, end = other->bkt + other->bkt_size;
  while(beg < end){ /* traverse vector */
    ref = *beg;
    while( ref != NULL ){ /* traverse list */
      _ref = self->bkt + self->key_hash(ref->value) % self->bkt_size;
      next = ref->next;
      ref->next = *_ref;  /* move `ref` to `*_ref` */
      *_ref = ref;
      ref = next;
    }
    *beg = NULL;
    ++beg;
  }
  self->size += other->size;
  other->size = 0;
  return self;
}


const table_z Table = 
{
  ._sizeof_ = sizeof(table_t),
  ._init_   = _init_,
  ._initcp_ = _initcp_,
  ._del_    = _del_,
  .begin    = _begin,
  .end      = _end,
  .empty    = _empty,
  .size     = _size,
  .clear    = _clear,
  .insert   = _insert,
  .insert_n = _insert_n,
  .insert_r = _insert_r,
  .erase    = _erase,
  .find     = _find,
  .bucket   = _bucket,
  .bucket_count = _bucket_count,
  .load_factor  = _load_factor,
  .max_load_factor = _max_load_factor,
  .rehash   = _rehash,
  .reserve  = _reserve,
  .swap     = _swap,
  .merge    = _merge,
};

