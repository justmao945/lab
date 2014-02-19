//
// Definiton:
//   X and Y are strings, and test if all letters of X are in Y and
//   len(X) is equal to len(Y).
// e.g.  
//   X = "abcde", Y1 = "cdeba", Y2 = "cdz".
//   match(X, Y1) is true.
//   match(X, Y2) is false because of length.
//
// Algorithms:
//  1. Naive way: iter X and Y one by one, then get the result. O(n^2)
//  2. Compare after sorted. O(nlog(n)) + O(n)
//  3. Counter array. O(n)


#include <stdio.h>
#include <string.h>


// Test if `s1` and `s2` are match. Algorithm 3.
// @param s1 s2 Two string to test.
// @return 0 not contain, 1 contain.
int  match(const char *s1, const char *s2)
{
  int s1c[256], s2c[256];
  int i, len_s1, len_s2;

  memset(s1c, 0, sizeof s1c);
  memset(s2c, 0, sizeof s2c);

  len_s1 = strlen(s1);
  len_s2 = strlen(s2);

  if(len_s1 != len_s2)
    return 0;

  for(i=0; i<len_s1; i++){
    s1c[(unsigned)s1[i]]++;
    s2c[(unsigned)s2[i]]++;
  }

  for(i=0; i<256; i++)
    if( s1c[i] != s2c[i] )
      return 0;
  return 1;
}


int main()
{
  char buf[2048], buf1[2048];

  printf("Test if `s1` and `s2` are match, enter `s1` and `s2`\n");
  scanf("%2048s%2048s", buf, buf1);
  printf("ByteMap result: %i\n", match(buf, buf1));
  
  return 0;
}
