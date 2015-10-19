#ifndef _SIM_H_
#define _SIM_H_

#include <string>
#include <map>
#include <vector>
#include "aobject.h"

class Sim: public AObject{
public:
	Sim()NO_THROW{_bHaveOutFile = false;}
	~Sim() NO_THROW{}

	void init()NO_THROW;
  void clear()NO_THROW;
  
  bool run( const char* szFileName,
            const char* szDisFileName=0)NO_THROW;
  // load file and start simulator, but not run MIPS instructions
  // szDisFileName is the name of disassemble file name
  
  void reset()NO_THROW; // reset the simulator
  
  bool step()NO_THROW;
  // step run code, return false if all instr executed, or true
  
  void  runAll()NO_THROW; // run all code

  const u32&   getPC()NO_THROW; // return $pc register
  const int32& getHI()NO_THROW; // return $hi register
  const int32& getLO()NO_THROW; // return $lo register
  const int32* getRegFile()NO_THROW; // return an int32[32] array
  const char** getRegNameArray()NO_THROW; // return a char*[32] array
  const char*  getCurrentInstr()NO_THROW; // return current instruction
  int32        getMemVal(const u32& nAddr)NO_THROW;

protected:

  class _Instr{
  // a simple class to store instructions
  public:
    void (*_exe)(_Instr&, Sim&);  // _Instrction execute function pointer
    int32 *s, *t, *d; // point to _RegArray[x]
    u32 uImm;
    u32 uShamt, uAddr;

    void exe(Sim& sim){ _exe(*this, sim); } // just for easier to call
  };

  u32 _uPC, _uOPC; // pc register
  int32 _nHI, _nLO; // HI, LO register, get from mfhi, mflo

  int32                     _RegFile[32];
  std::map<u32, int32>      _mapMem;
  std::vector<_Instr>       _vecInstr;
  std::vector<std::string>  _vecInstrString;
  // if exist disassemble file, then read the file
  // and return the instrction when step executed

  std::map<u32, void(*)(_Instr&, Sim&)> _mapRtypeFunc;
  std::map<u32, void(*)(_Instr&, Sim&)> _mapIJtypeFunc;
  //

protected:
  void (* getRtypeFunc(const u32& uFunc)NO_THROW )(_Instr&, Sim&);
  void (* getIJtypeFunc(const u32& uOp)NO_THROW )(_Instr&, Sim&);
  // 
  

  ///////////////////////////////////////////////////
  // Static members
  // ////////////////////////////////////////////////

  static void (*_RtypeFuncArray[])(_Instr&, Sim&);
  static void (*_ItypeFuncArray[])(_Instr&, Sim&);
  static void (*_JtypeFuncArray[])(_Instr&, Sim&);
  // Function arrays

  static void advancePC(Sim& sim, int32 nOffset=4)NO_THROW; // default offset is 4
  static int32 getMemVal(Sim& sim, const u32& nAddr)NO_THROW;

  static void _add(_Instr&, Sim&)NO_THROW;
  static void _addu(_Instr&, Sim&)NO_THROW;
  static void _and(_Instr&, Sim&)NO_THROW;
  static void _div(_Instr&, Sim&)NO_THROW;
  static void _divu(_Instr&, Sim&)NO_THROW;
  static void _mfhi(_Instr&, Sim&)NO_THROW;
  static void _mflo(_Instr&, Sim&)NO_THROW;
  static void _mult(_Instr&, Sim&)NO_THROW;
  static void _multu(_Instr&, Sim&)NO_THROW;
  static void _or(_Instr&, Sim&)NO_THROW;
  static void _sll(_Instr&, Sim&)NO_THROW;
  static void _sllv(_Instr&, Sim&)NO_THROW;
  static void _slt(_Instr&, Sim&)NO_THROW;
  static void _sltu(_Instr&, Sim&)NO_THROW;
  static void _sra(_Instr&, Sim&)NO_THROW;
  static void _srav(_Instr&, Sim&)NO_THROW;
  static void _srl(_Instr&, Sim&)NO_THROW;
  static void _srlv(_Instr&, Sim&)NO_THROW;
  static void _sub(_Instr&, Sim&)NO_THROW;
  static void _subu(_Instr&, Sim&)NO_THROW;
  static void _xor(_Instr&, Sim&)NO_THROW;
  static void _syscall(_Instr&, Sim&)NO_THROW;
  static void _jr(_Instr&, Sim&)NO_THROW;
  static void _addi(_Instr&, Sim&)NO_THROW;
  static void _addiu(_Instr&, Sim&)NO_THROW;
  static void _andi(_Instr&, Sim&)NO_THROW;
  static void _beq(_Instr&, Sim&)NO_THROW;
  static void _bne(_Instr&, Sim&)NO_THROW;
  static void _lb(_Instr&, Sim&)NO_THROW;
  static void _lw(_Instr&, Sim&)NO_THROW;
  static void _lui(_Instr&, Sim&)NO_THROW;
  static void _ori(_Instr&, Sim&)NO_THROW;
  static void _sb(_Instr&, Sim&)NO_THROW;
  static void _slti(_Instr&, Sim&)NO_THROW;
  static void _sltiu(_Instr&, Sim&)NO_THROW;
  static void _sw(_Instr&, Sim&)NO_THROW;
  static void _xori(_Instr&, Sim&)NO_THROW;
  static void _j(_Instr&, Sim&)NO_THROW;
  static void _jal(_Instr&, Sim&)NO_THROW;
};


#endif

