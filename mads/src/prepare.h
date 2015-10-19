#ifndef _PREPARE_H_
#define _PREPARE_H_

/*
 * Prepare for assembly
 * 1. Trim comments, blank lines and rundancy spaces
 * 2. Convert all instruments into lower case
 * 3. Convert pseudo instruments to inner instruments
 * 4. Add original line number(for reporting error)
*/

#include <string>
#include "aobject.h"

class Prepare: public AObject{
public:
  Prepare()NO_THROW{_strFileExt = PREPARE_FILE_EXT;}
	~Prepare()NO_THROW{}
  
  void init()NO_THROW;
  bool run(const char* szFileName, 
           const char* sz=0)NO_THROW;
  // statring preprocess

protected:
  std::map<std::string, char> _mapAllOp;
  void _convPseudoInstr(const std::string& strInstr)NO_THROW;
  /* convert fake instr into inner instr
     "bgez, bgtz, blez, bltz, noop" are fake instr
     "div, divu, jal, lb, lui, lw, mfhi, mflo, mult,
     multu, sb, sll, sra, srl, sw, syscall, jr "
     and all R type instr need to be processed:
     R-type: LineNum OP $rd $rs $rt Shamt
     I-type: LineNum OP $rt $rs offset
  */
};


#endif
