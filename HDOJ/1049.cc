#include<cstdio>
#include<cassert>
int main()
{
  int n, u, d, t; // d<u, n<100
  while( scanf("%i%i%i", &n, &u, &d) != EOF){
    if(n==0)
      break;
    
    t = u;
    while( (n - t) % (u - d) != 0 ){
      assert(t > 0);
      --t;
    }
    printf("%i\n", 1 + ((n-t)/(u-d)<<1));
  }
  return 0;
}

