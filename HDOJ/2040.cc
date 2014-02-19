#include<cstdio>
#include<cmath>

int sum(int x)
{
  int s = 1, upper = (int)sqrt((double)x);

  for(int i=2; i<upper; ++i){
    if( x % i == 0 ){
      s += x / i + i;
    }
  }

  if( x % upper == 0 ){
    s += upper;
    if(x / upper != upper)
      s += x / upper;
  }
  return s;
}


int main()
{
  int M, a, b;
  scanf("%i", &M);

  for(int i=0; i<M; ++i){
    scanf("%i%i", &a, &b);
    if( a == sum(b) && b == sum(a) )
      puts("YES");
    else
      puts("NO");
  }
  return 0;
}
