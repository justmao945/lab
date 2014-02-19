#include<cstdio>
#include<cassert>

long long mama[55] = {0,1,1,1,1}, child[55] = {0,0,1,2,3}; 

int main()
{
  int n;
  for(int i=5; i<55; ++i){
    mama[i] = mama[i-1] + mama[i-3]; // mama[i-3] == new child in year i-3
    child[i] = child[i-1] + mama[i-1];
  }

  while( ~scanf("%d", &n) && n ){
    assert(mama[n] + child[n] > 0);
#ifdef ONLINE_JUDGE
    printf("%I64d\n", mama[n] + child[n]);
#else
    printf("%lld\n", mama[n] + child[n]);
#endif
  }
  return 0;
}
