#include<cstdio>
#include<cstdlib>

int buf[1001];

int cmp(const void* a, const void* b)
{
  return *(int*)a - *(int*)b;
}

int main()
{
  int T, N;
  
  scanf("%i", &T);

  while(T--){
    scanf("%i", &N);
    for(int i=0; i<N; ++i){
      scanf("%i", buf + i);
    }
    qsort(buf, N, sizeof(int), cmp);
    printf("%i", buf[0]);
    for(int i=1; i<N; ++i){
      printf(" %i", buf[i]);
    }
    putchar('\n');
  }
  return 0;
}
