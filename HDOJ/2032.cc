#include<cstdio>

int buf[30][30]; // or 2x30

int main()
{
  int n;

  buf[0][0] = 1;
  buf[1][0] = buf[1][1] = 1;

  for(int i=2; i<30; ++i){
    for(int j=1; j<=i; ++j){
      buf[i][j] = buf[i-1][j-1] + buf[i-1][j];
    }
    buf[i][0] = buf[i][i] = 1;
  }

  while( scanf("%i", &n) != EOF ){
    for(int i=0; i<n; ++i){
      printf("%i", buf[i][0]);
      for(int j=1; j<=i; ++j){
        printf(" %i", buf[i][j]);
      }
      putchar('\n');
    }
    putchar('\n');
  }
  return 0;
}
