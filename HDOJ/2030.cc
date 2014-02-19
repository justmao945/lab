#include<cstdio>
#include<cstring>

char buf[40960]; // GBK ?? fuck...

int main()
{
  int n, cnt;
  scanf("%i", &n);
  getchar();
  for(int i=0; i<n; ++i){
    fgets(buf, 40960, stdin);
    size_t len = strlen(buf); // bug ?
    cnt = 0;
    for(size_t j=0; j<len; ++j){
      if( (buf[j] & 0x80) == 0x80 )
        ++cnt;
    }
    printf("%i\n", cnt >> 1);
  }
  return 0;
}
