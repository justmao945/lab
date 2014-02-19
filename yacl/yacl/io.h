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

#ifndef YACL_IO_H
#define YACL_IO_H

#include <stdio.h>
#include <yacl/object.h>

typedef void io_t;

typedef struct{
  def_object_z(io_t, io_t *self);
  int (*read)(object_t* out, io_t* in, const void* Sub);
  int (*write)(const object_t* in, io_t* out, const void* Sub);
}io_z;

/*
 * io class.
 * FIXME: ...recursively definition. (x.x)
 */
#undef  def_io_z
#define def_io_z(io_t, object_t, ...) \
struct { \
  def_object_z(io_t, __VA_ARGS__); \
  \
  int (*read)(object_t* out, io_t* in, const io_z* Sub); \
  \
  int (*write)(const object_t* in, io_t* out, const io_z* Sub); \
}

#undef  def_stdio_t
#define def_stdio_t struct{ FILE* std; }  /* make the stdio in the first slot. */

/**
 * Set `FILE` to `stdio_t`.
 * Convert file descriptor(`int`) to file stream(`FILE*`):
 *   `FILE* fdopen(int fd, const char* mode)`
 * or reverse:
 *   `int fileno(FILE *stream)`
 */
typedef FILE stdio_t;

/* built-in io class. */
#undef  def_bltin_io_z
#define def_bltin_io_z(io_t, object_t, ...) \
struct { \
  def_object_z(io_t, __VA_ARGS__); \
  \
  int (*read)(object_t* out, io_t* in); \
  \
  int (*write)(const object_t* in, io_t* out); \
}


/** 
 * builtin io classes.
 *
 * Prototype of built-in IOs.
 * ```
   int (*read)(type* out, stdio_t* in); 
   int (*write)(const type* in, stdio_t* out); 
 * ```
 */

typedef def_bltin_io_z(stdio_t, short, stdio_t* self) io_short_z;

typedef def_bltin_io_z(stdio_t, unsigned short, stdio_t* self) io_ushort_z;

typedef def_bltin_io_z(stdio_t, int, stdio_t* self) io_int_z;

typedef def_bltin_io_z(stdio_t, unsigned int, stdio_t* self) io_uint_z;

typedef def_bltin_io_z(stdio_t, long, stdio_t* self) io_long_z;

typedef def_bltin_io_z(stdio_t, unsigned long, stdio_t* self) io_ulong_z;

typedef def_bltin_io_z(stdio_t, long long, stdio_t* self) io_llong_z;

typedef def_bltin_io_z(stdio_t, unsigned long long, stdio_t* self) io_ullong_z;

typedef def_bltin_io_z(stdio_t, ref_t, stdio_t* self) io_ref_z;

typedef def_bltin_io_z(stdio_t, ptr_t, stdio_t* self) io_ptr_z;

typedef def_bltin_io_z(stdio_t, ptrdiff_t, stdio_t* self) io_ptrdiff_z;

typedef def_bltin_io_z(stdio_t, size_t, stdio_t* self) io_size_z;

typedef def_bltin_io_z(stdio_t, char, stdio_t* self) io_char_z;

typedef def_bltin_io_z(stdio_t, float, stdio_t* self) io_float_z;

typedef def_bltin_io_z(stdio_t, double, stdio_t* self) io_double_z;

typedef def_bltin_io_z(stdio_t, cstring_t, stdio_t* self) io_cstring_z;

typedef struct{
  const io_short_z  Short;
  const io_ushort_z UShort;
  const io_int_z    Int;
  const io_uint_z   UInt;
  const io_long_z   Long;
  const io_ulong_z  ULong;
  const io_llong_z  LLong;
  const io_ullong_z ULLong;
}base_z;

typedef struct{
  const base_z Oct;
  const base_z Dec;
  const base_z Hex;
  const io_ref_z      Ref;  /* for `ref_t` */
  const io_ptr_z      Ptr;
  const io_ptrdiff_z  Ptrdiff;
  const io_size_z     Size;
  const io_char_z     Char;
  const io_float_z    Float;
  const io_double_z   Double;
  const io_cstring_z  CString;
}stdio_z;

extern const stdio_z Stdio;

#endif  /* - YACL_IO_H - */
