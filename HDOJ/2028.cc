#include<cstdio>

int gcd(int x, int y)
{
  if(x > y)
    return gcd(y, x); // x <= y
  int tmp;
  while(1){
    if( (tmp = y % x) == 0 ){
      return x;
    }else{
      y = x;
      x = tmp;
    }
  }
}


int main()
{
  int N, lcm, x;
  while( ~scanf("%d", &N) ){
    lcm = 1;
    while(N--){
      scanf("%d", &x);
      lcm = lcm / gcd(lcm, x) * x;
    }
    printf("%d\n", lcm);
  }
  return 0;
}
