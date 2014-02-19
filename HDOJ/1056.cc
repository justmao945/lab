#include <cstdio>
#include <cmath>
double D[300];

int main(int argc, const char *argv[])
{
  D[0]  = 0;
  for( int i=1; i<300; i++)
    D[i] = D[i-1] + 1.0/(i+1.0);

  double d;
  while(scanf("%lf", &d) != EOF && d > 1e-6){
    for(int i=1; i<300; i++)
      if(D[i]>=d){
        printf("%d card(s)\n", i);
        break;
      }
  }
  return 0;
}
