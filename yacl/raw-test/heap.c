
#include <stdio.h>
#include <yacl/heap.h>

void pr(const vector_t* v)
{
  int *beg = Vector.begin(v), *end = Vector.end(v);
  while( beg < end ){
    printf("%i ", *beg);
    ++beg;
  }
  putchar('\n');
}

int main()
{
  int x[] = {3, 1 ,4, 1, 5, 9};
  int y;
  vector_t v;
  make(Vector, &v, (object_t*)&Int);
  Vector.append_n(&v, sizeof(x)/sizeof(int), x);
  pr(&v);
  make_heap(Vector.begin(&v), Vector.end(&v), (object_t*)&Int, NULL);
  pr(&v);
  y = 6;
  Vector.push_back(&v, &y);
  pr(&v);
  push_heap(Vector.begin(&v), Vector.end(&v), (object_t*)&Int, NULL);
  pr(&v);
  sort_heap(Vector.begin(&v), Vector.end(&v), (object_t*)&Int, NULL);
  pr(&v);
  clean(Vector, &v);
  return 0;
}


