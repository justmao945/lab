#include<cstdio>

int main()
{
  int i, n;
  long long tbl[51] = {0, 1};

  for(i=2; i<51; ++i)
    tbl[i] = tbl[i-1] + tbl[i-2];

  while(~scanf("%d", &n) && ~n)
    printf("%I64d\n", tbl[n]);
  return 0;
}
