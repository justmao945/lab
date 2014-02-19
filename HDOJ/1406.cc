#include<cstdio>

/* in the range (1,10000): 6 28 496 8128 */
bool isComplete(int x)
{
  int sum = 1;
  for(int i=2; i*i<=x; ++i){
    if( x % i == 0 ){
      sum += i;
      if( i != x / i ) sum += x / i;
    }
  }
  return sum == x;
}

#define IN(x, a, b) ( (x) >= (a) && (x) <= (b) )

int main()
{
  int n, a, b, sum;

  scanf("%d", &n);
  while(n--){
    scanf("%d%d", &a, &b);
    if(a > b) a^=b^=a^=b;
    sum = 0;
    if( IN(6, a, b) ) ++sum;
    if( IN(28, a, b) ) ++sum;
    if( IN(496, a, b) ) ++sum;
    if( IN(8128, a, b) ) ++sum;
    printf("%d\n", sum);
  }
  return 0;
}
