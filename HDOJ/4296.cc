// Greedy algo. s1 - w2 > s2 - w1 ==> s1 under s2.

#include<cstdio>
#include<cstdlib>
#include<cassert>

struct floor_t{
  int w, s;
};

// sort floors from top to bottom.
int cmp(const void* a, const void* b)
{
  assert(a);
  assert(b);
#define a ((const floor_t*)a)
#define b ((const floor_t*)b)
  return (a->s - b->w) - (b->s - a->w);
#undef a
#undef b
}

floor_t buf[100000];

int main()
{
  int N, i;
  __int64 w, tmp, pdv;
  while( scanf("%d", &N) != EOF ){
    for(i=0; i<N; ++i){
      scanf("%d%d", &buf[i].w, &buf[i].s);
    }
    qsort(buf, N, sizeof(floor_t), cmp);
    w = buf[0].w; // top of building.
    pdv = 0;
    for(i=1; i<N; ++i){
      if( (tmp = w - buf[i].s) > pdv )
        pdv = tmp;
      w += buf[i].w;
    }
    printf("%I64d\n", pdv);
  }
  return 0;
}
