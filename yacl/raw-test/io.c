#include <yacl/io.h>

int main()
{
  double f;
  char buf[100] = "3123\r13\n1\f3";
  char * s = buf;
  Stdio.Double.read(&f, stdin);
  Stdio.Double.write(&f, stdout);
  putchar('\n');
  puts(s);
  Stdio.CString.write(&s, stdout);
  putchar('\n');
  while( Stdio.CString.read(&s, stdin) != EOF ){
    puts(s);
    Stdio.CString.write(&s, stdout);
    putchar('\n');
  }

  char c;
  while( Stdio.Char.read(&c, stdin) != EOF ){
    getchar();
    Stdio.Char.write(&c, stdout);
  }

  return 0;
}

