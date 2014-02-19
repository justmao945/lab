
#include <stdio.h>
#include <yacl/object.h>

int main()
{
  int x = 100;
  float y = 1.244f;
  char* s = "hello c";
  printf("%zu %zu %zu\n", Int._hash_(&x), Float._hash_(&y), CString._hash_(&s));
  return 0;
}

