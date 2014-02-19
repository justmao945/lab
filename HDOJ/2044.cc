#include<cstdio>
#include<cassert>

// F(n) = F(n-1) + F(n-2) 

long long int buf[51];

int main()
{
  int N, a, b;
  scanf("%d", &N);
  while(N--){
    scanf("%d%d", &a, &b);
    buf[a] = 1;
    buf[++a] = 1;
    while(++a <= b){
      buf[a] = buf[a-1] + buf[a-2];
      assert(buf[a] > 0);
    }
#ifdef ONLINE_JUDGE
    printf("%I64d\n", buf[b]);
#else
    printf("%lld\n", buf[b]);
#endif
  }
  return 0;
}
