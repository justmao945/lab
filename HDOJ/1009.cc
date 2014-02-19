#include<cstdio>
#include<cstdlib>

struct Room{
  int javaBean;
  int catFood;
};

Room r[1005];

int cmp(const void* a, const void* b) // a < b ? return 1
{
#define a ((Room*)a)
#define b ((Room*)b)
  
  double pa, pb;
  pa = 1.0 * a->javaBean / a->catFood;
  pb = 1.0 * b->javaBean / b->catFood;
  return pa == pb ? 0 : pa < pb ? 1 : -1;

#undef a
#undef b
}

int main()
{
  int M, N;
  while( scanf("%i%i", &M, &N) != EOF ){
    if(M == -1 && N == -1)
      break;

    for(int i=0; i<N; ++i){
      scanf("%i%i", &r[i].javaBean, &r[i].catFood);
    }
    qsort(r, N, sizeof(Room), cmp);

    double bean = 0;
    for(int i=0; i<N && M>0; ++i){
      if(M >= r[i].catFood){
        bean += (double)r[i].javaBean;
        M -= r[i].catFood;
      }else{
        bean += 1.0 * r[i].javaBean / r[i].catFood * M;
        M = 0;
      }
    }
    printf("%.3f\n", bean);
  }
  return 0;
}
