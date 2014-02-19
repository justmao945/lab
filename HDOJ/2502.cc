#include<cstdio>

// left most ONE: (1<<n)-(1<<(n-1))
//
// 1 ONE: (n-1) * 1
// 2 ONEs: 2C(n-1) * 2
// 3 ONEs: 3C(n-1) * 3
// ... n ONEs: nC(n-1) * n
//     => F(n) = 0 * 0Cn + 1     * 1Cn + ... + n * nCn
// assume G(n) = n * 0Cn + (n-1) * 1Cn + ... + 0 * nCn
// and F(n) == G(n) ==> 2F(n) = n * 2^n ==> F(n) = n * 2^(n-1)

int main()
{
  int T, n;
  scanf("%d", &T);
  while(T--){
    scanf("%d", &n);
    printf("%d\n", (1<<n)-(1<<(n-1)) + (n >= 2 ? (n-1)*(1<<(n-2)) : 0));
  }
  return 0;
}
