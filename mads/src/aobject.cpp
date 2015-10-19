#include "aobject.h"

Logger AObject::_logger;
bool   AObject::_isLoggerOpen = false;

const char*
AObject::_RegArray[] = 
{ 
  // Store  32 registers'name, and the order 
  // indicate the postion of the register
  "$zero", "$at", "$v0", "$v1",
  "$a0", "$a1", "$a2", "$a3",
  "$t0", "$t1", "$t2", "$t3",
  "$t4", "$t5", "$t6", "$t7",
  "$s0", "$s1", "$s2", "$s3",
  "$s4", "$s5", "$s6", "$s7",
  "$t8", "$t9", "$k0", "$k1",
  "$gp", "$sp", "$fp", "$ra",
};  // here use $fp to replace $s8...so not support $s8

const char* 
AObject::_RtypeArray[] = 
{ 
  // R-type instructions' op = 00000, shamt = 00000 or hhhhh, 
  // but different Func code
  // opName, Func,  Type (r: normal; m: mult/div; s: shift; x: extend)
  "add",    "100000", "r",
  "addu",   "100001", "r",
  "and",    "100100", "r",
  "div",    "011010", "m",
  "divu",   "011011", "m",
  "jr",     "001000", "x",
  "mfhi",   "010000", "x",
  "mflo",   "010010", "x",
  "mult",   "011000", "m",
  "multu",  "011001", "m",
  "or",     "100101", "r",
  "sll",    "000000", "s",
  "sllv",   "000100", "r",
  "slt",    "101010", "r",
  "sltu",   "101011", "r",
  "sra",    "000011", "s",
  "srav",   "000111", "r",
  "srl",    "000010", "s",
  "srlv",   "000110", "r",
  "sub",    "100010", "r",
  "subu",   "100011", "r",
  "syscall","001100", "x",
  "xor",    "100110", "r",
}; 

const char* 
AObject::_ItypeArray[] = 
{ 
  // R-type instructions' Op code is different from each
  // opName, opCode, Type (i: normal; l:load/store x:extend)
  "addi",  "001000", "i",
  "addiu", "001001", "i",
  "andi",  "001100", "i",
  "beq",   "000100", "i",
  "bne",   "000101", "i",
  "lb",    "100000", "l",
  "lw",    "100011", "l",
  "lui",   "001111", "x",
  "ori",   "001101", "i",
  "sb",    "101000", "l",
  "slti",  "001010", "i",
  "sltiu", "001011", "i",
  "sw",    "101011", "l",
  "xori",  "001110", "i",
};

const char* 
AObject::_JtypeArray[] = {
  // J-type Op code
  // opName, opCode, Type 
  "j",   "000010", "j",
  "jal", "000011", "j",
};

const u32
  AObject::_REG_AZ = 32, // register file size
  AObject::_R_AZ   = 69, // 3* size of R-type instr
  AObject::_I_AZ   = 42, // 3* size of I-type instr
  AObject::_J_AZ   = 6; // 3* size of J-type instr

/* 
  5 fake instructions:
    "bgez", "bgtz", "blez", "bltz", "noop"
*/


void 
AObject::_createAllOpMap(std::map<std::string, char>& mapAllOp)NO_THROW
{
  // init R I J type instruction's Op code
  for ( u32 i=0; i<_R_AZ; i+=3 ) 
    mapAllOp[_RtypeArray[i]] = _RtypeArray[i+2][0];
  
  for ( u32 i=0; i<_I_AZ; i+=3 )
    mapAllOp[_ItypeArray[i]] = _ItypeArray[i+2][0];
  
  for ( u32 i=0; i<_J_AZ; i+=3 ) 
    mapAllOp[_JtypeArray[i]] = _JtypeArray[i+2][0];
}


void
AObject::closeFile()NO_THROW
{
  _sin.close();
  
  if(_bHaveOutFile)
    _sout.close();
}

void
AObject::clear()NO_THROW
{
  closeFile();
  _uCurrentLine = 0;
  //if((_logger.e_size() != 0 || _logger.u_size() != 0) && _bHaveOutFile)
    //remove(getOutFileName());
}

char
AObject::_getOp(
    std::map<std::string, char>& mapAllOp, 
    const std::string& strOp )NO_THROW
{ 
  // get op form std::map, here must use find(),
  // or will change the std::map's storage
  std::map<std::string, char>::const_iterator
      it = mapAllOp.find(strOp);
  if(it != mapAllOp.end())
    return it->second;
  else
    return 0;
}

bool
AObject::_loadFromFile( const char* szFileName)NO_THROW
{
  // load files and open out files 
  // Here will parse the file extension, and will 
  // add some new extension
  
  _strInFilePathName = szFileName;

  std::vector<std::string> vecInFileName;
  
  if( _strInFilePathName.find('/') != std::string::npos )
    split(vecInFileName, C(_strInFilePathName), '/');
  else if( _strInFilePathName.find('\\') != std::string::npos )
    split(vecInFileName, C(_strInFilePathName), '\\');

  if(vecInFileName.empty()) // not include path
    _strInFileName = szFileName;
  else
    _strInFileName = *(vecInFileName.end() - 1);


	_strOutFileName.clear(); // clear old file name
  for ( std::string::const_iterator it=_strInFileName.begin();
      it!=_strInFileName.end(); ++it ) {
    if ( *it != '.' )
      _strOutFileName.push_back(*it);
    else
      break;
  }

  //OPEN Logger 
  if(!_isLoggerOpen)
    _isLoggerOpen = _logger.open( (_strOutFileName + LOG_FILE_EXT).c_str() );

  // init done!!!
  if ( !_sin.open(szFileName) ){
    _logger.err(str("Error: unable to open file: %s", szFileName));
    clear();
    return false;
  }
  
  if(_bHaveOutFile) {
    _strOutFileName += getFileExt(); // set for new file name
    if(_strInFileName == _strOutFileName){
      _logger.err("Output file can't be same with Input file.");
      clear();
      return false;
    }

    if ( !_sout.open(C(_strOutFileName)) ){
      _logger.err(str("Error: unable to create file: %s",C(_strOutFileName)));
      clear();
      return false;
    }
  }
  return true;
}


bool
AObject::run(const char* szFileName, const char* sz)NO_THROW
{
  if( !_loadFromFile(szFileName) )
    return false;
  std::string strLine;
  while( _sin>>strLine ){
    _uCurrentLine++;
    _sout<<_uCurrentLine<<' '<<strLine<<'\n';
  }
  clear();
  return true;
}

void
AObject::_Log(
    _LoggerLevel v,
    const std::string& strMsg,
    const std::vector<std::string> *pv)NO_THROW
{ 
  const char* a = getInFileName();
  const u32   b = getCurrentLine();
  const char* c = strMsg.c_str();
  std::string extra = "\t";
  switch(v){
    case DBG:
      _logger.debug(str("%s::%u::%s", a, b,c));
      break;
    case WAR:
      _logger.warn(str("%s::%u::%s", a,b,c));
      break;
    case ERR:
      _logger.err(str("%s::%u::%s", a,b,c));
      break;
    default:
      _logger.raw("UNKNOWN ERROR");
      break;
  }
  if(pv){
    for(int i=0; i!=pv->size(); i++){
      extra += (*pv)[i] + " ";
    }
    _logger.raw(extra);
  }

}

