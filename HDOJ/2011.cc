#include<cstdio>

double buf[1000];

int main()
{
  int m, n, s;

  buf[0] = 0;
  s = 1;
  for(int i=1; i<1000; ++i){
    buf[i] = buf[i-1] +  1.0 / i * s;
    s = -s;
  }

  scanf("%d", &m);
  for(int i=0; i<m; ++i){
    scanf("%d", &n);
    printf("%.2lf\n", buf[n]);
  }
  //getchar();getchar();
  return 0;
}
