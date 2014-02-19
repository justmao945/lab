#include<cstdio>
#include<cassert>

// F(n) = (F(n-1) - 1)<<1, F(0) = 3;

int buf[31];

int main()
{
  int N, a;

  buf[0] = 3;
  for(int i=1; i<31; ++i){
    buf[i] = (buf[i-1] - 1) << 1;
    assert(buf[i] > 0);
  }

  scanf("%d", &N);
  while(N--){
    scanf("%d", &a);
    printf("%d\n", buf[a]);
  }
  return 0;
}
