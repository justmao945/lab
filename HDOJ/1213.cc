#include <stdio.h>

int arr[1001];

int find_root(int x)
{
  if(x != arr[x])
    return arr[x] = find_root(arr[x]);
  else
    return x;
}

int main(int argc, const char *argv[])
{
  int T;
  int N, M;
  int i, j;
  int a, b;
  scanf("%d", &T);
  while(T--){
    scanf("%d%d", &N,&M);
    for(i=1;i<=N;i++)
      arr[i] = i;
    for(i=0;i<M;i++){
      scanf("%d%d", &a, &b);
      a = find_root(a);
      b = find_root(b);
      if(a!=b)
        arr[a] = b;
    }
    for(i=1, j=0;i<=N;i++)
      if(i == arr[i])
        j++;
    printf("%d\n", j);
  }
  return 0;
}
