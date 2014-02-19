#include <stdio.h>
#include <yacl/list.h>

void pr(list_t* l)
{
  list_io_t lo;

/*  list_ref_t beg = List.begin(l), end = List.end(l);
  while( beg != end ){
    printf("%i ", *(int*)beg->value);
    beg = beg->next;
  }
  putchar('\n'); */

  // here `stdout` can be any other `stdio_t` or its *successor*.
  make(ListIO, &lo, stdout, NULL);
  ListIO.write(l, &lo, (io_z*)&Stdio.Dec.Int);
  putchar('\n');
}

void pr_str(list_t* l)
{
  list_io_t lo;

/*  list_ref_t beg = List.begin(l), end = List.end(l);
  while( beg != end ){
    printf("%s ", *(cstring_t*)beg->value);
    beg = beg->next;
  }
  putchar('\n');
  */
  make(ListIO, &lo, stdout, NULL);
  ListIO.write(l, &lo, (io_z*)&Stdio.CString);
  putchar('\n');
}

int main()
{
  list_io_t li;
  list_t l;
  int x[] = {1,2,3,4}, t;

  make(List, &l, (object_z*)&Int);
  List.insert_n(&l, List.begin(&l), sizeof(x)/sizeof(int), x);
  pr(&l);
  List.insert_n(&l, List.begin(&l), sizeof(x)/sizeof(int), x);
  pr(&l);
  List.insert_n(&l, List.begin(&l), sizeof(x)/sizeof(int), x);
  pr(&l);
  List.sort(&l, (cmp_f*)Int._lt_);
  pr(&l);
  List.reverse(&l);
  pr(&l);
  t = 100;
  List.resize(&l, 16, &t);
  pr(&l);
  
  make(ListIO, &li, stdin, NULL);
//  printf("read: %i\n", ListIO.read(&l, &li, Stdio.Dec.Int.read));
//  pr(&l);

  clean(List, &l);

  /* test string */
  const char* str[] = {"abc", "efg", "xyz", "hello", "world", "-3-"};
  const char* s = "haha, --#";

  make(List, &l, (object_z*)&CString);
  List.insert_n(&l, List.begin(&l), sizeof(str)/sizeof(cstring_t), str);
  pr_str(&l);
  List.insert_n(&l, List.begin(&l), sizeof(str)/sizeof(cstring_t), str);
  pr_str(&l);
  List.insert_n(&l, List.begin(&l), sizeof(str)/sizeof(cstring_t), str);
  pr_str(&l);
  List.sort(&l, (cmp_f*)CString._lt_);
  pr_str(&l);
  List.reverse(&l);
  pr_str(&l);
  List.resize(&l, 24, &s);
  pr_str(&l);

  clean(List, &l);
  return 0;
}


