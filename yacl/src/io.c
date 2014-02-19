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

#include <stdio.h>
#include <yacl/io.h>

/**
 * empty funcs, 
 * FIXME: HowTo impl ???  * The `FILE` struct is transparent to us...
 */
static stdio_t* _init_(stdio_t* self){ return self; }
static stdio_t* _initcp_(stdio_t* self, const stdio_t* other){ return self; }
static stdio_t* _del_(stdio_t* self){return self;}


#undef  impl
#define impl(__, object_t, fmt) \
static int __##_read(object_t* out, stdio_t* in) \
  { assert(out); assert(in); return fscanf(in, fmt, out); } \
\
static int __##_write(const object_t* in, stdio_t* out) \
  { assert(in); assert(out); return fprintf(out, fmt, *in); } \

/* FIXME: `fscanf` and `fprintf` --> `fgetc` and `fputc` ? */

impl(_oct_short, short, "%ho");
impl(_oct_ushort, unsigned short, "%ho");
impl(_oct_int, int, "%o");
impl(_oct_uint, unsigned int, "%o");
impl(_oct_long, long, "%lo");
impl(_oct_ulong, unsigned long, "%lo");
impl(_oct_llong, long long, "%llo");
impl(_oct_ullong, unsigned long long, "%llo");

impl(_dec_short, short, "%hi");
impl(_dec_ushort, unsigned short, "%hu");
impl(_dec_int, int, "%i");
impl(_dec_uint, unsigned int, "%u");
impl(_dec_long, long, "%li");
impl(_dec_ulong, unsigned long, "%lu");
impl(_dec_llong, long long, "%lli");
impl(_dec_ullong, unsigned long long, "%llu");

impl(_hex_short, short, "%hx");
impl(_hex_ushort, unsigned short, "%hx");
impl(_hex_int, int, "%x");
impl(_hex_uint, unsigned int, "%x");
impl(_hex_long, long, "%lx");
impl(_hex_ulong, unsigned long, "%lx");
impl(_hex_llong, long long, "%llx");
impl(_hex_ullong, unsigned long long, "%llx");

impl(_ref, ref_t, "%p");
impl(_ptr, ptr_t, "%p");
impl(_ptrdiff, ptrdiff_t, "%td");
impl(_size, size_t, "%zu");
impl(_char, char, "'%c'");
impl(_float, float, "%g");

/* impl(_double, double, "%.16lg"); */
static int _double_read(double* out, stdio_t* in)
  { assert(out); assert(in); return fscanf(in, "%lg", out); }

static int _double_write(const double* in, stdio_t* out)
  { assert(in); assert(out); return fprintf(out, "%.16lg", *in); }

/* impl(_cstring, cstring_t, "\"%s\""); */
static int _cstring_read(cstring_t* out, stdio_t* in)
{
  int c, peek;
  char *str;

  assert(out);
  assert(in);

  str = *out;
  assert(str);

  while( true ){
    if((c = fgetc(in)) == EOF){
      *str = '\0';
      return EOF;
    } else if (c == '\\'){ /* skip \\\" */
      if( (peek = fgetc(in)) == EOF ){
        *str = '\0';
        return EOF;
      }
    } else if (c == '\"'){
      break;
    }
  } /* find `\"`(not `\\\"`) start of string */

  while( true ){
    if( (c = fgetc(in)) == EOF ) {
      *str = '\0';
      return EOF;
    } else if( c == '\\' ){  /* decode  */
      switch( peek = fgetc(in) ){
        case EOF: *str = '\0'; return EOF;
        case 'b': *str++ = '\b'; break;
        case 'f': *str++ = '\f'; break;
        case 'n': *str++ = '\n'; break;
        case 'r': *str++ = '\r'; break;
        case 't': *str++ = '\t'; break;
        case '\"': case '\\': default: *str++ = peek; break;
      }
    } else if( c == '\"' ){ /* end of string */
      break;
    } else {
      *str++ = c;
    }
  }
  *str = '\0';
  return str - *out;
}

