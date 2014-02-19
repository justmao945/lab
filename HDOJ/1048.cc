#include<cstdio>
#include<cstring>

static const char* dict = "VWXYZABCDEFGHIJKLMNOPQRSTU";
char buf[4096];

int main()
{
  while(1){
    fgets(buf, 4096, stdin);
    if( strncmp("ENDOFINPUT\n", buf, 11) == 0 )
      break;
    else if( strncmp("START\n", buf, 6) != 0 && strncmp("END\n", buf, 4) != 0 ){ // cipher
      for(size_t i=0; buf[i]!='\0'; ++i){
        if(buf[i] >= 'A' && buf[i] <= 'Z')
          putchar(dict[buf[i] - 'A']);
        else
          putchar(buf[i]);
      }
    }
  } 
  return 0;
}
