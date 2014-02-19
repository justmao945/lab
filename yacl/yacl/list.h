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

#ifndef YACL_LIST_H
#define YACL_LIST_H

#include <yacl/alloc.h>

/* list reference type */
typedef struct __list_ref_t* list_ref_t;
struct __list_ref_t
{
  list_ref_t prev;
  list_ref_t next;
  char value[];
};

/* list reference class */
typedef struct{
  def_ref_z(list_ref_t, void);
  list_ref_t (*prev)(list_ref_t ref);
}list_ref_z;


/* list type. `bbeg` and `end` are the same node, so the list is linked as a ring. */
#undef  def_list_t
#define def_list_t \
struct { \
  const object_z *Object; \
  union { \
    list_ref_t bbeg; \
    list_ref_t end; \
  }; \
  const alloc_z *Alloc; \
  alloc_t *alloc; \
}
typedef def_list_t list_t;

/* list class */
typedef struct {
  def_object_z(list_t, list_t* self, const object_z* Object);
 
  object_t* (*get)(const list_t* self, ptrdiff_t i);
 
  list_t* (*set)(list_t* self, ptrdiff_t i, const object_t* value);
 
  object_t* (*front)(const list_t* self);
 
  object_t* (*back)(const list_t* self);
 
  list_ref_t (*begin)(const list_t* self);
 
  list_ref_t (*end)(const list_t* self);
 
  list_ref_t (*rbegin)(const list_t* self);
 
  list_ref_t (*rend)(const list_t* self);
 
  list_ref_t (*find)(const list_t* self, cmp_f *eq, const object_t* data);
 
  bool    (*empty)(const list_t* self);
 
  size_t  (*size)(const list_t* self);
 
  list_t* (*clear)(list_t* self);
 
  list_ref_t (*insert)(list_t* self, list_ref_t pos, const object_t* value);
 
  list_ref_t (*insert_n)(list_t* self, list_ref_t pos, size_t n, const object_t* data);
 
  list_ref_t (*insert_r)(list_t* self, list_ref_t pos,
              const ref_z* Ref, ref_t first, ref_t last);
 
  list_ref_t (*erase)(list_t* self, list_ref_t pos);
 
  list_ref_t (*erase_r)(list_t* self, list_ref_t first, list_ref_t last);
 
  list_t* (*push_back)(list_t* self, const object_t* value);
 
  list_t* (*pop_back)(list_t* self);
 
  list_t* (*push_front)(list_t* self, const object_t* value);
 
  list_t* (*pop_front)(list_t* self);
 
  list_t* (*resize)(list_t* self, size_t n, const object_t* value);
 
  list_t* (*merge)(list_t* self, list_t* other, cmp_f* lt);
 
  list_t* (*splice)(list_t* self, list_ref_t pos, list_t* other);
 
  list_t* (*splice_at)(list_t* self, list_ref_t pos, list_t* other, list_ref_t it);
 
  list_t* (*splice_r)(list_t* self, list_ref_t pos,
            list_t* other, list_ref_t first, list_ref_t last);
 
  list_t* (*remove)(list_t* self, cmp_f* pred, const object_t* value);
 
  list_t* (*reverse)(list_t* self);
 
  list_t* (*unique)(list_t* self, cmp_f* pred);
 
  list_t* (*sort)(list_t* self, cmp_f* lt);
 
  list_t* (*swap)(list_t* self, list_t* other);
} list_z;

/* class instances */
extern const list_ref_z ListRef;
extern const list_ref_z ListRefR;
extern const list_z List;


/* list stdio */
#include <yacl/io.h>

/* list io types */
typedef struct{
  def_stdio_t; /* sub io */
  const io_z* SSub;  /* Sub io class of list content(sub of sub) */
}list_io_t;

/* list IO classes.  */
typedef def_io_z(list_io_t, list_t,
    list_io_t* self, stdio_t* sub, const io_z* SSub) list_io_z;

extern const list_io_z ListIO;


/* small macros */
#define list_empty(self) \
  (assert(self), (self)->bbeg->next == (self)->end)

#define list_front(self) \
  (assert(! list_empty(self)), (self)->bbeg->next->value)

#define list_back(self) \
  (assert(! list_empty(self)), (self)->end->prev->value)

#define list_begin(self) \
  (assert(self), (list_ref_t)(self)->bbeg->next)

#define list_end(self) \
  (assert(self), (list_ref_t)(self)->end)

#define list_rbegin(self) \
  (assert(self), (list_ref_t)(self)->end->prev)

#define list_rend(self) \
  (assert(self), (list_ref_t)(self)->bbeg)


#endif /* YACL_LIST_H */

