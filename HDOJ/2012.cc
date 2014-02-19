#include<cstdio>
#include<cmath>
using namespace std;

bool isPrime(int x)
{
  if(x == 0 || x == 1)
    return false;

  int upper = (int)sqrt((double)x);
  for(int i=2; i<=upper; ++i){
    if( x % i == 0 )
      return false;
  }
  return true;
}



int main()
{
  int x, y;
  while( scanf("%i%i", &x, &y) != EOF ){
    if( x == 0 && y == 0)
      break;

    while(x <= y){
      if( ! isPrime(x*x + x + 41) ){
        puts("Sorry");
        ++x;
        break;
      }
      ++x;
    }
    if(x > y)
      puts("OK");
  }
  return 0;
}
