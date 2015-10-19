#include <iostream>
#include "disasm.h"
using namespace std;
int main(int argc, const char *argv[])
{
  Disasm p;
/*
  cout<<p.getCurrentLine()<<endl
    <<p.getInFileName()<<endl
    <<p.getOutFileName()<<endl
    <<p.getFileExt()<<endl;
  */
  p.init();
    p.run("tst.bin");
  cout<<p.getLogger();
  return 0;
}
