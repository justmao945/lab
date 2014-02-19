#include<cstdio>
#include<cstdlib>

struct Student{
  int solved;
  int time;
  int mark;
};

Student stu[103];
int ix[103];

int cmp(const void* a, const void *b)
{
#define a (*(int*)a)
#define b (*(int*)b)

  if(stu[a].solved == stu[b].solved)
    return stu[a].time - stu[b].time;
  else
    return stu[b].solved - stu[a].solved;

#undef a
#undef b
}

int main()
{
  int N, i, j, h, m, s, k, half;
  while( scanf("%i", &N) != EOF && N >= 0 ){
    for(i=0; i<N; ++i){
      scanf("%d", &stu[i].solved);
      scanf("%d:%d:%d", &h, &m, &s);
      stu[i].time = h * 3600 + m * 60 + s;
      ix[i] = i;
    }
    qsort(ix, N, sizeof(int), cmp);
    //for(i=0; i<N; ++i)
      //printf("%d ", ix[i]);
    //putchar('\n');

    for(i=0; i<N; ){
      j = i + 1; 
      while( j < N && stu[ix[i]].solved == stu[ix[j]].solved )
        ++j;
      
      for(k=i, half=i+((j-i)>>1); k<half; ++k){
        switch(stu[ix[k]].solved){
          case 5: stu[ix[k]].mark = 100; break;
          case 4: stu[ix[k]].mark = 95; break;
          case 3: stu[ix[k]].mark = 85; break;
          case 2: stu[ix[k]].mark = 75; break;
          case 1: stu[ix[k]].mark = 65; break;
          case 0: stu[ix[k]].mark = 50; break;
          default: break;
        }
      }

      for(; k<j; ++k){
        switch(stu[ix[k]].solved){
          case 5: stu[ix[k]].mark = 100; break;
          case 4: stu[ix[k]].mark = 90; break;
          case 3: stu[ix[k]].mark = 80; break;
          case 2: stu[ix[k]].mark = 70; break;
          case 1: stu[ix[k]].mark = 60; break;
          case 0: stu[ix[k]].mark = 50; break;
          default: break;
        }
      }
      i = j;
    }
    for(i=0; i<N; ++i)
      printf("%i\n", stu[i].mark);
    putchar('\n');
  }
  return 0;
}
