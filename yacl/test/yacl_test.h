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


#ifndef YACL_TEST_H
#define YACL_TEST_H

#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <yacl/__conf.h>


#ifdef  COLOR_TERM
  #define TERM_CO_W "\e[1;30m"
  #define TERM_CO_R "\e[1;31m"
  #define TERM_CO_G "\e[1;32m"
  #define TERM_CO_Y "\e[1;33m"
  #define TERM_CO_B "\e[1;34m"
  #define TERM_CO_M "\e[1;35m"
  #define TERM_CO_C "\e[1;36m"
  #define TERM_CO__ "\e[0m"
#else  /* COLOR_TERM */
  #define TERM_CO_W 
  #define TERM_CO_R 
  #define TERM_CO_G 
  #define TERM_CO_Y 
  #define TERM_CO_B 
  #define TERM_CO_M 
  #define TERM_CO_C 
  #define TERM_CO__
#endif /* COLOR_TERM */


#undef EXE
#define EXE(proc)  \
{ \
  fprintf(stdout, \
    TERM_CO_G"<EXE>"TERM_CO_W"[%4i]"TERM_CO_G""#proc""TERM_CO__"\n", \
    __LINE__); \
  fflush(stdout); \
  (proc);\
}

#undef  DBG
#define DBG(msg) \
  ( \
  fprintf(stdout, \
    TERM_CO_G"<DBG>"TERM_CO_W"[%4i]"TERM_CO_G" %s."TERM_CO__"\n",\
    __LINE__, (msg)), fflush(stdout) \
  )

#undef  WAR
#define WAR(msg) \
  ( \
  fprintf(stdout, \
    TERM_CO_Y"<WAR>"TERM_CO_W"[%4i]"TERM_CO_Y" %s."TERM_CO__"\n",\
    __LINE__, (msg)), fflush(stdout) \
  )

#undef  ERR
#define ERR(msg) \
  ( \
  fprintf(stderr, \
    TERM_CO_R"<ERR>"TERM_CO_W"[%4i]"TERM_CO_R" %s."TERM_CO__"\n",\
    __LINE__, (msg)), fflush(stderr) \
  )

#undef  ASSERT
#define ASSERT(x) ( (x) ? 0 : ERR("ASSERT FAILED ("#x")") )

#undef  BR
#define BR (putchar('\n'), fflush(stdout)) 

#undef STUB
#define STUB \
  ( \
    printf(TERM_CO_Y"<STUB>"TERM_CO_W"[%i]\n"TERM_CO__, __LINE__), \
    fflush(stdout) \
  )

#endif /* YACL_TEST_H */


