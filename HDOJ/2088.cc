#include<cstdio>

int main()
{
  int N, avg, sum, i, buf[50], ans, set = 0;

  while( ~scanf("%d", &N) && N ){
    sum = 0;
    for(i=0; i<N; ++i){
      scanf("%d", &buf[i]);
      sum += buf[i];
    }
    avg = sum / N;
    ans = 0;
    for(i=0; i<N; ++i){
      if( buf[i] > avg )
        ans += buf[i] - avg;
    }
    if(set == 0){
      printf("%d\n", ans);
    } else {
      printf("\n%d\n", ans);
    }
    ++set;
  }
  return 0;
}
