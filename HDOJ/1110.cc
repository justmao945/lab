#include <cstdio>
#include <cmath>
int main(int argc, const char *argv[])
{
  int T;
  scanf("%d", &T);
  while(T--){
    int A,B,X,Y;
    scanf("%d%d%d%d", &A,&B,&X,&Y);
    if(A>B)
      A^=B^=A^=B;
    if(X>Y)
      X^=Y^=X^=Y;
    // A<=B, X<=Y

    if(A>X && B>Y)
      puts("Escape is possible.");
    else if(A*B <= X*Y)
      puts("Box cannot be dropped.");
    else if()
    else{
    }
  }
  return 0;
}
