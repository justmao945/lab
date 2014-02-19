#include<cstdio>

int gcd(int x, int y)
{
  int t;
  while(1){
    if(y < x) x ^= y ^= x ^= y;
    if(y % x == 0){ // y >= x
      return x;
    } else {
      t = x;
      x = y % x;
      y = t;
    }
  }
}


int main()
{
  int x, y;
  while(scanf("%d%d", &x, &y) != EOF){
    printf("%d\n", x / gcd(x, y) * y);
  }

  return 0;
}
