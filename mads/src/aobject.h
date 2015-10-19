#ifndef _AOBJECT_H_
#define _AOBJECT_H_

// File aobject.h base class AObject
// Provide basic File I/O and string operations
#include <map>
#include <string>
#include "except.h"
#include "adef.h"
#include "utils.h"
#include "logger.h"
#include "storage.h"

class AObject{
public:
  AObject() NO_THROW
  {
    _chCommentFlag = DEFAULT_COMMENT_FLAG; // flag out the comment flag
    _uCurrentLine  = 0;                   // current line number
    _strFileExt    = AOBJECT_FILE_EXT;    // file's extension
    _bHaveOutFile  = true;                // true if have out file, or false
  }

  virtual               ~AObject() NO_THROW{}
  virtual void          clear()NO_THROW;
  virtual bool          run(const char* szFileName,
                            const char* sz=0)NO_THROW; // load file and run
  
  inline const char     getCommentFlag()NO_THROW;		  // get comment flag
  inline const char*    getFileExt()NO_THROW;				  // get file name extension
  inline const char*    getInFilePathName()NO_THROW;  // get read in file name, include path
  inline const char*    getInFileName()NO_THROW;			// get read in file name, no path
  inline const char*    getOutFileName()NO_THROW;		  // get out file name and pat
  inline u32            getCurrentLine()NO_THROW;		  // get current dealing line number
  void                  closeFile()NO_THROW;          // close the file

  static inline Logger& getLogger()NO_THROW;
  static inline bool    closeLogger()NO_THROW;
  static inline bool    isLoggerHErr()NO_THROW;

protected:
  static const u32 _REG_AZ, _R_AZ, _I_AZ, _J_AZ;
  // Registers Array size, and R,I,J type instructions' Array size
  enum _LoggerLevel{DBG, WAR, ERR};

  static const char
    *_RegArray[],
    *_RtypeArray[],
    *_ItypeArray[],
    *_JtypeArray[];
    // Registers Array, and R,I,J type instructions' Array
  
  static Logger _logger;
  static bool   _isLoggerOpen;

  std::string   _strFileExt;
  std::string   _strInFilePathName;
  std::string   _strInFileName;
  std::string   _strOutFileName;
  bool          _bHaveOutFile;  // whether have a out file or not
  char          _chCommentFlag;
  u32           _uCurrentLine;  // start from 1
  StorageIn     _sin;
  StorageOut    _sout;


protected:
  void _createAllOpMap(std::map<std::string, char>& mapAllOp)NO_THROW;
  char _getOp(std::map<std::string, char>& mapAllOp,
             const std::string& strOp)NO_THROW;
  // get op type from op code

  bool         _loadFromFile(const char* szFileName)NO_THROW; //load file
  inline bool  _isSpace(char ch)NO_THROW; // space or tab
  void         _Log( _LoggerLevel v, const std::string& strMsg,
      const std::vector<std::string> *pv=NULL)NO_THROW;  // record msg into _logger
  inline int32 _str2int(const char* szNum)NO_THROW;
  inline u32   _str2u(const char* szNum)NO_THROW;
  inline int32 _bin2int(const char* szBin)NO_THROW;
  inline u32   _bin2u(const char* szBin)NO_THROW;

};

////////////////////////////////////////////////
//  Inline Functions
/////////////////////////////////////////////////
bool 
AObject::_isSpace(char ch)NO_THROW
{ return (ch == ' ' || ch == '\t'); }

const char 
AObject::getCommentFlag()NO_THROW
{ return _chCommentFlag; }

u32
AObject::getCurrentLine()NO_THROW
{ return _uCurrentLine; }


const char* 
AObject::getFileExt()NO_THROW
{
  return _strFileExt.c_str();
}

const char*
AObject::getInFileName()NO_THROW
{ 
  return _strInFileName.c_str(); 
}

const char*
AObject::getInFilePathName()NO_THROW
{ 
  return _strInFilePathName.c_str(); 
}

const char* 
AObject::getOutFileName()NO_THROW
{
  if(_bHaveOutFile)
    return _strOutFileName.c_str(); 
  else {
    _logger.warn("Out file is not enabled");
    return 0;
  }
}

Logger&
AObject::getLogger()NO_THROW
{
  return _logger;
}

bool 
AObject::isLoggerHErr()NO_THROW
{
  return _logger.e_size() || _logger.u_size();
}

int32 
AObject::_str2int(const char* szNum)NO_THROW
{
  int32 res;
  try{
    res = str2int(szNum);
  }catch(TypeConvertErr&){
    _Log(ERR, str("TypeConvertErr.-->%s", szNum));
    res = 0;
  }catch(TypeConvertOverflow&){
    _Log(WAR, str("TypeConvertOverflow.-->%s", szNum));
    res = 0;
  }
  return res;
}

u32   
AObject::_str2u(const char* szNum)NO_THROW
{
  u32 res;
  try{
    res = str2u(szNum);
  }catch(TypeConvertErr&){
    _Log(ERR, str("TypeConvertErr.-->%s", szNum));
    res = 0;
  }catch(TypeConvertOverflow&){
    _Log(WAR, str("TypeConvertOverflow.-->%s", szNum));
    res = 0;
  }
  return res;
}

int32 
AObject::_bin2int(const char* szBin)NO_THROW
{
  int32 res;
  try{
    res = bin2int(szBin);
  }catch(TypeConvertErr&){
    _Log(ERR, str("TypeConvertErr.-->%s", szBin));
    res = 0;
  }catch(TypeConvertOverflow&){
    _Log(WAR, str("TypeConvertOverflow.-->%s", szBin));
    res = 0;
  }
  return res;
}

u32
AObject::_bin2u(const char* szBin)NO_THROW
{
  u32 res;
  try{
    res = bin2u(szBin);
  }catch(TypeConvertErr&){
    _Log(ERR, str("TypeConvertErr.-->%s", szBin));
    res = 0;
  }catch(TypeConvertOverflow&){
    _Log(WAR, str("TypeConvertOverflow.-->%s", szBin));
    res = 0;
  }
  return res;
}

#endif
