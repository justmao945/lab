#ifndef _ADEF_H_
#define _ADEF_H_

typedef short int int16;
typedef short unsigned int u16;

typedef int int32;
typedef unsigned int u32;

typedef long long int int64;
typedef unsigned long long int u64;

#define C(str) (str.c_str())

#define THROW
#define NO_THROW throw()

#define DEFAULT_COMMENT_FLAG  ('#')
#define LOG_FILE_EXT          (".log")
#define AOBJECT_FILE_EXT      (".ao.s")
#define ASM_FILE_EXT          (".bin")
#define DISASM_FILE_EXT       (".dis.s")
#define PREPARE_FILE_EXT      (".pre.s")

#endif
