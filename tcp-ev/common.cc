//#define DEBUG

#include "common.h"

void _dummy(...)
{

}

// op: 0 clear flag, 1 set flag
//
void fcntl_fl(int op ,int fd, int flag)
{
  int arg, narg;
  
  arg = fcntl(fd, F_GETFL, 0);
  if(arg < 0){
    perror("getfl faild");
    return;
  }
  
  if(op == 0){
    narg = arg & (~flag);
  } else if(op == 1){
    narg = arg | flag;
  } else{
    perror("op error");
    return;
  }

  if (fcntl(fd, F_SETFL, narg) < 0 ){
    perror("setfl faild");
  }
  d_printf("getfl %x, setfl %x\n", arg, narg);
}

