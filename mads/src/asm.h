#ifndef _ASSEMBLE_H_
#define _ASSEMBLE_H_

#include <string>
#include <map>
#include "aobject.h"

class Asm: public AObject{
public:
	Asm()NO_THROW
  {
    _uRealCurrentLine = 0;
    _strFileExt = ASM_FILE_EXT;
  }
	~Asm()NO_THROW{}

	void init()NO_THROW;
  void clear()NO_THROW;
  
  bool run(const char* szFileName,
           const char* sz=0)NO_THROW;
  // start compiling , bASCII is true then the output 
  // will print every binary instruction per line in ascii mode.
  // or only in one line in binary mode.

protected:
  u32 _uRealCurrentLine;
  // record real current line number in prepared file
  // flag line not recorded and starting from 1

  std::map<std::string, u32>         _mapJumpFlag;
  // recode Jump flags for Jump type or branch type instructions
  
  std::map<std::string, int32> _mapReg;
  std::map<std::string, int32> _mapRtypeFunc;
  std::map<std::string, int32> _mapItypeOp;

protected:
  u32   _getFlagLine(const std::string& strFlag)NO_THROW;   // get flag line number
  int32 _getRegBin(const std::string& strReg)NO_THROW;      // get register binary code from register's name
  int32 _getRtypeFuncBin(const std::string& strOP)NO_THROW; //get R type Func binary code from instrction's op name
  int32 _getItypeOpBin(const std::string& strOP)NO_THROW;
  // get I type Op binray code from instr's op name

  int32 _getRtypeBin(
      const std::string& strOP,
      const std::string& strRD,
      const std::string& strRS,
      const std::string& strRT,
      const std::string& strShamt="0")NO_THROW;
  // get R type instr binary code
  
  int32 _getItypeBin(
      const std::string& strOP,
      const std::string& strRT,
      const std::string& strRS,
      const std::string& strImm)NO_THROW;
  // get I type instr binary code

  int32 _getJtypeBin(
      const std::string& strOp,
      const std::string& strAddr)NO_THROW;
  // get J type instr binary code
};

#endif
