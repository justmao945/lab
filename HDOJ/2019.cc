#include<cstdio>

// deque binary search...

int main()
{
  int n, m, x, i;
  bool inserted;
  while(scanf("%i%i", &n, &m) != EOF){
    if(n==0 && m==0)
      break;
    inserted = false;
    i=0;
    
    if(n > 0){
      scanf("%i", &x);
      if(m <= x){
        printf("%i ", m);
        inserted = true;
      }
      printf("%i", x);
      ++i;
    }

    if( ! inserted ){
      for(; i<n; ++i){
        scanf("%i", &x);
        if(m <= x){
          printf(" %i %i", m, x);
          inserted = true;
          ++i;
          break;
        }
        printf(" %i", x);
      }
    }

    for(; i<n; ++i){
      scanf("%i", &x);
      printf(" %i", x);
    }
    if( ! inserted ){
      if(i == 0)
        printf("%i", m);
      else
        printf(" %i", m);
    }
    putchar('\n');
  }
  return 0;
}
