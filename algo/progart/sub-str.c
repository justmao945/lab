//
// Definiton:
//   X and Y are strings, and tests if string Y is a substring of X.
//    *same as <string.h>::strstr
// e.g.  
//   X = "abcde", Y1 = "cd", Y2 = "cdz".
//   substr(X, Y1) is true.
//   substr(X, Y2) is false because of `z`.
//
// Algorithms:
//  1. Naive way: iter X and Y one by one, then get the result. O(m*n)
//  2. KMP(knuth-Morris-Pratt). O(n+m).


#include <stdio.h>
#include <string.h>


//  
// Test if `sub` is a substring of `s`.
// @param s Original string to be tested.
// @param sub Sub-string to test.
// @return 0 not contain, 1 contain.
int  substr(const char *s, const char *sub)
{
  
  return 0;
}



int main()
{
  char buf[2048], buf1[2048];

  printf("Test if `sub` is a substring of `s`, enter `s` and `sub`\n");
  scanf("%2048s%2048s", buf, buf1);
  printf("result: %i\n", contain_3(buf, buf1));
  
  return 0;
}
