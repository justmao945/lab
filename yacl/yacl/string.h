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


#ifndef YACL_STRING_H
#define YACL_STRING_H

#include <yacl/alloc.h>

typedef char* string_ref_t;

/* string reference class */
typedef struct{
  def_ref_z(string_ref_t, void);
  string_ref_t (*prev)(string_ref_t ref);
}string_ref_z;


/* string type */
#undef  def_string_t
#define def_string_t \
struct { \
  char *beg; \
  char *end; \
  char *mem_end; \
  const alloc_z *Alloc; \
  alloc_t *alloc; \
}
typedef def_string_t string_t;

/* string class */
typedef struct {
  def_objcmp_z(string_t, string_t *self);
 
  int (*get)(const string_t* self, ptrdiff_t i);
 
  string_t* (*set)(string_t* self, ptrdiff_t i, int value);
 
  char (*front)(const string_t* self);
 
  char (*back)(const string_t* self);
 
  string_ref_t (*begin)(const string_t* self);
 
  string_ref_t (*end)(const string_t* self);
 
  string_ref_t (*rbegin)(const string_t* self);
 
  string_ref_t (*rend)(const string_t* self);
 
  string_ref_t (*find)(const string_t* self, cmp_f *eq, int data);
 
  bool   (*empty)(const string_t* self);
 
  union {
    size_t (*size)(const string_t* self);
   
    size_t (*length)(const string_t* self);
  }; 
 
  size_t (*capacity)(const string_t* self);
 
  string_t* (*reserve)(string_t* self, size_t n);
 
  string_t* (*shrink)(string_t* self);
 
  string_t* (*clear)(string_t* self);
 
  string_t* (*push_back)(string_t* self, int value);
 
  string_t* (*pop_back)(string_t* self);
 
  string_t* (*resize)(string_t* self, size_t n, int value);
 
  string_t* (*reverse)(string_t* self);
 
  string_t* (*append)(string_t* self, const string_t* other);
 
  string_t* (*append_n)(string_t* self, size_t n, const char* data);
 
  string_t* (*append_r)(string_t* self, const ref_z* Ref, ref_t first, ref_t last);
 
  string_ref_t (*erase_back)(string_t* self, string_ref_t pos);
 
  string_t* (*swap)(string_t* self, string_t* other);
 
  char* (*c_str)(const string_t* self);
} string_z;


/* class instances */
extern const string_ref_z StringRef;
extern const string_ref_z StringRefR;
extern const string_z String;

/* TODO: add io */

#endif  /* YACL_STRING_H */
