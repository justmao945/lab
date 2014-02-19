#include<cstdio>

int main()
{
  int T, N;
  long long int sum;
  scanf("%d", &T);
  while( T-- ){
    scanf("%d", &N);
    if(N == 1) puts("0");
    sum = 1;
    for(int i=2; i*i<=N; ++i){
      if( N % i == 0 )
        sum += i == N / i ? i : i + N / i;
    }
    printf("%I64d\n", sum);
  }
  return 0;
}
