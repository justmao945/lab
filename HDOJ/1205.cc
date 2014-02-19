#include<cstdio>
// 1*1*1*1*1*1*1...
// max <= sum - max + 1

int main()
{
  int T, N, x;
  long long max, sum;
  scanf("%d", &T);
  while(T--){
    scanf("%d", &N);
    max = -1;
    sum = 0;
    while(N--){
      scanf("%d", &x);
      sum += x;
      if(x > max) max = x;
    }
    if( max <= sum - max + 1 )
      puts("Yes");
    else
      puts("No");
  }
  return 0;
}
