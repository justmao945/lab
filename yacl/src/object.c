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

#include <string.h>
#include <stdlib.h>
#include <yacl/object.h>
#include <yacl/utils.h>
#include <yacl/hash.h>


#undef  impl
#define impl(__, object_t) \
\
static object_t* __##_initcp_(object_t* self, const object_t* other) \
  { assert(self); assert(other); *self = *(object_t*)other; return self; } \
\
static bool __##_lt_(const object_t* self, const object_t* other) \
  { assert(self); assert(other); return *self < *other; } \
\
static bool __##_le_(const object_t* self, const object_t* other) \
  { assert(self); assert(other); return *self <= *other; } \
\
static bool __##_eq_(const object_t* self, const object_t* other) \
  { assert(self); assert(other); return *self == *other; } \
\
static bool __##_ne_(const object_t* self, const object_t* other) \
  { assert(self); assert(other); return *self != *other; } \
\
static bool __##_gt_(const object_t* self, const object_t* other) \
  { assert(self); assert(other); return *self > *other; } \
\
static bool __##_ge_(const object_t* self, const object_t* other) \
  { assert(self); assert(other); return *self >= *other; } \


/* FIXME: long long overflow ? */
#undef  hash_int
#define hash_int(__, object_t) \
static size_t __##_hash_(const object_t* self) { assert(self); return (size_t)*self; }


/* FIXME: long double overflow ? */
#undef  hash_float
#define hash_float(__, object_t) \
static size_t __##_hash_(const object_t* self) \
{ \
  typedef union{ size_t u; object_t f; } uf_t;\
  assert(self); return ((uf_t*)self)->u; \
}


#undef  decl
#define decl(object_z, Object, __, object_t) \
  const object_z Object = \
{  \
  ._sizeof_ = sizeof(object_t), \
  ._init_   = NULL, \
  ._initcp_ = __##_initcp_, \
  ._del_    = NULL, \
  ._hash_   = __##_hash_, \
  ._lt_ = __##_lt_, \
  ._le_ = __##_le_, \
  ._eq_ = __##_eq_, \
  ._ne_ = __##_ne_, \
  ._gt_ = __##_gt_, \
  ._ge_ = __##_ge_, \
};


impl(_ptr, ptr_t)
hash_int(_ptr, ptr_t)
decl(ptr_z, Ptr, _ptr, ptr_t)

impl(_ptrdiff, ptrdiff_t)
hash_int(_ptrdiff, ptrdiff_t)
decl(ptrdiff_z, Ptrdiff, _ptrdiff, ptrdiff_t)

impl(_size, size_t)
hash_int(_size, size_t)
decl(size_z, Size, _size, size_t)

impl(_char, char)
hash_int(_char, char)
decl(char_z, Char, _char, char)

impl(_schar, signed char)
hash_int(_schar, signed char)
decl(schar_z, SChar, _schar, signed char)

impl(_uchar, unsigned char)
hash_int(_uchar, unsigned char)
decl(uchar_z, UChar, _uchar, unsigned char)

impl(_short, short)
hash_int(_short, short)
decl(short_z, Short, _short, short)

impl(_ushort, unsigned short)
hash_int(_ushort, unsigned short)
decl(ushort_z, UShort, _ushort, unsigned short)

impl(_int, int)
hash_int(_int, int)
decl(int_z, Int, _int, int)

impl(_uint, unsigned int)
hash_int(_uint, unsigned int)
decl(uint_z, UInt, _uint, unsigned int)

impl(_long, long)
hash_int(_long, long)
decl(long_z, Long, _long, long)

impl(_ulong, unsigned long)
hash_int(_ulong, unsigned long)
decl(ulong_z, ULong, _ulong, unsigned long)

#ifdef YACL_LONGLONG
impl(_llong, long long)
hash_int(_llong, long long)
decl(llong_z, LLong, _llong, long long)

impl(_ullong, unsigned long long)
hash_int(_ullong, unsigned long long)
decl(ullong_z, ULLong, _ullong, unsigned long long)
#endif /* YACL_LONGLONG */

impl(_float, float)
hash_float(_float, float)
decl(float_z, Float, _float, float)

impl(_double, double)
hash_float(_double, double)
decl(double_z, Double, _double, double)


/* builtin string */
static cstring_t* _str_initcp_(cstring_t* self, const cstring_t* other)
{
  assert(self);
  assert(other);
  *self = *other;
  return self;
}

static size_t _str_hash_(const cstring_t* self)
{
  assert(self);
  return hash_murmur2(*self, strlen(*self));
}

static bool _str_lt_(const cstring_t* self, const cstring_t* other)
{
  assert(self);
  assert(other);
  return strcmp(*self, *other) < 0;
}

static bool _str_le_(const cstring_t* self, const cstring_t* other)
{
  assert(self);
  assert(other);
  return strcmp(*self, *other) <= 0;
}

