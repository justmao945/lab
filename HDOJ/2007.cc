#include<cstdio>
#include<cassert>
using namespace std;

int sum0(int a, int b)
{
  int s = 0;
  while(a <= b){
    s += a * a;
    a += 2;
  }
  return s;
}

int sum1(int a, int b)
{
  int s = 0;
  while(a <= b){
    s += a * a * a;
    a += 2;
  }
  return s;
}


int main()
{
  int a, b;
  while( scanf("%i%i", &a, &b) != EOF ){
    if(a > b)
      a ^= b ^= a ^= b;

    if(a % 2 == 0){
      printf("%i %i\n", sum0(a, b), sum1(a+1, b));
    }else{
      printf("%i %i\n", sum0(a+1, b), sum1(a, b));
    }
  }
  return 0;
}
