#include<cstdio>

int sum(int x, int base)
{
  int s = 0;
  while( x ){
    s += x % base;
    x /= base;
  }
  return s;
}

int main()
{
  int x, t;
  while( scanf("%d", &x) != EOF && x ){
    if( (t = sum(x, 10)) == sum(x, 16) && t == sum(x, 12) )
      printf("%d is a Sky Number.\n", x);
    else
      printf("%d is not a Sky Number.\n", x);
  }
  return 0;
}
