#include "sim.h"

void (*Sim::_RtypeFuncArray[])(Sim::_Instr&, Sim&) =
{
  Sim::_add,  Sim::_addu, Sim::_and,
  Sim::_div,  Sim::_divu, Sim::_jr,
  Sim::_mfhi, Sim::_mflo, Sim::_mult,
  Sim::_multu,Sim::_or,   Sim::_sll,
  Sim::_sllv, Sim::_slt,  Sim::_sltu,
  Sim::_sra,  Sim::_srl,  Sim::_srlv,
  Sim::_sub,  Sim::_subu, Sim::_syscall,
  Sim::_xor,
};

void (*Sim::_ItypeFuncArray[])(Sim::_Instr&, Sim&) =
{
  Sim::_addi, Sim::_addiu,
  Sim::_andi, Sim::_beq,
  Sim::_bne,  Sim::_lb,
  Sim::_lw,   Sim::_lui,
  Sim::_ori,  Sim::_sb,
  Sim::_slti, Sim::_sltiu,
  Sim::_sw,   Sim::_xori,
  Sim::_j,    Sim::_jal,
};

void (*Sim::_JtypeFuncArray[])(Sim::_Instr&, Sim&) =
{
  Sim::_j,    Sim::_jal,
};


void 
Sim::init()NO_THROW
{
  for ( u32 i=0; i<_R_AZ; i+=3 )
    _mapRtypeFunc[_bin2u(_RtypeArray[i+1])] = _RtypeFuncArray[i/3];
  
  for ( u32 i=0; i<_I_AZ; i+=3 )
    _mapIJtypeFunc[_bin2u(_ItypeArray[i+1])] = _ItypeFuncArray[i/3];
  
  for ( u32 i=0; i<_J_AZ; i+=3 )
    _mapIJtypeFunc[_bin2u(_JtypeArray[i+1])] = _JtypeFuncArray[i/3];
}

void 
Sim::clear()NO_THROW
{
  AObject::clear();
  reset();
}

bool
Sim::run(const char* szFileName, const char* szDisFileName)NO_THROW
{
  if(isLoggerHErr())
    return false;

  if( !_loadFromFile(szFileName) )
    return false;
  u32 uInstr;
  static const u32 IS_AZ = 4;
  char insArray[IS_AZ];
  _Instr ins;

  while( _sin.read(insArray, IS_AZ) == IS_AZ ){
    uInstr = 0;
    for ( u32 i=0; i<IS_AZ; i++ ) 
      uInstr |= (u32(insArray[i]) & 0xff)<<(8*(IS_AZ-i-1));

    ins.s = &_RegFile[(uInstr>>21)&0x1f];
    ins.t = &_RegFile[(uInstr>>16)&0x1f];
    ins.d = &_RegFile[(uInstr>>11)&0x1f];
    ins.uShamt = (uInstr>>6)&0x1f;
    ins.uImm = uInstr & 0xffff;
    ins.uAddr = uInstr&0x03ffffff;
      
    if( (uInstr>>26) == 0 ) // R type
      ins._exe = getRtypeFunc( uInstr&0x3f );
    else // I or J type
      ins._exe = getIJtypeFunc( uInstr>>26 );
    _vecInstr.push_back(ins);
  }
  closeFile();
  
  if(szDisFileName){  // have disassemble file
    StorageIn iDisFile;
    
    if( !iDisFile.open(szDisFileName) )
      _Log(WAR, str("Unable to open file: %s", szDisFileName));
    else{
      std::string strLine;
      while ( iDisFile >> strLine )
        _vecInstrString.push_back(strLine);
      iDisFile.close();
    }
  }
  
  clear();
  if(isLoggerHErr())
    return false;
  return true;
}

  
void 
Sim::reset()NO_THROW
{
  _RegFile[0] = 0;
  _uPC = _uOPC = 0;
  _mapMem.clear();
}


bool
Sim::step()NO_THROW
{
  if( (_uPC>>2) < _vecInstr.size() ) {
    _vecInstr[_uPC>>2].exe(*this);
    return true;
  }
  return false;
}

void
Sim::runAll()NO_THROW
{
  while( (_uPC>>2) < _vecInstr.size() )
    _vecInstr[_uPC>>2].exe(*this);
}

const u32&
Sim::getPC()NO_THROW
{ return _uOPC; }

const int32&
Sim::getHI()NO_THROW
{ return _nHI; }

const int32&
Sim::getLO()NO_THROW
{ return _nLO; }

const int32 *
Sim::getRegFile()NO_THROW
{ return _RegFile; }

const char**
Sim::getRegNameArray()NO_THROW
{ return _RegArray; }

