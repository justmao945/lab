#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define XX 500
int p[XX];

int main(int argc, const char *argv[])
{
  int M, N;
  int i, j, k, min;
  scanf("%d", &M);
  while(M--){
    min = -1;
    scanf("%d", &N);
    for(i=0;i<N;i++)
      scanf("%d", &p[i]);

    for(i=0;i<N;i++){
      for(j=0, k=0;j<N;j++)
        k += abs(p[i]-p[j]);
      if(min == -1 || min > k)
        min = k;
    }
    printf("%d\n", min);
  }
  return 0;
}
