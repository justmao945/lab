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

#include <stdlib.h>
#include <yacl/alloc.h>

static alloc_t* _init_(alloc_t* self){ return self; }
static alloc_t* _initcp_(alloc_t* self, const alloc_t* other){ return self; }
static alloc_t* _del_(alloc_t* self){ return self; }

static void* 
_malloc(alloc_t* alloc, size_t size)
{
  return malloc(size);
}


static void*
_calloc(alloc_t* alloc, size_t num, size_t size)
{
  return calloc(num, size);
}


static void* 
_realloc(alloc_t* alloc, void* ptr, size_t new_size)
{
  return realloc(ptr, new_size);
}


static void 
_free(alloc_t* alloc, void* ptr)
{
  free(ptr);
}


const alloc_z Alloc = 
{
  ._sizeof_ = sizeof(alloc_t),
  ._init_   = _init_,
  ._initcp_ = _initcp_,
  ._del_   = _del_,
  .malloc  = _malloc,
  .calloc  = _calloc,
  .realloc = _realloc,
  .free    = _free,
};

alloc_t _alloc_;

