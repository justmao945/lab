#include<cstdio>

#define EPS (1e-6)

// y = 8*x^4 + 7*x^3 + 2*x^2 + 3*x + 6 - Y in (0,100) is increasing.
double f(double x, double Y)
{
  double sq = x * x;
  return 8.0 * sq * sq + 7.0 * sq * x + 2.0 * sq + 3.0 * x + 6.0 - Y;
}

double fabs(double x)
{
  return x >= 0 ? x : -x;
}


int main()
{
  int T;
  double Y, a, b, m, sa, sb, sm;
  scanf("%d", &T);
  while(T--){
    scanf("%lf", &Y);
    a = 0; b = 100;
    while( fabs(a - b) >= EPS ){
      sa = f(a, Y);
      sb = f(b, Y);
      if(sa * sb > 0){
        puts("No solution!");
        break;
      }else{
        m = (a + b) / 2.0;
        sm = f(m, Y);
        if( sa * sm <= 0 )
          b = m;
        else
          a = m;
      }
    }
    if( fabs(a - b) < EPS )
      printf("%.4f\n", (a+b) / 2);
  }
  return 0;
}
