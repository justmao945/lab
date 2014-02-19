#include<cstdio>

#define ABS(x) ((x)>=0 ? (x) : -(x))

int main()
{
  int m,n,i,j,mark, _i, _j, _mark;
  while( scanf("%i%i", &m, &n) != EOF ){
    _i = _j = 1;
    _mark = 0;

    for(i=1; i<=m; ++i){
      for(j=1; j<=n; ++j){
        scanf("%i", &mark);
        if( ABS(mark) > ABS(_mark) ){
          _i = i;
          _j = j;
          _mark = mark;
        }
      }
    }
    printf("%i %i %i\n", _i, _j, _mark);
  }
  return 0;
}