static int 
_cstring_write(const cstring_t* in, stdio_t* out)
{
#define __fputc(chr, out)  {if(fputc(chr, out) == EOF) return EOF;}

  char *str;

  assert(in);
  assert(out);

  str = *in;

  __fputc('\"', out); /* start */
  while( *str != '\0' ){
    switch( *str ){ /* encode */
      case '\"': __fputc('\\', out); __fputc('\"', out); break;
      case '\\': __fputc('\\', out); __fputc('\\', out); break;
      case '\b': __fputc('\\', out); __fputc('b', out); break;
      case '\f': __fputc('\\', out); __fputc('f', out); break;
      case '\n': __fputc('\\', out); __fputc('n', out); break;
      case '\r': __fputc('\\', out); __fputc('r', out); break;
      case '\t': __fputc('\\', out); __fputc('t', out); break;
      default: __fputc(*str, out); break;
    }
    str++;
  }
  __fputc('\"', out); /* end */
  fflush(out);
  return str - *in;
#undef __fputc
}


#undef  obj_init
#define obj_init \
  ._sizeof_ = sizeof(stdio_t),\
  ._init_   = _init_, \
  ._initcp_ = _initcp_, \
  ._del_    = _del_, \


const stdio_z Stdio = 
{
  .Oct = 
    {
      .Short  = { obj_init .read = _oct_short_read,  .write = _oct_short_write },
      .UShort = { obj_init .read = _oct_ushort_read, .write = _oct_ushort_write},
      .Int    = { obj_init .read = _oct_int_read,    .write = _oct_int_write   },
      .UInt   = { obj_init .read = _oct_uint_read,   .write = _oct_uint_write  },
      .Long   = { obj_init .read = _oct_long_read,   .write = _oct_long_write  },
      .ULong  = { obj_init .read = _oct_ulong_read,  .write = _oct_ulong_write },
      .LLong  = { obj_init .read = _oct_llong_read,  .write = _oct_llong_write },
      .ULLong = { obj_init .read = _oct_ullong_read, .write = _oct_ullong_write},
    },
  .Dec = 
    {
      .Short  = { obj_init .read = _dec_short_read,  .write = _dec_short_write },
      .UShort = { obj_init .read = _dec_ushort_read, .write = _dec_ushort_write},
      .Int    = { obj_init .read = _dec_int_read,    .write = _dec_int_write   },
      .UInt   = { obj_init .read = _dec_uint_read,   .write = _dec_uint_write  },
      .Long   = { obj_init .read = _dec_long_read,   .write = _dec_long_write  },
      .ULong  = { obj_init .read = _dec_ulong_read,  .write = _dec_ulong_write },
      .LLong  = { obj_init .read = _dec_llong_read,  .write = _dec_llong_write },
      .ULLong = { obj_init .read = _dec_ullong_read, .write = _dec_ullong_write},
    },
  .Hex = 
    {
      .Short  = { obj_init .read = _hex_short_read,  .write = _hex_short_write },
      .UShort = { obj_init .read = _hex_ushort_read, .write = _hex_ushort_write},
      .Int    = { obj_init .read = _hex_int_read,    .write = _hex_int_write   },
      .UInt   = { obj_init .read = _hex_uint_read,   .write = _hex_uint_write  },
      .Long   = { obj_init .read = _hex_long_read,   .write = _hex_long_write  },
      .ULong  = { obj_init .read = _hex_ulong_read,  .write = _hex_ulong_write },
      .LLong  = { obj_init .read = _hex_llong_read,  .write = _hex_llong_write },
      .ULLong = { obj_init .read = _hex_ullong_read, .write = _hex_ullong_write},
    },
  .Ref      = { obj_init .read = _ref_read,      .write = _ref_write     },
  .Ptr      = { obj_init .read = _ptr_read,      .write = _ptr_write     },
  .Ptrdiff  = { obj_init .read = _ptrdiff_read,  .write = _ptrdiff_write },
  .Size     = { obj_init .read = _size_read,     .write = _size_write    },
  .Char     = { obj_init .read = _char_read,     .write = _char_write    },
  .Float    = { obj_init .read = _float_read,    .write = _float_write   },
  .Double   = { obj_init .read = _double_read,   .write = _double_write  },
  .CString  = { obj_init .read = _cstring_read,  .write = _cstring_write },
};

