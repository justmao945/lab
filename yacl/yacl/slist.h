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

#ifndef YACL_SLIST_H
#define YACL_SLIST_H

#include <yacl/alloc.h>

typedef struct __slist_ref_t* slist_ref_t;
struct __slist_ref_t
{
  slist_ref_t next;
  char value[];
};

typedef def_ref_z(slist_ref_t, void) slist_ref_z;


/* slist type. `end` is `NULL`. */
#undef  def_slist_t
#define def_slist_t \
struct { \
  const object_z *Object; \
  slist_ref_t bbeg; \
  const alloc_z *Alloc; \
  alloc_t *alloc; \
}
typedef def_slist_t slist_t;

/* slist class */
typedef struct {
  def_object_z(slist_t, slist_t* self, const object_z* Object);
 
  object_t* (*get)(const slist_t* self, size_t i);
 
  slist_t* (*set)(slist_t* self, size_t i, const object_t* value);
 
  object_t* (*front)(const slist_t* self);
 
  slist_ref_t (*before_begin)(const slist_t* self);
 
  slist_ref_t (*begin)(const slist_t* self);
 
  slist_ref_t (*end)(const slist_t* self);
 
  slist_ref_t (*find_next)(const slist_t* self, cmp_f *eq, const object_t* data);
 
  bool    (*empty)(const slist_t* self);
 
  size_t  (*size)(const slist_t* self);
 
  slist_t* (*clear)(slist_t* self);
 
  slist_ref_t (*insert_after)(slist_t* self, slist_ref_t pos, const object_t* value);
 
  slist_ref_t (*insert_after_n)(slist_t* self, slist_ref_t pos,
              size_t n, const object_t* data);
 
  slist_ref_t (*insert_after_r)(slist_t* self, slist_ref_t pos,
              const ref_z* Ref, ref_t first, ref_t last);
 
  slist_ref_t (*erase_after)(slist_t* self, slist_ref_t pos);
 
  slist_ref_t (*erase_after_r)(slist_t* self, slist_ref_t first, slist_ref_t last);
 
  slist_t* (*push_front)(slist_t* self, const object_t* value);
 
  slist_t* (*pop_front)(slist_t* self);
 
  slist_t* (*resize)(slist_t* self, size_t n, const object_t* value);
 
  slist_t* (*merge)(slist_t* self, slist_t* other, cmp_f* lt);
 
  slist_t* (*splice_after)(slist_t* self, slist_ref_t pos, slist_t* other);
 
  slist_t* (*splice_after_at)(slist_t* self, slist_ref_t pos,
            slist_t* other, slist_ref_t it);
 
  slist_t* (*splice_after_r)(slist_t* self, slist_ref_t pos,
            slist_t* other, slist_ref_t first, slist_ref_t last);
 
  slist_t* (*remove)(slist_t* self, cmp_f* pred, const object_t* value);
 
  slist_t* (*reverse)(slist_t* self);
 
  slist_t* (*unique)(slist_t* self, cmp_f* pred);
 
  slist_t* (*sort)(slist_t* self, cmp_f* lt);
 
  slist_t* (*swap)(slist_t* self, slist_t* other);
} slist_z;

/* class instances */
extern const slist_ref_z SListRef;
extern const slist_z SList;

/* TODO: add io */


/* small macros */
#define slist_empty(self) \
  (assert(self), (self)->bbeg->next == NULL)

#define slist_front(self) \
  (assert(! list_empty(self)), (self)->bbeg->next->value)

#define slist_before_begin(self) \
  (assert(self), (list_ref_t)(self)->bbeg)

#define slist_begin(self) \
  (assert(self), (list_ref_t)(self)->bbeg->next)

#define slist_end(self) NULL

#endif /* YACL_SLIST_H */

