#include<cstdio>

int dict[][5] = 
{
  {0,0},
  {1,1},
  {2,4,8,6,2},
  {3,9,7,1,3},
  {4,6,4},
  {5,5},
  {6,6},
  {7,9,3,1,7},
  {8,4,2,6,8},
  {9,1,9},
};

int period[] = {1,1,4,4,2,1,1,4,4,2};

int main()
{
  int T, N, i;
  scanf("%d", &T);
  while(T--){
    scanf("%d", &N);
    printf("%d\n", dict[N % 10][(N-1) % period[N % 10]]);
  }
  return 0;
}
