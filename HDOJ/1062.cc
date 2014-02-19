#include<cstdio>

#define isblank(x) ((x) == ' ' || (x) == '\t')

void reverse_print(char* str, size_t len)
{
  bool newline = false;
  if(len && str[len-1] == '\n'){
    newline = true;
    --len;
  }
  while(len) putchar(str[--len]);
  if( newline ) putchar('\n');
}

char buf[1005];

int main()
{
  int N, i, j;
  scanf("%i", &N);
  getchar();
  while(N--){
    fgets(buf, 1005, stdin);
    i = 0;
    while( buf[i] != '\0' ){
      while( isblank(buf[i]) ) putchar(buf[i++]);
      j = i;
      while( buf[j] != '\0' && ! isblank(buf[j]) ) ++j;
      reverse_print(buf+i, j-i);;
      i = j;
    }
  }
  return 0;
}