const char* 
Sim::getCurrentInstr()NO_THROW
{ 
  if( (_uOPC>>2) < _vecInstrString.size() )
    return _vecInstrString[_uOPC>>2].c_str(); 
  else
    return 0;
}

int32 
Sim::getMemVal(const u32& uAddr)NO_THROW
{
  std::map<u32, int32>::const_iterator
      it = _mapMem.find(uAddr);
  if(it != _mapMem.end())
    return it->second;
  else
    return 0;
}

int32 
Sim::getMemVal(Sim& sim, const u32& uAddr)NO_THROW
{
  std::map<u32, int32>::const_iterator
      it = sim._mapMem.find(uAddr);
  if(it != sim._mapMem.end())
    return it->second;
  else
    return 0;
}


void (* Sim::getRtypeFunc(const u32& uFunc)NO_THROW )(Sim::_Instr&, Sim&)
{ 
  // return R type function if found in mapRtypeFunc
  std::map<u32, void(*)(_Instr&, Sim&)>::const_iterator
      it = _mapRtypeFunc.find(uFunc);
  if(it != _mapRtypeFunc.end())
    return it->second;
  else{
    _Log(ERR, str("Can't get Rtype function.-->%u", uFunc));
    return _mapRtypeFunc.begin()->second;
  }
}

void (* Sim::getIJtypeFunc(const u32& uOp)NO_THROW )(Sim::_Instr&, Sim&)
{
  // return IJ type function if found in mapIJtypeFunc
  std::map<u32, void(*)(_Instr&, Sim&)>::const_iterator
      it = _mapIJtypeFunc.find(uOp);
  if(it != _mapIJtypeFunc.end())
    return it->second;
  else{
    _Log(ERR, str("Can't get I or Jtype function.-->%u", uOp));
    return _mapIJtypeFunc.begin()->second;
  }
}


void 
Sim::advancePC(Sim& sim, int32 nOffset)NO_THROW
{
  sim._uOPC = sim._uPC;
  sim._uPC += nOffset;
}
  
void
Sim::_add(_Instr& ins, Sim& sim)NO_THROW
{
  *ins.d = *ins.s + *ins.t;
  advancePC(sim);
}

void
Sim::_addu(_Instr& ins, Sim& sim)NO_THROW
{
  *ins.d = u32(*ins.s) + u32(*ins.t);
  advancePC(sim);
}

void
Sim::_and(_Instr& ins, Sim& sim)NO_THROW
{
  *ins.d = *ins.s & *ins.t;
  advancePC(sim);
}


void
Sim::_div(_Instr& ins, Sim& sim)NO_THROW
{
  sim._nLO = *ins.s / *ins.t;
  sim._nHI = *ins.s % *ins.t;
  advancePC(sim);
}

void
Sim::_divu(_Instr& ins, Sim& sim)NO_THROW
{
  sim._nLO = u32(*ins.s) / u32(*ins.t);
  sim._nHI = u32(*ins.s) % u32(*ins.t);
  advancePC(sim);
}

void
Sim::_mfhi(_Instr& ins, Sim& sim)NO_THROW
{
  *ins.d = sim._nHI;
  advancePC(sim);
}

void
Sim::_mflo(_Instr& ins, Sim& sim)NO_THROW
{
  *ins.d = sim._nLO;
  advancePC(sim);
}

void
Sim::_mult(_Instr& ins, Sim& sim)NO_THROW
{
  sim._nLO = *ins.s * *ins.t;
  advancePC(sim);
}

void
Sim::_multu(_Instr& ins, Sim& sim)NO_THROW
{
  sim._nLO = u32(*ins.s) * u32(*ins.t);
  advancePC(sim);
}

void
Sim::_or(_Instr& ins, Sim& sim)NO_THROW
{
  *ins.d = *ins.s | *ins.t;
  advancePC(sim);
}

void
Sim::_sll(_Instr& ins, Sim& sim)NO_THROW
{
  *ins.d = *ins.t << ins.uShamt;
  advancePC(sim);
}

void
Sim::_sllv(_Instr& ins, Sim& sim)NO_THROW
{
  *ins.d = *ins.t << *ins.s;
  advancePC(sim);
}

void
Sim::_slt(_Instr& ins, Sim& sim)NO_THROW
{
  *ins.d = *ins.s < *ins.t ? 1 : 0;
  advancePC(sim);
}

void
Sim::_sltu(_Instr& ins, Sim& sim)NO_THROW
{
  *ins.d = u32(*ins.s) < u32(*ins.t) ? 1 : 0;
  advancePC(sim);
}

void
Sim::_sra(_Instr& ins, Sim& sim)NO_THROW
{
  *ins.d = (int32)*ins.t >> ins.uShamt;
  advancePC(sim);
}

