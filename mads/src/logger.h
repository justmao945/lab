#ifndef _LOGGER_H_
#define _LOGGER_H_

#include<cstdio>
#include<iostream>
#include<string>
#include"storage.h"

class Logger{
public:
  Logger()NO_THROW
  {
    _uDebug = _uWarn = _uError = _uUnknown = 0;
  }
  ~Logger()NO_THROW{}

  bool open(const char* szLogFile)NO_THROW
  {
    _strLogFileName = szLogFile;
    return _Logger.open(szLogFile);
  }

  bool close()NO_THROW
  {
    _uDebug = _uWarn = _uError = _uUnknown = 0;
    return _Logger.close();
  }

  void clear()NO_THROW
  {
    _Logger.clear();
    _uDebug = _uWarn = _uError = _uUnknown = 0;
  }

  void debug(const std::string& strMsg)NO_THROW
  {
    _Logger<<"DEBUG: "<<strMsg<<'\n';
    _uDebug++;
  }
  
  void warn(const std::string& strMsg)NO_THROW
  {
    _Logger<<"WARNNING: "<<strMsg<<'\n';
    _uWarn++;
  }
  
  void err(const std::string& strMsg)NO_THROW
  {
    _Logger<<"ERROR: "<<strMsg<<'\n';
    _uError++;
  }

  void raw(const std::string& strMsg)NO_THROW
  {
    _Logger<<strMsg<<'\n';
    _uUnknown++;
  }
  
  size_t d_size()NO_THROW{return _uDebug;}
  size_t w_size()NO_THROW{return _uWarn;}
  size_t e_size()NO_THROW{return _uError;}
  size_t u_size()NO_THROW{return _uUnknown;}
  
  friend std::ostream& operator <<(std::ostream& os, Logger& logger)NO_THROW
  {
    const size_t BUF_SIZE=4096;
    char buf[BUF_SIZE];
    logger._Logger.close();
    FILE * fp = fopen(logger._strLogFileName.c_str(), "rb");
    size_t s = fread(buf, sizeof(char), BUF_SIZE-1, fp);
    fclose(fp);
    buf[s] = '\0';
    os<<"DEBUG("<<logger._uDebug<<") "
      <<"WARNNING("<<logger._uWarn<<") "
      <<"ERROR("<<logger._uError<<") "
      <<"UNKNOWN("<<logger._uUnknown<<")\n"
      <<buf<<"...";
      
    if(logger._uError == 0 && logger._uUnknown == 0){
      os<<"Successfully!"<<std::endl;
      remove(logger._strLogFileName.c_str());
    }
    else
      os<<"Fatal error occured! More info see "<<logger._strLogFileName<<std::endl;
    logger.clear();
    return os;
  }

protected:
  StorageOut _Logger;
  std::string _strLogFileName;
  size_t _uDebug, _uWarn, _uError, _uUnknown;
};

#endif
