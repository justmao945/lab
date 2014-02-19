
#include <stdio.h>
#include <yacl/slist.h>

void pr(slist_t* l)
{
  slist_ref_t beg = SList.begin(l), end = SList.end(l);
  while( beg != end ){
    printf("%i ", *(int*)beg->value);
    beg = beg->next;
  }
  putchar('\n');
}

int main()
{
  slist_t l;
  int x[] = {1,2,3,4}, t;

  SList._init_(&l, (object_z*)&Int);
  SList.insert_after_n(&l, SList.before_begin(&l), sizeof(x)/sizeof(int), x);
  pr(&l);
  SList.insert_after_n(&l, SList.before_begin(&l), sizeof(x)/sizeof(int), x);
  pr(&l);
  SList.insert_after_n(&l, SList.before_begin(&l), sizeof(x)/sizeof(int), x);
  pr(&l);
  SList.sort(&l, (cmp_f*)Int._lt_);
  pr(&l);
  SList.reverse(&l);
  pr(&l);
  t = 100;
  SList.resize(&l, 16, &t);
  pr(&l);
  t = 1000;
  SList.push_front(&l, &t);
  pr(&l);
  SList.unique(&l, (cmp_f*)Int._eq_);
  pr(&l);
  t = 1;
  SList.remove(&l, (cmp_f*)Int._eq_, &t);
  pr(&l);
  SList._del_(&l);
  return 0;
}
