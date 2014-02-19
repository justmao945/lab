#include<cstdio>

const static int buf[] = {100, 50, 10, 5, 2, 1};

int p(int m)
{
  int s = 0;
  for(int i=0; i<sizeof(buf)/sizeof(int); ++i){
    if(m >= buf[i]){
      s += m / buf[i];
      m %= buf[i];
    }
  }
  return s;
}

int main()
{
  int n, s, x;
  while( scanf("%i", &n) != EOF){
    if(n == 0) break;
    s = 0;
    for(int i=0; i<n; ++i){
      scanf("%i", &x);
      s += p(x);
    }
    printf("%i\n", s);
  }
  return 0;
}
