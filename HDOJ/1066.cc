#include <stdio.h>
#include <math.h>
int main(int argc, const char *argv[])
{
  int u, i;
  long long int r, j;
  while(scanf("%d", &u) != EOF){
    for(r=i=1; i<=u; i++){
      r *= i;
      for(j=1000000000000000000; j>1;j/=10)
        if(r%j == 0)
          break;
      r = r / j % 10000000000;
    }
    printf("%lld\n", r%10);
  }
  return 0;
}
