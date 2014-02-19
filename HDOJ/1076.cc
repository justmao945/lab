#include<cstdio>

#define isLeap(x) ( ((x)%4==0 && (x)%100!=0) || (x)%400==0 )


int main()
{
  int T, Y, N;
  scanf("%i", &T);
  while(T--){
    scanf("%i%i", &Y, &N);
    for(int i=0; i<N; ){
      if( isLeap(Y) ) ++i;
      ++Y;
    }
    printf("%i\n", --Y);
  }

  return 0;
}
