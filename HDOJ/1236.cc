#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _stu{
  char num[21];
  int sum;
} stu[1000];

int marks[11];

int cmp(const void* a, const void* b)
{
  struct _stu* pa = (struct _stu*)a;
  struct _stu* pb = (struct _stu*)b;
  if(pa->sum != pb->sum)
    return pb->sum - pa->sum;
  else
    return strcmp(pa->num, pb->num);
}

int main(int argc, const char *argv[])
{
  int N, M, G;
  int i,j,k, p;
  int sum;
  while(scanf("%d", &N)!=EOF && N){
    scanf("%d%d",&M,&G);
    sum = 0;
    memset(stu, 0, 1000*sizeof(struct _stu));

    for(i=1;i<=M;i++)
      scanf("%d", &marks[i]);
    
    for(i=0;i<N;i++){
      scanf("%s%d", stu[i].num, &p);
      for(j=0;j<p;j++){
        scanf("%d", &k);
        stu[i].sum += marks[k];
      }
      if(stu[i].sum >= G)
        sum++;
    }

    qsort(stu, N, sizeof(struct _stu), cmp);
    printf("%d\n", sum);
    for(i=0;i<sum;i++)
      printf("%s %d\n", stu[i].num, stu[i].sum);
  }
  return 0;
}
