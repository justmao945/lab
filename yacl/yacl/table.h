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
 
#ifndef YACL_TABLE_H
#define YACL_TABLE_H

#include <yacl/alloc.h>

/**
 * The end of ref is `NULL`.
 */
typedef struct __table_ref_t  *table_ref_t;
struct __table_ref_t{
  table_ref_t next;
  char value[];
};

/* table reference class */
typedef def_ref_z(table_ref_t, void) table_ref_z;


/* hash table type(implemented in separate chainning) */
#undef  def_table_t
#define def_table_t \
struct { \
  const alloc_z *Alloc; \
  alloc_t *alloc; \
  table_ref_t *bkt; \
  size_t bkt_size; \
  const objcmp_z *Key; \
  hash_f *key_hash; \
  cmp_f *key_eq; \
  size_t size; \
  float  max_load_factor; \
}
typedef def_table_t table_t;


/* hash table class type */
typedef struct{
  /**
   * table type defs
   * If `key_hash` is `NULL` then uses `Key->_hash_`, or uses `key_hash`.
   * If `key_eq` is `NULL` then uses `Key->_eq_`, or uses `key_eq`.
   */
  def_object_z(table_t, table_t* self, const objcmp_z* Key, hash_f* key_hash, cmp_f* key_eq);
 
  table_ref_t (*begin)(const table_t* self, size_t bucket_ix);
 
  table_ref_t (*end)(const table_t* self, size_t bucket_ix);
 
  bool    (*empty)(const table_t* self);
 
  size_t  (*size)(const table_t* self);
 
  table_t*  (*clear)(table_t* self);
 
  table_ref_t (*insert)(table_t* self, const objcmp_t* key);
 
  table_ref_t (*insert_n)(table_t* self, size_t n, const objcmp_t* keys);
 
  table_ref_t (*insert_r)(table_t* self, const ref_z* Ref, ref_t first, ref_t last);
 
  table_ref_t (*erase)(table_t* self, const objcmp_t* key);
 
  table_ref_t (*find)(const table_t* self, const objcmp_t* key);
 
  size_t (*bucket)(const table_t* self, const objcmp_t* key);
 
  size_t (*bucket_count)(const table_t* self);
 
  float (*load_factor)(const table_t* self);
 
  float (*max_load_factor)(table_t* self, float factor);
 
  table_t* (*rehash)(table_t* self, size_t n_buckets);
 
  table_t* (*reserve)(table_t* self, size_t n_elements);
 
  table_t* (*swap)(table_t* self, table_t* other);
 
  table_t* (*merge)(table_t* self, table_t* other);
} table_z;

/* class instance. */
extern const table_z Table;
extern const table_ref_z TableRef;

/* TODO: io */


/* small macros */
#define table_empty(self) \
  (assert(self), (self)->size == 0)

#define table_size(self) \
  (assert(self), (self)->size)

#define table_bucket_count(self) \
  (assert(self), (self)->bkt_size)

#define table_load_factor(self) \
  (assert(self), (self)->bkt_size == 0 ? 0.f : (float)(self)->size / (self)->bkt_size)

/* max_load_factor: (float) */
#define table_max_load_factor(self, max_load_factor) \
  (assert(self), max_load_factor <= 0.f ? \
  (self)->max_load_factor : \
  (self)->max_load_factor = max_load_factor)

/* key: (const objcmp_t*) */
#define table_bucket(self, key) \
  (assert(self), assert(key), \
  (self)->bkt_size == 0 ? -1 : (self)->key_hash(key) % (self)->bkt_size)


#endif /* YACL_TABLE_H */
