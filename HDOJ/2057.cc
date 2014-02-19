#include<cstdio>

int main()
{
  long long a, b;
  while( ~scanf(  "%I64X%I64X" /* "%llx%llx" */, &a, &b) ){
    a += b;
    if(a < 0)
      printf( /* "-%llx\n" */ "-%I64X\n", -a);
    else
      printf( /* "%llx\n" */ "%I64X\n", a);
  }
  return 0;
}
