#include<cstdio>
#include<cmath>

#define P(x) ((x)*(x))
//
//  sqrt((x1-x2)^2 + (y1-y2)^2)

int main()
{
  double in[4];
  while(scanf("%lf%lf%lf%lf", &in[0], &in[1], &in[2], &in[3]) != EOF){
    printf("%.2f\n", sqrt( P(in[0] - in[2]) + P(in[1] - in[3])) );
  }
  return 0;
}
