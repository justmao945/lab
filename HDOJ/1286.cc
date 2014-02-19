#include<cstdio>

// p and q are prime numbers
// ==> u(p^k1 * q^k2) = u(p^k1) * u(q^k2)
//      = (p-1)*p^(k1-1) * (q-1)*q^(k2-1)
int euler(int x)
{
  int pro = 1, i;
  for(int p=2; p*p<=x; ++p){
    if( x % p == 0 ){ // starts from prime number.
      i = 1;
      //printf("p = %d\n", p);
      x /= p; // -1 times.
      pro *= p - 1; // pro *= (p-1)
      while( x % p == 0 ){
        pro *= p; // p^(k-1)
        x /= p;   // 
        ++i;
      }
      //printf("k = %d\n", i);
    }
  }
  //printf("x = %d\n", x);
  if( x > 1 ){ // remaining prime number.
    pro *= x - 1; // (x-1)*x^(1-1)
  }
  return pro;
}

int main()
{
  int CN, N;
  scanf("%d", &CN);
  while(CN--){
    scanf("%d", &N);
    printf("%d\n", euler(N));
  }
  return 0;
}
