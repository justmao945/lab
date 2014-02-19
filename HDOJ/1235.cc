#include<cstdio>
#include<cstring>

int buf[101];

int main()
{
  int T, x;
  while( scanf("%d", &T) != EOF && T > 0){
    memset(buf, 0, sizeof buf);
    for(int i=0; i<T; ++i){
      scanf("%d", &x);
      ++buf[x];
    }
    scanf("%d", &x);
    printf("%d\n", buf[x]);
  }
  return 0;
}
