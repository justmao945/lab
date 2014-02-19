#include<cstdio>
#include<cassert>

// considering the most right char. H or D.
// H: F(m-1, n).
// D: F(m, n-1).
// F(m,n) = F(m-1, n) + F(m, n-1);

long long int dict[21][21];

int main()
{
  int m,n;
  dict[0][0] = dict[0][1] = 0;
  dict[1][0] = 1;
  for(int i=1; i<21; ++i){
    dict[i][0] = 1;
    for(int j=1; j<21; ++j){
      if( i < j )
        dict[i][j] = 0;
      else
        dict[i][j] = dict[i-1][j] + dict[i][j-1];
      //printf("dict[%d][%d] = %lld\n", i, j, dict[i][j]);
      assert(dict[i][j] >= 0);
    }
  }

  while( ~scanf("%d%d", &m, &n) ){
#ifdef ONLINE_JUDGE
    printf("%I64d\n", dict[m][n]);
#else
    printf("%lld\n", dict[m][n]);
#endif
  }
  return 0;
}
