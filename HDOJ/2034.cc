#include<cstdio>
#include<cstdlib>
#include<set>

int cmp(const void* a, const void* b)
{
  return *(int*)a - *(int*)b;
}

int main()
{
  int buf[100];
  int N, M, x;
  std::set<int> set;

  while( scanf("%i%i", &N, &M) != EOF ){
    if( N == 0 && M == 0 )
      break;
    set.clear();
    for(int i=0; i<N; ++i){
      scanf("%i", buf + i);
    }
    for(int i=0; i<M; ++i){
      scanf("%i", &x);
      set.insert(x); 
    }
    x = 0;
    qsort(buf, N, sizeof(int), cmp);
    for( int i=0; i<N; ++i ){
      if( set.find(buf[i]) == set.end() ){ // not in set B
        printf("%i ", buf[i]);
        ++x;
      }
    }
    if(x == 0)
      puts("NULL");
    else
      putchar('\n');
  }
  return 0;
}
