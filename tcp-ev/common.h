#ifndef _COMMON_H_
#define _COMMON_H_

#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>
#include<time.h>
#include<fcntl.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<ev.h> // for libev

#define ID_LEN 8 // 32-bit integer in HEX string
#define ID_LEN_STR "8"
#define HEAD_LEN 16  // 2*ID_LEN

#define MSG_BUF_SIZE 1024  // message buffer size

#define SRV_PORT 9000
#define SRV_MAX_CONN 128


#ifdef DEBUG
  #define d_putchar putchar
  #define d_printf  printf
  #define d_fprintf fprintf
  #define d_write   write
  #define d_fflush  fflush
#else
  #define d_putchar _dummy
  #define d_printf  _dummy
  #define d_fprintf _dummy
  #define d_write   _dummy
  #define d_fflush  _dummy
#endif

// Client struct used in server
struct Client{
  struct sockaddr_in addr; // clinet addr
};

// dummy function.
void _dummy(...);


// fcntl set/clr flag
// @op 0 to clear and 1 to set
// @fd Which fd to set
// @flag See fcntl
void fcntl_fl(int op ,int fd, int flag);


#endif


