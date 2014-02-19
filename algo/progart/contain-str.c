//
// Definiton:
//   X and Y are strings and test if all letters of Y are in X.
// e.g.  
//   X = "abcde", Y1 = "cd", Y2 = "cdz".
//   contain(X, Y1) is true.
//   contain(X, Y2) is false because of `z`.
//
// Algorithms:
//  1. Naive way: iter X and Y one by one, then get the result. O(m*n)
//  2. Compare after sorted. O(mlog(m)) + O(nlog(n)) + O(m+n)
//  3. ByteMap compare. Need two arrays(256) to store all count. O(m+n)
//


#include <stdio.h>
#include <string.h>

// ByteMap compare.
int  contain_3(const char *s, const char *sub)
{
  char sc[256];
  int i, len_s, len_sub;

  memset(sc, 0, sizeof sc);

  len_s  = strlen(s);
  len_sub = strlen(sub);

  for(i=0; i<len_s; i++)
    sc[(unsigned)s[i]] = 1;

  for(i=0; i<len_sub; i++)
    if( sc[(unsigned)sub[i]] ==0 )
      return 0;
  return 1;
}


// Test if all letters of `sub` are in `s`
// @param s Original string to be tested.
// @param sub Sub-string to test.
// @return 0 not contain, 1 contain.
int  contain(const char *s, const char *sub)
{

  return 0;
}


int main()
{
  char buf[2048], buf1[2048];

  printf("Test if all letters of `sub` are in `s`, enter `s` and `sub`\n");
  scanf("%2048s%2048s", buf, buf1);
  printf("ByteMap result: %i\n", contain_3(buf, buf1));
  
  return 0;
}
