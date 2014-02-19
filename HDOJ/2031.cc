#include<cstdio>
#include<stack>

int main()
{
  int dec, base, dig;
  std::stack<char> stk;

  while( scanf("%i%i", &dec, &base) != EOF ){
    if(dec < 0){
      putchar('-');
      dec = -dec;
    }

    while(dec){
      dig = dec % base;
      if(dig < 10)
        stk.push('0' + dig);
      else
        stk.push('A' + dig - 10);
      dec /= base;
    }
    while( ! stk.empty() ){
      putchar( stk.top() );
      stk.pop();
    }
    putchar('\n');
  }
  return 0;
}
