#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define E(a,b) (fabs((a)-(b)) < 1e-6)

double G(double a, double b, double c, double d, double e, double x)
{
  return a/3.0*x*x*x + (b-d)/2.0*x*x + (c-e)*x;
}

int main(int argc, const char *argv[])
{
  /* y = aX^2 + bX + c; y = dX + e */
  double x1,y1,x2,y2,x3,y3,x4,y4;
  double a,b,c,d,e;
  int T;
  scanf("%d", &T);
  while(T--){
    scanf("%lf%lf%lf%lf%lf%lf", &x1,&y1,&x2,&y2,&x3,&y3);
    d = (y2-y3)/(x2-x3);
    e = y2 - d * x2;
    x4 = x2;
    y4 = y2;
    if(E(x1,x2)&&E(x2,x3)&&E(y1,y2)&&E(y2,y3))
      printf("0.00\n");
    else if(E(x1,x2)&&E(y1,y2)){
      x4 = x3;
      y4 = y3;
    }
    a = (y4-y1)/((x4-x1)*(x4-x1));
    b = -2 * a * x1;
    c = y1 - a*x1*x1 - b*x1;
    printf("%.2f\n", G(a,b,c,d,e,x3)-G(a,b,c,d,e,x2));
  }
  return 0;
}
