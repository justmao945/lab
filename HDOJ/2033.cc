#include<cstdio>

int main()
{
  int N, ah, am, as, bh, bm, bs;
  scanf("%i", &N);
  for(int i=0; i<N; ++i){
    scanf("%i%i%i%i%i%i", &ah, &am, &as, &bh, &bm, &bs);
    printf("%i %i %i\n", ah + bh + (am + bm + (as + bs)/60) / 60,
        (am + bm + (as + bs)/60) % 60 , (as + bs) % 60);
  }
  return 0;
}
