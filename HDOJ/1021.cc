#include<cstdio>

void print()
{
  int a = 7, b = 11, t;
  
  for(int i=2; i<100; ++i){
    printf("%d %d %d\n",i, a + b, (a+b) % 3 == 0);
    t = a;
    a = b;
    b += t;
  }
}


int main()
{
  // print();
  int x;
  while( scanf("%d", &x) != EOF ){
    if(x < 2)
      puts("no");
    else if( (x-2) % 4 == 0 )
      puts("yes");
    else
      puts("no");
  }
  return 0;
}
