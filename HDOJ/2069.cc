#include<cstdio>

int main()
{
  int i50, i25, i10, i5;
  int r50, r25, r10, r5;
  int x, sum;
  while( ~scanf("%d", &x) ){
    sum = 0;
    for(i50=x/50; i50>=0; --i50){
      r50 = x - i50 * 50;
      for(i25=r50/25; i25>=0; --i25){
        r25 = r50 - i25 * 25;
        for(i10=r25/10; i10>=0; --i10){
          r10 = r25 - i10 * 10;
          for(i5=r10/5; i5>=0; --i5){
            r5 = r10 - i5 * 5;
            if( i50 + i25 + i10 + i5 + r5 <= 100 )
              ++sum;
          }
        }
      }
    }
    printf("%d\n", sum);
  }
  return 0;
}
