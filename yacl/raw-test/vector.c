
#include <stdio.h>
#include <yacl/vector.h>

void pr(const vector_t* v)
{
  int *beg, *end;
  for(beg=Vector.begin(v), end=Vector.end(v); beg != end; ++beg) {
    printf("%i ", *beg);
  }
  putchar('\n');
}



int main()
{
  int x[] = {1,2,3,4};
  vector_t v;
  Vector._init_(&v, (object_z*)&Int);

  Vector.append_n(&v, sizeof(x)/sizeof(int), x);
  pr(&v);
  Vector.append(&v, &v);
  pr(&v);
  Vector.sort(&v, (cmp_f*)Int._lt_);
  pr(&v);
  Vector.reverse(&v);
  pr(&v);
  Vector.push_back(&v, x);
  pr(&v);
  int *ref = Vector.find(&v, (cmp_f*)Int._eq_, &x[1]);
  printf("find: %i @ %p, beg:%p\n", *ref, ref, Vector.begin(&v));
  Vector._del_(&v);
  return 0;
}

