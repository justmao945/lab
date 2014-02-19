#include<cstdio>

int gcd(int x, int y)
{
  int t;
  while(1){
    if(x > y) x ^= y ^= x ^= y;
    if( y % x == 0 ){
      return x;
    }else{
      t = x;
      x = y % x;
      y = t;
    }
  }
}

int main()
{
  int T, n, x, lcm;
  scanf("%d", &T);
  while(T--){
    scanf("%d", &n);
    lcm = 1;
    for(int i=0; i<n; ++i){
      scanf("%d", &x);
      lcm = lcm / gcd(lcm, x) * x;
    }
    printf("%d\n", lcm);
  }
  return 0;
}
