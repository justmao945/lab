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

#ifndef __YACL_CONF_H
#define __YACL_CONF_H

/* C++ mode */
#if defined(__cplusplus)
  #define YACL_CXX
#endif


/* Compilers */
#if defined(_MSC_VER)
  #define YACL_MSC
#endif

#if defined(__GNUC__)
  #define YACL_GCC
#endif

#if defined(__clang__)
  #define YACL_CLANG
#endif


/* C version */
#if __STDC_VERSION__ >= 199901L
  #define YACL_C99

  #if !defined(__STDC_NO_VLA__)
    #define YACL_VLA
  #endif
#endif


/* long long */
#if defined(YACL_C99) || defined(YACL_GCC) || defined(YACL_CLANG)
  #define YACL_LONGLONG
#endif

/* 32-bit or 64-bit */
#include<limits.h>
#if INT_MAX < LONG_MAX  /* bug? */
  #define YACL_64
#endif

/**
 * FIXME:
 * 1. Config for generic algorithm support?
 * 2. Config for CDECL calling? (aka variadic functions)
 */

#endif /* __YACL_CONF_H */

