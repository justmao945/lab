
// Definition:
//  Shift  string `s` right `n` bytes.
//  e.g. shift("abcdef", 2) = "efabcd"
// 
// Limit:
//  T = O(n)
//
// Algorithm:
//   1. X and Y are strings, X^T means invert(X). So YX = (X^TY^T)^T
//   2. 

#include<stdio.h>
#include<string.h>

// Invert first `n` bytes of string `s`.
// @param s String to invert.
// @param n Invert first `n` bytes.
// @return 
int invert(char *s, int n)
{
  int i, half;
  char t;
  
  half = n >> 1;
  for(i=0; i<half && s[i]!='\0'; i++){
    t = s[i];
    s[i] = s[n-i-1];
    s[n-i-1] = t;
  }
  return 0;
}


// Shift stirng `s` right `n`  bytes. `n` can be greater than
// the length of `s` and when `n` is negative, which means shift left.
// @param s String to shift, end with '\0'.
// @param n N bytes to shift right.
// @return
int shift(char *s, int n)
{
  int len, neg = 0;

  len = strlen(s);
  if ( n < 0 ){
    neg = 1;
    n = -n;
  }
  n %= len;
  
  if(neg) {
    invert(s, n); // X^T
    invert(s+n, len-n); // Y^T
  }else{
    invert(s, len-n);
    invert(s+len-n, n);
  }
  invert(s, len);  // (X^TY^T)^T
  return 0;
}



int main()
{
  char buf[2048];
  int n;

  printf("Shift string `s` right `n` bytes, enter `s` and `n`\n");
  scanf("%2048s%i", buf, &n);
  shift(buf, n);
  printf("result: %s\n", buf);
  return 0;
}

