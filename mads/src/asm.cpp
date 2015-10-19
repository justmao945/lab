#include "asm.h"

void
Asm::init()NO_THROW
{
  // Initialize reg binary code
  for ( u32 i=0; i<_REG_AZ; i++ )
    _mapReg[_RegArray[i]] = i;

  // Initialize R-type Func binary code
  for ( u32 i=0; i<_R_AZ; i+=3 )
    _mapRtypeFunc[_RtypeArray[i]] = bin2int(_RtypeArray[i+1]);

  // Initialize I-type Operate binary code
  for ( u32 i=0; i<_I_AZ; i+=3 )
    _mapItypeOp[_ItypeArray[i]] = bin2int(_ItypeArray[i+1]);
}

void
Asm::clear()NO_THROW
{
  AObject::clear();
  _uRealCurrentLine = 0;
  _mapJumpFlag.clear();
}

u32
Asm::_getFlagLine(const std::string& strFlag)NO_THROW
{
  std::map<std::string, u32>::const_iterator
      it = _mapJumpFlag.find(strFlag);
  if(it != _mapJumpFlag.end())
    return it->second;
  else
    _Log(ERR, "Jump flag name error -->" + strFlag);
  return 0;
}

int32
Asm::_getRegBin(const std::string& strReg)NO_THROW
{
  std::map<std::string, int32>::const_iterator
      itReg = _mapReg.find(strReg);
  if(itReg != _mapReg.end())
    return itReg->second;
  else
    _Log(ERR, "Register name error -->" + strReg);
  return 0;
}

int32
Asm::_getRtypeFuncBin(const std::string& strOP)NO_THROW
{
  std::map<std::string, int32>::const_iterator
      itOP = _mapRtypeFunc.find(strOP);
  if(itOP != _mapRtypeFunc.end())
    return itOP->second;
  else
    _Log(ERR, "Operator name error -->"+strOP);
  return 0;
}

int32
Asm::_getItypeOpBin(const std::string& strOP)NO_THROW
{
  std::map<std::string, int32>::const_iterator
      itOP = _mapItypeOp.find(strOP);
  if(itOP != _mapItypeOp.end())
    return itOP->second;
  else
    _Log(ERR, "Operator name error -->"+strOP);
  return 0;
}


int32
Asm::_getRtypeBin(
    const std::string& strOP,
    const std::string& strRD,
    const std::string& strRS,
    const std::string& strRT,
    const std::string& strShamt)NO_THROW
{
  int32 nShamt;
  try{
    nShamt = _str2int(C(strShamt));
  }catch(TypeConvertErr&){
    _Log(ERR, "TypeConvertErr.-->"+strShamt);
    nShamt = 0;
  }
  
  return (_getRegBin(strRS)<<21) |
      (_getRegBin(strRT)<<16) |
      (_getRegBin(strRD)<<11) |
      (nShamt <<6 )|
      _getRtypeFuncBin(strOP);
}
  
int32 
Asm::_getItypeBin(
    const std::string& strOP,
    const std::string& strRT,
    const std::string& strRS,
    const std::string& strImm)NO_THROW
{
  int32 nImmBin;

  if( (!strImm.empty() && strImm[0]>='0' && strImm[0]<='9') ||
      (strImm.size()>=2 && strImm[0]=='-' && strImm[1]>='0' && strImm[1]<='9')
      ) // strImm is a number
    nImmBin = _str2int(C(strImm));
  else // strImm is a Jump flag
    nImmBin = (int32)(_getFlagLine(strImm) - _uRealCurrentLine) -1;

  return (_getItypeOpBin(strOP)<<26) |
      (_getRegBin(strRS)<<21) |
      (_getRegBin(strRT)<<16) |
      (nImmBin & 0xffff) ;
}

int32
Asm::_getJtypeBin(const std::string& strOp, const std::string& strAddr)NO_THROW
{
  int32 nOpBin, nAddrBin;
  u32 i;
  for ( i=0; i<_J_AZ; i+=3 )
    if(strOp == _JtypeArray[i]) {
      nOpBin = bin2int(_JtypeArray[i+1]);
      break;
    }
  if( i == _J_AZ )
    _Log(ERR, "Instrction format error -->" + strOp);

  if(strAddr[0] >= '0' && strAddr[0] <= '9') // strAddr is a number
    nAddrBin = _str2int(C(strAddr));
  else { // strAddr is a Jump flag
    nAddrBin = _getFlagLine(strAddr) - (_uRealCurrentLine&0xf0000000) - 1;
    if(nAddrBin < 0)
      _Log(ERR, "Jump out of scale.");
  }
  return (nOpBin<<26)| nAddrBin;
}

bool
Asm::run(const char* szFileName, const char* sz)NO_THROW
{
  if(isLoggerHErr())
    return false;

  if( !_loadFromFile(szFileName) )
    return false;
  
  std::string strLine, strInstr;
  std::vector<std::string> vecInstr;

  while( _sin >> strLine ) { // First deal all jump flags
    _uRealCurrentLine++;
    if( *strLine.rbegin() == ':' ) { // Jump flag Line
      split(vecInstr, C(strLine));
      _mapJumpFlag[rstrip(C(vecInstr[1]), ':')] = _uRealCurrentLine;
    }
  }

  _sin.clear();
  _uRealCurrentLine = 0;
  int32 nInstr = -1;
  while( _sin >> strLine ) {
    _uRealCurrentLine++;
    split(vecInstr, C(strLine));
    _uCurrentLine = _str2u(C(vecInstr[0]));
    if( *strLine.rbegin() == ':' ) {// Jump flag Line, do nothing
        nInstr = _getRtypeBin("sll", "$at", "$zero", "$at", "0");
    }else {
      switch(vecInstr.size()){
        case 6:
        {
          nInstr = _getRtypeBin(vecInstr[1],vecInstr[2],vecInstr[3],vecInstr[4],vecInstr[5] );
          break;
        }
        case 5:
        {
          nInstr = _getItypeBin(vecInstr[1],vecInstr[2],vecInstr[3],vecInstr[4] );
          break;
        }
        case 3:
        {
          nInstr = _getJtypeBin( vecInstr[1], vecInstr[2] );
          break;
        }
        default:
          _Log(ERR, "Instruction format error");
          break;
      }
    }
    _sout<<((nInstr>>24)&0xff)<<((nInstr>>16)&0xff)<<((nInstr>>8)&0xff)<<(nInstr&0xff);
  }

  clear();
  if(isLoggerHErr())
    return false;
  return true;
}
