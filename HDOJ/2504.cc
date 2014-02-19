#include<cstdio>
#include<cassert>

int gcd(int x, int y)
{
  if(x > y) return gcd(y, x); // x <= y

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
  int N, a, b, c;
  scanf("%d", &N);
  while(N--){
    scanf("%d%d", &a, &b);
    assert( a % b == 0 );
    for(int i=2; ; ++i){
      c = b * i;
      if( gcd(a, c) == b ){
        printf("%d\n", c);
        break;
      }
    }
  }
  return 0;
}
