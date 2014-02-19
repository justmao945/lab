#include <stdio.h>
#define N 125

unsigned A[N], B[N];

int main(int argc, const char *argv[])
{ /* generate function 
     (1+x+x^2+...+x^n)*(1+x^2+x^4+...+x^n)*...*(1+x^n)
     with n sections
     */
  unsigned n, i, j, k;
  while(scanf("%d", &n) != EOF){
    for(i=0;i<=n;i++){
      A[i] = 1;
      B[i] = 0;
    }
    
    for(i=2;i<=n;i++){
      for(j=0;j<=n;j++)
        for ( k=0;k+j<=n;k+=i ) 
          B[j+k] += A[j];
        
      for(j=0;j<=n;j++){
        A[j] = B[j];
        B[j] = 0;
      }
    }
    printf("%u\n", A[n]);
  }
  return 0;
}
