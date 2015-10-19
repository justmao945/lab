#ifndef _DISASM_H_
#define _DISASM_H_

#include <string>
#include <map>
#include "aobject.h"

class Disasm: public AObject{
public:
	Disasm()NO_THROW{_strFileExt = DISASM_FILE_EXT;}
	~Disasm()NO_THROW{}
	void init()NO_THROW;

  bool run(const char* szFileName,
    const char* sz=0)NO_THROW;
  // convert binary to MIPS instructions

protected:
  std::map<u32, const char*>  _mapRFunc;
  std::map<u32, const char*>  _mapIJOp;
  std::map<std::string, char> _mapAllOp;

  const char* _getRegName(const u32 uReg)NO_THROW;
  const char* _getRName(const u32 uR)NO_THROW;
  const char* _getIJName(const u32 strIJ)NO_THROW;
};

#endif
