#include<cstdio>

using namespace std;

#define CUBE(a) ((a)*(a)*(a))

inline int splitCube(int x)
{
  int s = 0;
  while(x){
    s += CUBE(x % 10);
    x /= 10;
  }
  return s;
}

int main()
{
  int m, n, i;
  while( scanf("%i%i", &m, &n) != EOF ){
    i = 0;
    while(m <= n){ // first one
      if( splitCube(m) == m ){
        printf("%i", m);
        ++i;
        ++m;
        break;
      }
      ++m;
    }

    while(m <= n){
      if( splitCube(m) == m ){
        printf(" %i", m);
        ++i;
      }
      ++m;
    }
    if(i == 0)
      puts("no");
    else
      putchar('\n');
  }
  return 0;
}

