#include<cstdio>
#include<cassert>

// F(1) = 3, F(2) = 6, F(3) = 6.
// F(n) = F(n-1) + 2*F(n-2), n >= 4.
// case F(n-1): 1st-box and (n-1)th box have diff colors.(valid case)
// case F(n-2): 1st-box and (n-1)th box have the same color.(x.x)

long long int buf[51] = {0, 3, 6, 6};

int main()
{
  int N;

  for(int i=4; i<51; ++i){
    buf[i] = buf[i-1] + (buf[i-2]<<1);
    assert(buf[i] > 0);
  }

  while( ~scanf("%d", &N) ){
#ifdef ONLINE_JUDGE
    printf("%I64d\n", buf[N]);
#else
    printf("%lld\n", buf[N]);
#endif
  }
  return 0;
}
