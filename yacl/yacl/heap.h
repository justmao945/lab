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


#ifndef YACL_HEAP_H
#define YACL_HEAP_H

#include <yacl/alloc.h>
#include <yacl/vector.h>

/* heap type(based on vector) */
#undef  def_heap_t
#define def_heap_t \
struct { \
  def_vector_t; \
  cmp_f* lt; \
}
typedef def_heap_t heap_t;

/**
 * max-heap class.
 * Default uses `_lt_` --> max-heap,  or custom to `_gt_` --> min-heap.
 */
typedef struct{
  /**
   * If `lt` is `NULL` then uses `Objcmp->_lt_`, or uses `lt`.
   */
  def_object_z(heap_t, heap_t* self, const objcmp_z* Objcmp, cmp_f* lt);
  
  objcmp_t* (*top)(const heap_t* self);
  
  bool    (*empty)(const heap_t* self);
  
  size_t  (*size)(const heap_t* self);
  
  size_t  (*capacity)(const heap_t* self);
  
  heap_t* (*reserve)(heap_t* self, size_t n); 
  
  heap_t* (*shrink)(heap_t* self); 
  
  heap_t* (*clear)(heap_t* self); 
  
  heap_t* (*push)(heap_t* self, const objcmp_t* value); 
  
  heap_t* (*merge)(heap_t* self, const heap_t* other); 
  
  heap_t* (*push_n)(heap_t* self, size_t n, const objcmp_t* data); 
  
  heap_t* (*push_r)(heap_t* self, const ref_z* Ref, ref_t first, ref_t last); 
  
  heap_t* (*pop)(heap_t* self); 
  
  heap_t* (*pop_n)(heap_t* self, size_t n); 
  
  heap_t* (*swap)(heap_t* self, heap_t* other); 
} heap_z;

/* heap class instance. */
extern const heap_z Heap;

/* small macros */
#define heap_empty(self) \
  (assert(self), (self)->beg == (self)->end)

#define heap_size(self) \
  (assert(self), ((self)->end - (self)->beg) / (self)->Object->_sizeof_)

#define heap_capacity(self) \
  (assert(self), ((self)->mem_end - (self)->beg) / (self)->Object->_sizeof_)

#define heap_top(self) \
  (assert(! heap_empty(self)), (self)->beg)


#ifdef YACL_CXX
extern "C" {
#endif

/**
 * Checks if the elements in range `[first, last)` are a heap.
 * Default uses `Objcmp->_lt_` to compare when `lt` is `NULL`, or uses `lt`.
 *
 * @param first,last Range to examine. 
 * @param Objcmp Comparable class type pointer.
 * @param lt `NULL` to ignore this, or uses this to compare values.
 * @return `true` if is heap or `false`.
 */
bool is_heap(const objcmp_t* first, const objcmp_t* last,
     const objcmp_z* Objcmp, cmp_f* lt);

/**
 * Examines the range `[first, last)` and finds the largest range beginning at
 * `first` which is heap. Default uses `Objcmp->_lt_` to compare when `lt` is
 * `NULL`, or uses `lt`.
 *
 * @param first,last Range to examine.
 * @param Objcmp Comparable class type pointer.
 * @param lt `NULL` to ignore this, or uses this to compare values.
 * @return The one follow the last element of heap.
 */
objcmp_t* is_heap_until(const objcmp_t* first, const objcmp_t* last,
          const objcmp_z* Objcmp, cmp_f* lt);

/**
 * Constructs a heap in the range `[first, last)`.
 * Default uses `Objcmp->_lt_` to compare when `lt` is `NULL`, or uses `lt`.
 *
 * @param first,last Range to construct.
 * @param Objcmp Comparable class type pointer.
 * @param lt `NULL` to ignore this, or uses this to compare values.
 * @return
 */
void make_heap(objcmp_t* first, objcmp_t* last, const objcmp_z* Objcmp, cmp_f* lt);

/**
 * Inserts the element at the position `last - Objcmp->_sizeof_` into the 
 * heap defined by the range `[first, last - Objcmp->_sizeof_)`.
 * Default uses `Objcmp->_lt_` to compare when `lt` is `NULL`, or uses `lt`.
 *
 * @param first,last Range to determine
 * @param Objcmp Comparable class type pointer.
 * @param lt `NULL` to ignore this, or uses this to compare values.
 */
void push_heap(objcmp_t* first, objcmp_t* last, const objcmp_z* Objcmp, cmp_f* lt);

/**
 * Default uses `Objcmp->_lt_` to compare when `lt` is `NULL`, or uses `lt`.
 * @param first,last Range to determine
 * @param Objcmp Comparable class type pointer.
 * @param lt `NULL` to ignore this, or uses this to compare values.
 * @return
 * @bug Passes `_le_` and `_ge_` to `lt` will cause bugs when some elements are
 *      equal in the range `[first, last)`.
 */
void pop_heap(objcmp_t* first, objcmp_t* last, const objcmp_z* Objcmp, cmp_f* lt);

/**
 * Swaps the value in the position `first` and the value in the position 
 * `last - Objcmp->_sizeof_` and makes the subrange `[first, last - Objcmp->_sizeof_)`
 * into a heap. Default uses `Objcmp->_lt_` to compare when `lt` is `NULL`, or uses `lt`.
 *
 * @param first,last Range to determine
 * @param Objcmp Comparable class type pointer.
 * @param lt `NULL` to ignore this, or uses this to compare values.
 * @return
 * @bug Passes `_le_` and `_ge_` to `lt` will cause bugs when some elements are
 *      equal in the range `[first, last)`.
 */
void sort_heap(objcmp_t* first, objcmp_t* last, const objcmp_z* Objcmp, cmp_f* lt);

#ifdef YACL_CXX
}
#endif


#endif  /* YACL_HEAP_H */
