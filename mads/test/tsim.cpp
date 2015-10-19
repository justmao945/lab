#include <iostream>
#include "sim.h"
using namespace std;
int main(int argc, const char *argv[])
{
  Sim sim;
  sim.init();
  sim.run("tst.bin");
  sim.step();
  const int* regFile = sim.getRegFile();
  const char** regName = sim.getRegNameArray();
  cout<<hex;
  for ( unsigned i=0; i<32; i++ )
    cout<<regName[i]<<' '<<regFile[i]<<endl;
  cout<<dec;

  cout<<sim.getLogger();
  return 0;
}
