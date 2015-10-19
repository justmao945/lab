#include <cctype>
#include "prepare.h"

#define LOG(n) {\
  if(v.size() != n) { \
    _Log(ERR, "Instrction needs "#n" fields.-->" + v[0], &v); \
    break; \
  } \
}

#define _SOUT (_sout<<u2str(getCurrentLine())<<' ')

void
Prepare::init()NO_THROW
{
  _createAllOpMap(_mapAllOp);
}

void
Prepare::_convPseudoInstr(const std::string& strInstr)NO_THROW
{
  std::vector<std::string> v;
  split(v, C(strInstr));
  //_Log(DBG, str("%u", v.size()));
  char op = _getOp(_mapAllOp, v[0]);
  switch(op){
    case 'r':
    { // normal Rtype -->## add $1 $2 $3 0
      LOG(4);
      _SOUT<<lstrip(C(strInstr)) + " 0\n";
      break;
    }
    case 's':
    {  // shift Rtype -->## sll $d $zero $t h
      LOG(4);
      _SOUT<<v[0]<<' '<<v[1]<<" $zero "<<v[2]<<' '<<v[3]<<'\n';
      break;
    }
    case 'm':
    {  // divide and mulit -->## mut $zero $s $t 0
      LOG(3);
      _SOUT<<v[0]<<" $zero "<<v[1]<<' '<<v[2]<<" 0\n";
      break;
    }
    case 'i':
    { // normal I type  -->## addi $t, $s, imm
      LOG(4);
      _SOUT<<lstrip(C(strInstr))<<'\n';
      break;
    }
    case 'l': 
    { // load and store memrory
      LOG(3);
      std::string strReg, strOffset, strTemp = v[2];
      bool bLeftBracket = false;
      bool bRightBracket = false;
      bool bInBracket = false;
      bool bNoErr = true;
      for ( std::string::iterator it=strTemp.begin();
          it!=strTemp.end(); ++it) {
        if(*it != '(' && !bInBracket)
          strOffset.push_back(*it);
        else if(bInBracket && *it != ')')
          strReg.push_back(*it);
        else if(*it == '('){
          bLeftBracket = true;
          bInBracket = true;
        }
        else if(*it == ')'){
          bRightBracket = true;
          bInBracket =false;
        }
        else{
          _Log(ERR,"Instruction format error.-->" +  v[0]);
          bNoErr = false;
          break;
        }
      }
      if( !bLeftBracket ){
        _Log(ERR, "missing '('");
        bNoErr = false;
      }
      if( !bRightBracket ) {
        _Log(ERR, "missing ')'");
        bNoErr = false;
      }
      if(strOffset.empty()){
        _Log(ERR, "missing offset");
        bNoErr = false;
      }
      if(strReg.empty()){
        _Log(ERR, "missing offset regiter");
        bNoErr = false;
      }
      if(bNoErr) // ## lw $t, $s, offset
        _SOUT<<v[0]<<' '<<v[1]<<' '<<strReg<<' '<<strOffset<<'\n';
      break;
    }
    case 'j':
    {
      LOG(2); // ## j tar
      _SOUT<< lstrip(C(strInstr))<<'\n';
      break;
    }
    case 'x':
    {
      if(v[0] == "jr"){
        LOG(2); // ## jr $zero $s $zero 0
        _SOUT<<"jr $zero "<<v[1]<<" $zero 0\n";
      }
      else if(v[0] == "lui"){
        LOG(3); // ## lui $t $zero imm
        _SOUT<<"lui "<<v[1]<<" $zero "<<v[2]<<'\n';
      }
      else if(v[0] == "mfhi" || v[0] == "mflo"){
        LOG(2); // ## mfhi $d $zero $zero 0
        _SOUT<<v[0]<<' '<<v[1]<<" $zero $zero 0\n";
      }
      else if(v[0] == "syscall"){
        LOG(1);
        _SOUT<<v[0]<<" $zero $zero $zero 0\n";
      }
      break;
    }
    default:
    {
      if(v[0] == "noop"){
        LOG(1);
        _SOUT<<v[0]<<" $zero $zero $zero 0\n";
      }
      else if(v[0] == "bgez"){
        LOG(3);
        _SOUT<<"slt "<<v[1]<<' '<<v[1]<<" $zero 0\n";
        _SOUT<<"beq "<<v[1]<<" $zero "<<v[2]<<'\n';
      }
      else if(v[0] == "bgtz"){
        LOG(3);
        _SOUT<<"slt "<<v[1]<<" $zero "<<v[1]<<" 0\n";
        _SOUT<<"bne "<<v[1]<<" $zero "<<v[2]<<'\n';
      }
      else if(v[0] == "blez"){
        LOG(3)
        _SOUT<<"slt "<<v[1]<<" $zero "<<v[1]<<" 0\n";
        _SOUT<<"beq "<<v[1]<<" $zero "<<v[2]<<'\n';
      }
      else if(v[0] == "bltz"){
        LOG(3);
        _SOUT<<"slt "<<v[1]<<' '<<v[1]<<" $zero 0\n";
        _SOUT<<"bne "<<v[1]<<" $zero "<<v[2]<<'\n';
      }
      else
        _Log(ERR, "Instruction don't support.-->" + v[0]);
    }
    break;
  }
}

bool
Prepare::run(const char* szFileName, const char* sz)NO_THROW
{
  if(isLoggerHErr())
    return false;
  if( !_loadFromFile(szFileName) )
    return false;
  std::string strLine, strNewLine;

  while( _sin >> strLine ){
    strNewLine.reserve(strLine.capacity());
    _uCurrentLine++;
    bool bHaveSpace = true;
    bool bDupicateComma = false;

    std::string::iterator itEnd = strLine.end(); // No need refresh end()
    for (std::string::iterator it=strLine.begin(); it!=itEnd ; ++it){
      if(*it == '\n'||*it == '\r')
        continue;
      if( *it == getCommentFlag() ) // get out comment line
        break;
      else if(*it == ',' && bDupicateComma)
        _Log(ERR, "Too many commas here, or missing oprand?");
      else if(*it == ','){ // get out comma
        bDupicateComma = true;
        continue;
      }
      else if(*it == ':'){
        _sout<<str("%u%s:",getCurrentLine(), C(strNewLine))<<'\n';
        strNewLine.clear();
        continue;
      }
      else if(_isSpace(*it)) {
        bHaveSpace = true;
        continue;  // get out all spaces
      }
      else if(bHaveSpace){
        // add a space before a word
        strNewLine.push_back(' '); 
        bHaveSpace = false;
      }
      bDupicateComma = false;
      strNewLine.push_back(tolower(*it));
    }
    if(strNewLine.empty()) // get out blank line
      continue;
    
    _convPseudoInstr(strNewLine);
    strNewLine.clear();
  }
  clear();
  if(isLoggerHErr())
    return false;
  return true;
}
