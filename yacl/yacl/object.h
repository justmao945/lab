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

#ifndef YACL_OBJECT_H
#define YACL_OBJECT_H

#include <assert.h>
#include <stddef.h>
#include <stdbool.h>
#include <yacl/__conf.h>

/**
 * Object class.
 * `_sizeof_` represents the byte size of its type(must greater than 0).
 * `_init_` will *never* be used by YACL(can be `NULL`).
 * `_initcp_` is the copy constructor, and used in containers(can't be `NULL`).
 * `_del_` is the destructor, and used in containers, too(can be `NULL`).
 */
#undef  def_object_z
#define def_object_z(object_t, ...) \
struct { \
  size_t _sizeof_;  \
  \
  object_t* (*_init_)(__VA_ARGS__); \
  \
  object_t* (*_initcp_)(object_t* self, const object_t* other); \
  \
  object_t* (*_del_)(object_t* self); \
}

/* object type */
typedef void object_t;
typedef def_object_z(object_t, void) object_z;


/* object instance helper */
object_t* make(object_z Object, ...); /* dummy func for auto-complete? */
#undef  make
#define make(Object, ...)  (Object)._init_(__VA_ARGS__)

void    nmake(size_t n, object_z Object, ...);
#undef  nmake /* FIXME: strange style... */
#define nmake(n, Object, ...) \
  { size_t i; for(i=0; i<n; i++) (Object)._init_(__VA_ARGS__); }

object_t* make_copy(object_z Object, object_t* self, const object_t* other);
#undef  make_copy
#define make_copy(Object, self, other) (Object)._initcp_(self, other)

object_t* clean(object_z Object, object_t* self);
#undef  clean
#define clean(Object, self) (Object)._del_(self)

void    nclean(size_t n, object_z Object, object_t* i_self);
#undef  nclean /* FIXME: strange style... */
#define nclean(n, Object, self) \
  { size_t i; for(i=0; i<n; i++) (Object)._del_(self); }


/* Object reference class */
#undef  def_ref_z
#define def_ref_z(ref_t, ...) \
struct { \
  def_object_z(ref_t, __VA_ARGS__);  \
  \
  object_t* (*value)(ref_t ref);\
  \
  ref_t (*next)(ref_t ref); \
  \
  ref_t (*next_n)(ref_t ref, ptrdiff_t n); \
  \
  ptrdiff_t (*dist)(ref_t first, ref_t last); \
}

/* reference */
typedef object_t* ref_t;
typedef def_ref_z(ref_t, void) ref_z;


/* func types */
typedef bool(cmp_f)(const object_t*, const object_t*);
typedef size_t(hash_f)(const object_t*);

/* comparable class */
#undef  def_objcmp_z
#define def_objcmp_z(object_t, ...) \
struct { \
  def_object_z(object_t, __VA_ARGS__); \
  size_t (*_hash_)(const object_t* self); \
  bool (*_lt_)(const object_t* self, const object_t* other); \
  bool (*_le_)(const object_t* self, const object_t* other); \
  bool (*_eq_)(const object_t* self, const object_t* other); \
  bool (*_ne_)(const object_t* self, const object_t* other); \
  bool (*_gt_)(const object_t* self, const object_t* other); \
  bool (*_ge_)(const object_t* self, const object_t* other); \
}

/* comparable object */
typedef object_t objcmp_t;
typedef def_objcmp_z(objcmp_t, void) objcmp_z;


/* -- built in types --*/

typedef void* ptr_t;

typedef def_objcmp_z(ptr_t, ptr_t* self) ptr_z;
extern const ptr_z Ptr;  /* void* */

typedef def_objcmp_z(ptrdiff_t, ptrdiff_t* self) ptrdiff_z;
extern const ptrdiff_z Ptrdiff;  /* ptrdiff_t */

typedef def_objcmp_z(size_t, size_t* self) size_z;
extern const size_z Size;  /* size_t */

typedef def_objcmp_z(char, char* self) char_z;
extern const char_z Char;  /* char */

typedef def_objcmp_z(signed char, signed char* self) schar_z;
extern const schar_z SChar; /* signed char */

typedef def_objcmp_z(unsigned char, unsigned char* self) uchar_z;
extern const uchar_z UChar; /* unsigned char */

typedef def_objcmp_z(short, short* self) short_z;
extern const short_z Short; /* short */

typedef def_objcmp_z(unsigned short, unsigned short* self) ushort_z;
extern const ushort_z UShort; /* unsigned short */

typedef def_objcmp_z(int, int* self) int_z;
extern const int_z Int; /* int */

typedef def_objcmp_z(unsigned int, unsigned int* self) uint_z;
extern const uint_z UInt; /* unsigned int */

typedef def_objcmp_z(long, long* self) long_z;
extern const long_z Long; /* long */

typedef def_objcmp_z(unsigned long, unsigned long* self) ulong_z;
extern const ulong_z ULong; /* unsigned long */

typedef def_objcmp_z(float, float* self) float_z;
extern const float_z Float; /* float */

typedef def_objcmp_z(double, double* self) double_z;
extern const double_z Double; /* double */

#ifdef YACL_LONGLONG
typedef def_objcmp_z(long long, long long* self) llong_z;
extern const llong_z LLong; /* long long */

typedef def_objcmp_z(unsigned long long, unsigned long long* self) ullong_z;
extern const ullong_z ULLong; /* unsigned long long */
#endif /* YACL_LONGLONG */


/* C string type */
typedef char* cstring_t;

/* C string class */
typedef struct{
  def_objcmp_z(cstring_t, void);
  size_t (*size)(const char* cstr); /* strlen */
  char*  (*concat)(char* cstr, const char* other); /* strcat */
  char*  (*concat_n)(char* cstr, const char* other, size_t n);  /* strncat */
  char*  (*copy)(char* cstr, const char* other);  /* strcpy */
  char*  (*copy_n)(char* cstr, const char* other, size_t n);  /* strncpy */
  char*  (*find)(const char* cstr, int c); /* strchr */
  char*  (*rfind)(const char* cstr, int c); /* strrchr */
  char*  (*find_sub)(const char* cstr, const char* sub); /* strstr */

  /**
   * Find `cstr` for the first occurrence of a char is in `set`. 
   * Returns reference (NULL for not found).
   */
  char*  (*first_in)(const char* cstr, const char* set); /* strpbrk */

  /** 
   * Find `cstr` for the first absence of a char is in `set`.
   * Returns reference (NULL for not found).
   */
  char* (*first_not_in)(const char* cstr, const char* set); /* strspn */

  /**
   * Return the length of the initial portion of `cstr` containing chars in `set`.
   * Used as first not in ??
   */
  size_t (*match_set)(const char* cstr, const char* set); /* strspn */

  int    (*to_int)(const char* cstr); /* atoi */

  long   (*to_long)(const char* cstr);  /* atol */
  long   (*to_long0)(const char* cstr, char** err, int base);  /* strtol */
  unsigned long (*to_ulong0)(const char* cstr, char** err, int base);  /* strtoul */

  long long (*to_llong)(const char* cstr); /* atoll */
  long long (*to_llong0)(const char* cstr, char** err, int base); /* strtoll */
  unsigned long long (*to_ullong0)(const char* cstr, char** err, int base); /* strtoull */

  double (*to_double)(const char* cstr);
  float  (*to_float0)(const char* cstr, char** err);  /* strtof */
  double (*to_double0)(const char* cstr, char** err);  /* strtod */
} cstring_z;

/* c string class instance */
extern const cstring_z CString;

#endif /* YACL_OBJECT_H  */