static bool _str_eq_(const cstring_t* self, const cstring_t* other)
{
  assert(self);
  assert(other);
  return strcmp(*self, *other) == 0;
}

static bool _str_ne_(const cstring_t* self, const cstring_t* other)
{
  assert(self);
  assert(other);
  return strcmp(*self, *other) != 0;
}

static bool _str_gt_(const cstring_t* self, const cstring_t* other)
{
  assert(self);
  assert(other);
  return strcmp(*self, *other) > 0;
}

static bool _str_ge_(const cstring_t* self, const cstring_t* other)
{
  assert(self);
  assert(other);
  return strcmp(*self, *other) >= 0;
}

static size_t _str_size(const char* cstr)
{
  assert(cstr);
  return strlen(cstr);
}

static char* _str_concat(char* cstr, const char* other)
{
  assert(cstr);
  assert(other);
  return strcat(cstr, other);
}

static char* _str_concat_n(char* cstr, const char* other, size_t n)
{
  assert(cstr);
  assert(other);
  return strncat(cstr, other, n);
}

static char* _str_copy(char* cstr, const char* other)
{
  assert(cstr);
  assert(other);
  return strcpy(cstr, other);
}

static char* _str_copy_n(char* cstr, const char* other, size_t n)
{
  assert(cstr);
  assert(other);
  return strncpy(cstr, other, n);
}

static char* _str_find(const char* cstr, int c)
{
  assert(cstr);
  return strchr(cstr, c);
}

static char* _str_rfind(const char* cstr, int c)
{
  assert(cstr);
  return strrchr(cstr, c);
}

static char* _str_find_sub(const char* cstr, const char* sub)
{
  assert(cstr);
  assert(sub);
  return strstr(cstr, sub);
}

static char* _str_first_in(const char* cstr, const char* set)
{
  assert(cstr);
  assert(set);
  return strpbrk(cstr, set);
}

static char* _str_first_not_in(const char* cstr, const char* set)
{
  assert(cstr);
  assert(set);
  return (char*)cstr + strspn(cstr, set);
}

static size_t _str_match_set(const char* cstr, const char* set)
{
  assert(cstr);
  assert(set);
  return strspn(cstr, set);
}

static int _str_to_int(const char* cstr)
{
  assert(cstr);
  return atoi(cstr);
}

static long _str_to_long(const char* cstr)
{
  assert(cstr);
  return atol(cstr);
}

static long _str_to_long0(const char* cstr, char** err, int base)
{
  assert(cstr);
  return strtol(cstr, err, base);
}

static unsigned long _str_to_ulong0(const char* cstr, char** err, int base)
{
  assert(cstr);
  return strtoul(cstr, err, base);
}

static long long _str_to_llong(const char* cstr)
{
  assert(cstr);
  return atoll(cstr);
}

static long long _str_to_llong0(const char* cstr, char** err, int base)
{
  assert(cstr);
  return strtoll(cstr, err, base);
}

static unsigned long long _str_to_ullong0(const char* cstr, char** err, int base)
{
  assert(cstr);
  return strtoull(cstr, err, base);
}

static double _str_to_double(const char* cstr)
{
  assert(cstr);
  return atof(cstr);
}

static float _str_to_float0(const char* cstr, char** err)
{
  assert(cstr);
  return strtof(cstr, err);
}

static double _str_to_double0(const char* cstr, char** err)
{
  assert(cstr);
  return strtod(cstr, err);
}


const cstring_z CString =
{
  ._sizeof_ = sizeof(cstring_t),
  ._init_   = NULL,
  ._initcp_ = _str_initcp_,
  ._del_    = NULL,
  ._hash_   = _str_hash_,
  ._lt_     = _str_lt_,
  ._le_     = _str_le_,
  ._eq_     = _str_eq_,
  ._ne_     = _str_ne_,
  ._gt_     = _str_gt_,
  ._ge_     = _str_ge_,
  .size     = _str_size,
  .concat   = _str_concat,
  .concat_n = _str_concat_n,
  .copy     = _str_copy,
  .copy_n   = _str_copy_n,
  .find     = _str_find,
  .rfind    = _str_rfind,
  .find_sub = _str_find_sub,
  .first_in = _str_first_in,
  .first_not_in = _str_first_not_in,
  .match_set  = _str_match_set,
  .to_int     = _str_to_int,
  .to_long    = _str_to_long,
  .to_long0   = _str_to_long0,
  .to_ulong0  = _str_to_ulong0,
  .to_llong   = _str_to_llong,
  .to_llong0  = _str_to_llong0,
  .to_ullong0 = _str_to_ullong0,
  .to_double  = _str_to_double,
  .to_float0  = _str_to_float0,
  .to_double0 = _str_to_double0,
};


