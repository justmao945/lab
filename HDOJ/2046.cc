#include<cstdio>
#include<cassert>

long long int dict[51];

int main()
{
  int n;

  dict[1] = 1;
  dict[2] = 2;
  for(int i=3; i<51; ++i){
    dict[i] = dict[i-1] + dict[i-2];
    assert(dict[i] > 0);
  }

  while( ~scanf("%d", &n) ){
#ifdef ONLINE_JUDGE
    printf("%I64d\n", dict[n]);
#else
    printf("%lld\n", dict[n]);
#endif
  }
  return 0;
}
