/*
 * Copyright )(c) 2012, Mao Jianjun <justmao945@gmail.com>
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


#ifndef YACL_VECTOR_H
#define YACL_VECTOR_H

#include <yacl/alloc.h>

/**
 * vector reference.
 * But have no built-in refercences class type.
 */
typedef object_t* vector_ref_t;

/* vector type */
#undef  def_vector_t
#define def_vector_t \
struct { \
  const object_z *Object; \
  object_t *beg; \
  object_t *end; \
  object_t *mem_end; \
  const alloc_z *Alloc; \
  alloc_t *alloc; \
}
typedef def_vector_t vector_t;


/* vector class */
typedef struct { 
  def_object_z(vector_t, vector_t* self, const object_z* Object);
 
  object_t* (*get)(const vector_t* self, ptrdiff_t i);
 
  vector_t* (*set)(vector_t* self, ptrdiff_t i, const object_t* value);
 
  object_t* (*front)(const vector_t* self);
 
  object_t* (*back)(const vector_t* self);
 
  vector_ref_t (*begin)(const vector_t* self);
 
  vector_ref_t (*end)(const vector_t* self);
 
  vector_ref_t (*rbegin)(const vector_t* self);
 
  vector_ref_t (*rend)(const vector_t* self);
 
  vector_ref_t (*find)(const vector_t* self, cmp_f *eq, const object_t* data);
 
  bool   (*empty)(const vector_t* self);
 
  size_t (*size)(const vector_t* self);
 
  size_t (*capacity)(const vector_t* self);
 
  vector_t* (*reserve)(vector_t* self, size_t n);
 
  vector_t* (*shrink)(vector_t* self);
 
  vector_t* (*clear)(vector_t* self);
 
  vector_t* (*push_back)(vector_t* self, const object_t* value);
 
  vector_t* (*pop_back)(vector_t* self);
 
  vector_t* (*resize)(vector_t* self, size_t n, const object_t* value);
 
  vector_t* (*reverse)(vector_t* self);
 
  vector_t* (*sort)(vector_t* self, cmp_f *lt);
 
  vector_t* (*append)(vector_t* self, const vector_t* other);
 
  vector_t* (*append_n)(vector_t* self, size_t n, const object_t* data);
 
  vector_t* (*append_r)(vector_t* self, const ref_z* Ref, ref_t first, ref_t last);
 
  vector_ref_t (*erase_back)(vector_t* self, vector_ref_t pos);
 
  vector_t* (*swap)(vector_t* self, vector_t* other);
} vector_z;

/* class instances, but without built-in Refs... */
extern const vector_z Vector;

/* TODO: add io */


/* small macros */
#define vector_empty(self) \
  (assert(self), (self)->beg == (self)->end)

#define vector_size(self) \
  (assert(self), ((self)->end - (self)->beg) / (self)->Object->_sizeof_)

#define vector_capacity(self) \
  (assert(self), ((self)->mem_end - (self)->beg) / (self)->Object->_sizeof_)

#define vector_front(self) \
  (assert(! vector_empty(self)), (self)->beg)

#define vector_back(self) \
  (assert(! vector_empty(self)), (self)->end - (self)->Object->_sizeof_)

#define vector_begin(self) \
  (assert(self), (vector_ref_t)(self)->beg)

#define vector_end(self) \
  (assert(self), (vector_ref_t)(self)->end)

#define vector_rbegin(self) \
  (assert(self), (vector_ref_t)((self)->end - (self)->object->_sizeof_))

#define vector_rend(self) \
  (assert(self), (vector_ref_t)((self)->beg - (self)->object->_sizeof_))


#endif  /* YACL_VECTOR_H */
