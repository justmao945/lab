#include "disasm.h"

void 
Disasm::init()NO_THROW
{
  _createAllOpMap(_mapAllOp);

  // init Func binary code to R type
  for ( u32 i=0; i<_R_AZ; i+=3 )
    _mapRFunc[_bin2u(_RtypeArray[i+1])] = _RtypeArray[i];

  // init Op binary code to I J type
  for ( u32 i=0; i<_I_AZ; i+=3 )
    _mapIJOp[_bin2u(_ItypeArray[i+1])] = _ItypeArray[i];
  
  for ( u32 i=0; i<_J_AZ; i+=3 )
    _mapIJOp[_bin2u(_JtypeArray[i+1])] = _JtypeArray[i];
}

const char*
Disasm::_getRegName(const u32 uReg)NO_THROW
{
  if( uReg < _REG_AZ )
    return _RegArray[uReg];
  else
    _Log(ERR, str("Register binary code error.-->%u", uReg));
  return 0;
}

const char*
Disasm::_getRName(const u32 uR)NO_THROW
{
  std::map<u32, const char*>::const_iterator
      it = _mapRFunc.find(uR);
  if(it != _mapRFunc.end())
    return it->second;
  else
    _Log(ERR, str("R type Func binary code error.-->%u",uR));
  return _mapRFunc.begin()->second;
}

const char*
Disasm::_getIJName(const u32 uIJ)NO_THROW
{
  std::map<u32, const char*>::const_iterator
      it = _mapIJOp.find(uIJ);
  if(it != _mapIJOp.end())
    return it->second;
  else
    _Log(ERR, str("I type or J type Op binary code error.-->%u", uIJ));
  return _mapIJOp.begin()->second; 
}

bool
Disasm::run(const char* szFileName, const char* sz)NO_THROW
{
  if(isLoggerHErr())
    return false;

  if( !_loadFromFile(szFileName) )
    return false;
  u32 uInstr;
  static const u32 IS_AZ = 4;
  char insArray[IS_AZ];

  while( _sin.read(insArray, IS_AZ) == IS_AZ ){
    uInstr = 0;
    for ( u32 i=0; i<IS_AZ; i++ ) 
      uInstr |= (u32(insArray[i]) & 0xff)<<(8*(IS_AZ-i-1));

    const char* szRs = _getRegName( (uInstr>>21)&0x1f );
    const char* szRt = _getRegName( (uInstr>>16)&0x1f );
    const char* szRd = _getRegName( (uInstr>>11)&0x1f );
    
    u32 uShamt = (uInstr>>6)&0x1f;
    u32 uImm = uInstr & 0xffff;
    u32 uAddr = uInstr&0x03ffffff;
    
    int32 nImm = int32(int16(uImm));
    
    if( (uInstr>>26) == 0){  // R type
      const char* szName = _getRName(uInstr&0x3f);
      std::string strName(szName);
      char op = _getOp(_mapAllOp, strName );
      
      switch(op){
        case 'r':
          _sout<<szName<<' '<<szRd<<", "<<szRs<<", "<<szRt<<'\n';
          break;
        case 's':
          _sout<<szName<<' '<<szRd<<", "<<szRt<<", "<<u2str(uShamt)<<'\n';
          break;
        case 'm':
          _sout<<szName<<' '<<szRs<<", "<<szRt<<'\n';
          break;
        case 'x':
        {
          if(strName == "jr")
            _sout<<szName<<' '<<szRs<<'\n';
          else if(strName == "mfhi" || strName == "mflo")
            _sout<<szName<<' '<<szRd<<'\n';
          else if(strName == "syscall")
            _sout<<szName<<'\n';
          break;
        }
        default:
            _Log(ERR, "R type Func binary code error");
      }
    }
    else{ // I type or J type
      const char* szName = _getIJName(uInstr>>26);
      std::string strName(szName);
      char op = _getOp( _mapAllOp, strName );

      switch (op) {
        case 'i':
          if(*(strName.end() - 1) == 'u') // u32 and signed
            _sout<<szName<<' '<<szRt<<", "<<szRs<<", "<<u2str(uImm)<<'\n';
          else
            _sout<<szName<<' '<<szRt<<", "<<szRs<<", "<<int2str(nImm)<<'\n';
          break;
        case 'l':
          _sout<<szName<<' '<<szRt<<", "<<int2str(nImm)<<'('<<szRs<<")\n";
          break;
        case 'x':
          _sout<<"lui "<<szRt<<", "<<int2str(nImm)<<'\n';
          break;
        case 'j':
          _sout<<"j "<<u2str(uAddr)<<'\n';
          break;
        default:
          _Log(ERR, "I or J type Op binary code error");
      }
    }
  }
  clear();
  if(isLoggerHErr())
    return false;
  return true;
}
