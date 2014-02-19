#include <stdio.h>
#include <math.h>
int main(int argc, const char *argv[])
{
  int T, i;
  double x, y, r;
  scanf("%d",&T);

  for(i=1;i<=T;i++){
    scanf("%lf%lf", &x,&y);
    r = 3.1415926 * (x*x + y*y) / 2.0 / 50 + 1;
    printf("Property %d: This property will begin eroding in year %d.\n", i, (int)r);
  }
  puts("END OF OUTPUT.");
  return 0;
}