void
Sim::_srav(_Instr& ins, Sim& sim)NO_THROW
{
  *ins.d = (int32)*ins.t >> *ins.s;
  advancePC(sim);
}

void
Sim::_srl(_Instr& ins, Sim& sim)NO_THROW
{
  *ins.d = *ins.t >> ins.uShamt;
  advancePC(sim);
}

void
Sim::_srlv(_Instr& ins, Sim& sim)NO_THROW
{
  *ins.d = *ins.t >> *ins.s;
  advancePC(sim);
}

void
Sim::_sub(_Instr& ins, Sim& sim)NO_THROW
{
  *ins.d = *ins.s - *ins.t;
  advancePC(sim);
}

void
Sim::_subu(_Instr& ins, Sim& sim)NO_THROW
{
  *ins.d = u32(*ins.s) - u32(*ins.t);
  advancePC(sim);
}

void
Sim::_xor(_Instr& ins, Sim& sim)NO_THROW
{
  *ins.d = *ins.s ^ *ins.t;
  advancePC(sim);
}

void
Sim::_syscall(_Instr& ins, Sim& sim)NO_THROW
{}

void
Sim::_jr(_Instr& ins, Sim& sim)NO_THROW
{
  sim._uOPC = sim._uPC;
  sim._uPC = *ins.s;
}

void
Sim::_addi(_Instr& ins, Sim& sim)NO_THROW
{
  *ins.t = *ins.s + int32(int16(ins.uImm));
  advancePC(sim);
}

void
Sim::_addiu(_Instr& ins, Sim& sim)NO_THROW
{
  *ins.t = u32(*ins.s) + ins.uImm;
  advancePC(sim);
}

void
Sim::_andi(_Instr& ins, Sim& sim)NO_THROW
{
  *ins.t = *ins.s & int32(int16(ins.uImm));
  advancePC(sim);
}

void
Sim::_beq(_Instr& ins, Sim& sim)NO_THROW
{
  advancePC(sim, *ins.s == *ins.t ? int32(int16((ins.uImm+1)<<2)) : 4 );
}

void
Sim::_bne(_Instr& ins, Sim& sim)NO_THROW
{
  advancePC(sim, *ins.s != *ins.t ? int32(int16((ins.uImm+1)<<2)) : 4 );
}

void
Sim::_lb(_Instr& ins, Sim& sim)NO_THROW
{
  *ins.t = (signed char)getMemVal(sim, u32(*ins.s+int32(int16(ins.uImm))));
  advancePC(sim);
}

void
Sim::_lw(_Instr& ins, Sim& sim)NO_THROW
{
  *ins.t = getMemVal(sim, u32(*ins.s+int32(int16(ins.uImm))));
  advancePC(sim);
}

void
Sim::_lui(_Instr& ins, Sim& sim)NO_THROW
{
  *ins.t = int32(int16(ins.uImm)) << 16;
  advancePC(sim);
}

void
Sim::_ori(_Instr& ins, Sim& sim)NO_THROW
{
  *ins.t = *ins.s | int32(int16(ins.uImm));
  advancePC(sim);
}

void
Sim::_sb(_Instr& ins, Sim& sim)NO_THROW
{
  sim._mapMem[u32(*ins.s+int32(int16(ins.uImm)))] = *ins.t & 0xff;
  advancePC(sim);
}

void
Sim::_slti(_Instr& ins, Sim& sim)NO_THROW
{
  *ins.d = *ins.s < int32(int16(ins.uImm)) ? 1 : 0;
  advancePC(sim);
}

void
Sim::_sltiu(_Instr& ins, Sim& sim)NO_THROW
{
  *ins.d = u32(*ins.s) < ins.uImm ? 1 : 0;
  advancePC(sim);
}

void
Sim::_sw(_Instr& ins, Sim& sim)NO_THROW
{
  sim._mapMem[u32(*ins.s+int32(int16(ins.uImm)))] = *ins.t;
  advancePC(sim);
}

void
Sim::_xori(_Instr& ins, Sim& sim)NO_THROW
{
  *ins.t = *ins.s ^ int32(int16(ins.uImm));
  advancePC(sim);
}

void
Sim::_j(_Instr& ins, Sim& sim)NO_THROW
{
  sim._uOPC = sim._uPC;
  sim._uPC = (sim._uPC & 0xf0000000) | (ins.uAddr<<2);
}

void
Sim::_jal(_Instr& ins, Sim& sim)NO_THROW
{
  sim._RegFile[31] = sim._uPC + 8;
  sim._uOPC = sim._uPC;
  sim._uPC = (sim._uPC & 0xf0000000) | (ins.uAddr<<2);
}

