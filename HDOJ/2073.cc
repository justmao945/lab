#include <stdio.h>
#include <math.h>
#define X 200

double L[X][X];

void init()
{
  int i, j, k;
  const double d = sqrt(2);
  for(i=0;i<X;i++){
    for(j=0;j<=i;j++){
      if(i==0 && j==0)
        L[i][i-j] = 0;
      else if(j>0)
        L[j][i-j] = L[j-1][i-j+1] + d;
      else
        L[j][i-j] = L[i-1][0] + sqrt(i*i+(i-1)*(i-1));
    }
  }
}

int main(int argc, const char *argv[])
{
  int N;
  int x1, y1, x2, y2;
  int i, j;
  double l;

  init();
  scanf("%d", &N);
  while(N--){
    scanf("%d%d%d%d", &x1,&y1,&x2,&y2);
    printf("%.3f\n", fabs(L[x1][y1]-L[x2][y2]));
  }
  return 0;
}
