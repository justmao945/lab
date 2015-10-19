#include <iostream>
#include "prepare.h"
using namespace std;
int main(int argc, const char *argv[])
{
  Prepare p;
/*
  cout<<p.getCurrentLine()<<endl
    <<p.getInFileName()<<endl
    <<p.getOutFileName()<<endl
    <<p.getFileExt()<<endl;
  */
    p.init();
    p.run("../../tst/tst.s");
  cout<<p.getLogger();
  return 0;
}
