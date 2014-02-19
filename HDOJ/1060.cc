#include <stdio.h>
#include <math.h>
int main(int argc, const char *argv[])
{
  /* num = a * 10 ^ n
   * n = (int)log10(num)
   * log(N^N) = log(num)
   * */
  int T, n;
  double d;
  scanf("%d", &T);
  while(T--){
    scanf("%d", &n);
    d = n*log10(n);
    d -= (long long int)d;
    printf("%d\n", (int)pow(10, d));
  }
  return 0;
}
