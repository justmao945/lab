#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(int argc, const char *argv[])
{
  int T, i, j, k;
  int h,m,s;
  double dh, dm, ds, deg;
  scanf("%d", &T);
  for(i=0;i<T;i++){
    scanf("%d%d%d", &h,&m,&s);
    ds = 6 * s;
    dm = 6 * (s/60.0 + m);
    dh = 30 * ( s/3600.0 + m/60.0 + (h>=12 ? h-12 : h) );
    deg = fabs(dm - dh);
    if(deg > 180)
      deg = 360 - deg;
    printf("%d\n", (int)deg);
  }
  return 0;
}
