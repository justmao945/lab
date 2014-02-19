#include<cstdio>
#include<cassert>

// F(n) = F(n-1) + F(n-2)
// case n-1 should +1 to n
// case n-2 should +1 * 2 or +2 to n, but +1 * 2 is included in case n-1

int buf[41];

int main()
{
  int N, M;
  buf[0] = 0;
  buf[1] = 1;
  for(int i=2; i<41; ++i){
    buf[i] = buf[i-1] + buf[i-2];
    assert(buf[i] > 0);
  }

  scanf("%d", &N);
  while(N--){
    scanf("%d", &M);
    printf("%d\n", buf[M]);
  }
  return 0;
}

