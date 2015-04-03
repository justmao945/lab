#include <iostream>
#include <cstdlib>

using namespace std;

int intcmp(const void*a, const void*b) {
  return *((int*)a) - *((int*)b);
}

static int x[1 << 25];

int main(int argc, char* argv[]) {
  int n = atoi(argv[1]), i;
  cout << n << endl;
  for(i = 0; i < n; i++) {
    x[i] = i;
  }
  for(; i < 2*n; i++) {
    x[i] = 2*n - i - 1;
  }

  qsort(x, 2*n, sizeof(int), intcmp);

  return 0;
}
