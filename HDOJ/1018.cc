#include <stdio.h>
#include <math.h>
#define PI 3.1415926

int main(int argc, const char *argv[])
{
  /* digit = log(n!)/log(10) + 1 
   * or 斯特林公式, n!=sqrt(2*PI*n)*(n/e)^n
   *    log(n!) = 0.5 * log(2 * PI * n) + n * (log(n) - 1)
   * */
  int T, n, i, j;
  double logten = log(10);
  double res;
  scanf("%d", &T);
  while(T--){
    scanf("%d", &n);
    res = 0;
    /*
    for(i=1;i<=n;i++)
      res += log(i);
    */
    res =  0.5 * log(2 * PI * n) + n * (log(n) - 1);
    printf("%d\n", (int)(res/logten) + 1);
  }
  return 0;
}
