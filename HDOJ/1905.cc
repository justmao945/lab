#include<cstdio>
#include<cmath>

// a % p = m ==> a * a % p <==> m * a % p
//
// S1: find period T
// S2: using ?a^(2^0) + ?a^(2^1) + ?a^(2^2) +....

bool isPrime(int x)
{
  int upper = (int)sqrt((double)x);
  if(x == 1) return false;
  for(int i=2; i<=upper; ++i)
    if(x % i == 0) return false;
  return true;
}

long long int dict[32];

int main()
{
  int p, a, i, op;
  long long int rm;
  while(~scanf("%d%d", &p, &a)){
    if(p == 0 && a == 0)
      break;

    dict[0] = a % p;
    for(i=1; i<32 && (1<<i)<=p; ++i){
      dict[i] = (dict[i-1] * dict[i-1]) % p;
      //printf("dict[%d] = %lld\n", i, dict[i]);
    }
    rm = 1;
    op = p;
    while( p ){
      while(p < (1<<i)) --i;
      rm = rm * dict[i] % op;
      //printf("rm = %lld\n", rm);
      p %= 1<<i;
    }
    if(a == rm && !isPrime(op))
      puts("yes");
    else
      puts("no"); 
  }
  return 0;
}

