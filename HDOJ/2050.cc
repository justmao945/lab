#include<cstdio>
#include<cassert>

// F(n) = F(n-1) + 2*2(n-1) + 1
// ... has 2*2(n-1) new points.
// <= 2(n-1) old lines and an angle has two lines.

long long int dict[10001];

int main()
{
  int C, n;

  dict[1] = 2;
  for(int i=2; i<10001; ++i){
    dict[i] = dict[i-1] + 4 * (i-1) + 1;
    assert(dict[i] > 0);
  }
  
  scanf("%d", &C);
  while(C--){
    scanf("%d", &n);
#ifdef ONLINE_JUDGE
    printf("%I64d\n", dict[n]);
#else
    printf("%lld\n", dict[n]);
#endif
  }
  return 0;
}
