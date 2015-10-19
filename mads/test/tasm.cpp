#include <iostream>
#include "asm.h"
#include "except.h"
using namespace std;
int main(int argc, const char *argv[])
{
  Asm a;
/*  cout<<a.getCurrentLine()<<endl
    <<a.getInFileName()<<endl
    <<a.getOutFileName()<<endl
    <<a.getFileExt()<<endl;
  */
  a.init();
  a.run("tst.pre.s");
  cout<<a.getLogger();
  return 0;
}
