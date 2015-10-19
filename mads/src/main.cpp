#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <ctime>
#include "prepare.h"
#include "asm.h"
#include "disasm.h"
#include "sim.h"

using namespace std;

const char* HELP =
    "Usage: mads option file\n"
    "Options:\n"
    "  /s  Assemble an assembler file(.s), output binary(.bin)\n"
    "  /d  Disassemble an binary file(.bin), output asm(.dis.s)\n"
    "  /m  Simulator an binary file(.bin)\n"
    "  /v  MADS version";

// Eh, "-s" this format in Windows cmd.exe will ocurr some Error.
// But in bash is OK, and so change the option postfix into "/"

const char* VER =
  "MADS (MIPS Assembler|Disassembler|Simulator 32bit) 0.2.0";

void simConsole(Sim& sim)
{
  const char* SIM_HELP =
    "MADS Simulator\n"
    "  s  Step run instruction.\n"
    "  a  Run all instructions.\n"
    "  r  Show all registers value\n"
		"  t  Reset simulator\n"
    "  h  Simulator help\n"
		"  m[DOX]  Show memory value in Dec,Octol,Hex address\n"
		"          Examples: m0xa, m012, m10";

  cout<<SIM_HELP<<endl;
  
  char chCMD;
  while( cout<<'-', cin>>chCMD ){
    switch(chCMD){
      case 's':
      {
        sim.step();
        cout<<"$pc:0x"<<hex<<setw(8)<<setfill('0')<<sim.getPC()<<' '
            <<"Line:"<<dec<< ( (sim.getPC()>>2)+1) <<':'<<sim.getCurrentInstr()
            <<endl;
        break;
      }
      case 'a':
      {
        cout<<"Runing all..."<<endl;
        sim.runAll();
        cout<<"Done..."<<endl;
      }
      case 'r':
      {
        const int* regFile = sim.getRegFile();
        const char** regName = sim.getRegNameArray();
        cout<<hex;
        for ( u32 i=0; i<32; i++ ) {
          cout<<regName[i]<<":0x"<<setw(8)<<setfill('0')<<regFile[i];
          if( (i+1)%4 ==0 )
            cout<<endl;
          else
            cout<<' ';
        }
        cout<<"$pc:0x"<<setw(8)<<setfill('0')<<sim.getPC()<<' '
            <<"$hi:0x"<<setw(8)<<setfill('0')<<sim.getHI()<<' '
            <<"$lo:0x"<<setw(8)<<setfill('0')<<sim.getLO()<<endl;
        cout<<dec;
        break;
      }
      case 'm':
      {
				string strAddr;
        strAddr.reserve(20);
        u32 uAddr;
				cin>>strAddr;
				if(strAddr.empty())
					strAddr.push_back('0');
        try{
				  uAddr = u32(str2int(C(strAddr)));
        }catch(TypeConvertErr){
          cout<<"Address format error."<<endl;
          break;
        }catch(TypeConvertOverflow){
          cout<<"Address overflow."<<endl;
          break;
        }
				cout<<"Memory[0x"<<setw(8)<<setfill('0')<<hex<<uAddr<<"] = 0x"
					  <<setw(8)<<setfill('0')<<sim.getMemVal(uAddr)<<dec<<endl;
        break;
      }
      case 't':
      {
        sim.reset();
        break;
      }
      default:
        cout<<SIM_HELP<<endl;
    }
  }

}

bool calcTime(double& dTime,
    AObject& a,
    const char* szFileName, 
    const char* szDisFileName = 0)
{
  bool bRet;
  clock_t c1, c2;
  c1 = clock();
  bRet = a.run(szFileName, szDisFileName);
  c2 = clock();
  dTime = static_cast<double>(c2-c1)/CLOCKS_PER_SEC*1000;  // ms
  return bRet;
}


int main(int argc, const char *argv[])
{
  if( argc < 2 ||
      (argc == 2 && argv[1][1] != 'v') ||
      strlen(argv[1]) != 2 ||
      argv[1][0] != '/' ){
    cout<<HELP<<endl;
    return 0;
  }

  bool s1, s2;
  double t1, t2;
  switch( argv[1][1] ){
    case 's':
    {
      Prepare pre;
      Asm as;
      pre.init();
	    as.init();
      Logger& logger = AObject::getLogger();
      logger.clear();
      s1 = calcTime(t1,pre,argv[2]);
      s2 = calcTime(t2,as,pre.getOutFileName());
      cout<<logger<<"Output: "<<endl;
      if(s1)
        cout<<'\t'<<pre.getOutFileName()<<'\t'<<t1<<"ms"<<endl;
      if(s2)
        cout<<'\t'<<as.getOutFileName()<<'\t'<<t2<<"ms"<<endl;
      if(!(s1||s2))
        cout<<"\t No output file."<<endl;
      cout<<"Totoal: "<<t1+t2<<"ms"<<endl;
      break;
    }
    case 'd':
    {
      Disasm disas;
	    disas.init();
      Logger& logger = AObject::getLogger();
      logger.clear();
      s1 = calcTime(t1, disas, argv[2] );
      cout<<logger<<"Output: "<<endl;
      if(s1)
        cout<<'\t'<<disas.getOutFileName()<<'\t'<<t1<<"ms"<<endl;
      else
        cout<<"\t No output file."<<endl;
      cout<<"Totoal: "<<t1<<"ms"<<endl;
      break;
    }
    case 'm':
    {
      Disasm disas;
      Sim sim;
	    disas.init();
	    sim.init();
      Logger& logger = AObject::getLogger();
      s1 = calcTime(t1, disas, argv[2] );
      s2 = calcTime(t2, sim, argv[2], disas.getOutFileName());
      bool bNoErr = logger.e_size() ==0 && logger.u_size()==0;
      cout<<logger<<"Output: "<<endl;
      if(s1)
        cout<<'\t'<<disas.getOutFileName()<<'\t'<<t1<<"ms"<<'\n'
            <<"Simulator init..."<<t2<<"ms"<<endl;
      else
        cout<<"\t No output file."<<endl;
      cout<<"Totoal: "<<t1+t2<<"ms"<<endl;
      if(bNoErr)
        simConsole(sim);
      else
        cout<<"Simlulator down because of Fatal error ocurred"<<endl;
      break;
    }
    case 'v':
    {
      cout<<VER<<endl;
      break;
    }
    default:
      cout<<HELP<<endl;
  }

  return 0;
}
