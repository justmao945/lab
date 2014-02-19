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


#ifndef YACL_UTILS_H
#define YACL_UTILS_H

#include <stdbool.h>
#include <stddef.h>


#ifdef __cplusplus
extern "C" {
#endif


/**
 * Swap memory area pointed by `lhs` and `rhs.
 * @param lhs, rhs Pointers to memory area to exchange.
 * @param size Byte size of `lhs` and `rhs`.
 * @return Pointer to `lhs`
 * @complexity Liner in the `size`.
 */
void* m_swap(void *lhs, void *rhs, size_t size);

/**
 */
void* m_reverse(void* base, size_t n, size_t size);

/**
 * `swap` prototype: `Type* (*)(Type* l, Type* r)`
 *                   `Type* (*)(Type* l, Type* r, size_t size)`
 */
void* m_reverse0(void* base, size_t n, size_t size, void* swap);

/**
 * `lt` prototype: `bool (*)(Type* l, Type* r)`
 *                 `bool (*)(Type* l, Type* r, size_t size)`
 */
void* m_qsort(void* base, size_t n, size_t size, void* lt);

/**
 * `lt` prototype: `bool (*)(Type* l, Type* r)`
 *                 `bool (*)(Type* l, Type* r, size_t size)`
 * `swap` prototype: `Type* (*)(Type* l, Type* r)`
 *                   `Type* (*)(Type* l, Type* r, size_t size)`
 */
void* m_qsort0(void* base, size_t n, size_t size, void* lt, void* swap);


#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* YACL_UTILS_H */
