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
 
#ifndef YACL_ALLOC_H
#define YACL_ALLOC_H

#include <yacl/object.h>

#undef  def_alloc_z
#define def_alloc_z(alloc_t, ...) \
struct { \
  def_object_z(alloc_t, __VA_ARGS__); \
  \
  void* (*malloc)(alloc_t* self, size_t size); \
  \
  void* (*calloc)(alloc_t* self, size_t num, size_t size); \
  \
  void* (*realloc)(alloc_t* self, void* ptr, size_t new_size); \
  \
  void  (*free)(alloc_t* self, void* ptr); \
}

#undef  def_alloc_t
#define def_alloc_t struct{}

typedef def_alloc_t alloc_t;
typedef def_alloc_z(alloc_t, alloc_t* self) alloc_z;

/**
 * Default allocator instance.
 * FIXME: More efficient memory pool? (replace the one in the standard C library)
 *
 * `malloc` is performed by `malloc` @file stdlib.h
 * `calloc` is performed by `calloc` @file stdlib.h
 * `realloc` is performed by `realloc` @file stdlib.h
 * `free` is performed by `free` @file stdlib.h
 */
extern const alloc_z  Alloc;
extern alloc_t _alloc_;


#endif /* YACL_ALLOC_H */
