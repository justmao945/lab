#ifndef _STORAGE_H_
#define _STORAGE_H_

#include<string>
#include<vector>
#include<cstdio>
#include"adef.h"
#include"utils.h"

class StorageOut{
public:

  StorageOut(size_t uCapacity=2*1024*1024) NO_THROW 
  {
    _fp = 0;
    _uCapacity = uCapacity;
    _str.reserve(uCapacity + 1024);
  }
  ~StorageOut()NO_THROW {}

  bool open(const char* szFileName)NO_THROW
  {
    _str.clear();
    return (_fp = fopen(szFileName, "wb")) != NULL;
  }

  bool close()NO_THROW
  {
    if(_fp){
      bool b = flush(0);
      _str.clear();
      return b && fclose(_fp) != EOF;
    }
    else
      return true;
  }
  
  void clear()
  {
    _str.clear();
    if(_fp)
      fseek(_fp, 0, SEEK_SET);
  }

  bool flush(size_t uCapacity=-1)NO_THROW
  {
    if(!_fp)
      return false;

    if(uCapacity == -1 && _uCapacity > _str.size())
      return true;
    else if(uCapacity > _str.size())
      return true;
    size_t a = _str.size();
    size_t b = fwrite(_str.c_str(),sizeof(char),_str.size(),_fp);
    if(a == b)
      _str.clear();
    else
      fseek(_fp, (-1)*((long)b), SEEK_CUR);
    return a == b;
  }

  StorageOut& operator <<(const std::string& str)NO_THROW
  {
    if(_fp){
      _str += str;
      flush();
    }
    return *this;
  }

  StorageOut& operator <<(const char* sz)NO_THROW
  {
    if(_fp){
      _str += sz;
      flush();
    }
    return *this;
  }

  StorageOut& operator <<(const char ch)NO_THROW
  {
    if(_fp){
      _str.push_back(ch);
      flush();
    }
    return *this;
  }
protected:
  std::string _str;
  size_t _uCapacity;
  FILE* _fp;
};


class StorageIn{
public:
  StorageIn(size_t uCapacity=2*1024*1024)NO_THROW
  {
    _fp = 0;
    _uCapacity = uCapacity;
    _clear();
    _buf = new(std::nothrow) char[uCapacity];
  }
  ~StorageIn()NO_THROW
  {
    delete [] _buf;
  }

  bool open(const char* szFileName)NO_THROW
  {
    _clear();
    _fp = fopen(szFileName, "rb");
    return _fp != NULL && _buf != 0;
  }

  int close()NO_THROW
  { 
    if(_fp){
      _clear();
      return fclose(_fp);
    }
    else
      return 0;
  }

  size_t read(char* sz, size_t uSize)NO_THROW
  {
    if(_fp){
      size_t i;
      for(i=0;i<uSize && _rbuf();i++)
        sz[i] = _buf[_cursor++];
      return i;
    }
    else
      return 0;
  }

  int seek(long offset, int origin)NO_THROW
  {
    if(_fp){
      _clear();
      return fseek(_fp, offset, origin);
    }
    return -1;
  }

  bool eof()NO_THROW
  {
    if(_fp)
      return !_rbuf();
    else
      return false;
  }

  void clear()NO_THROW
  { 
    _clear();
    if(_fp)
      fseek(_fp, 0, SEEK_SET);
  }

  operator void*()NO_THROW 
  { // used for operator >> bool test
    return (void*)(_rbuf() + _cut);
  }

  StorageIn& operator >>(std::string& str)NO_THROW
  {
    if(!_fp)
      return *this;

    _cut = 0;
    str.clear();
    while(_rbuf()){
      if(_buf[_cursor]=='\r' && _cursor+1<_uSize && _buf[_cursor+1] == '\n'){
        str += std::string(&_buf[_cursor-_cut],_cut);
        _cursor += 2;
        return *this;
      }
      else if(_chLast == '\r' && _buf[_cursor] == '\n')
        _cursor++;
      else if(_buf[_cursor] == '\r' || _buf[_cursor] == '\n'){
        str += std::string(&_buf[_cursor-_cut],_cut);
        _cursor++;
        return *this;
      }
      else if(_cursor + 1 == _uCapacity){ // !!!IMPORTANT!!! when _rbuf() really read
        _cursor++;
        _cut++;
        str += std::string(&_buf[_cursor-_cut], _cut);
        _chLast = _buf[_cursor];
      }
      else{
        _cut++;
        _cursor++;
      }
    }
    if(_cut)
      str += std::string(&_buf[_cursor-_cut],_cut);
    return *this;
  }

protected:
  char *_buf;
  FILE *_fp;
  size_t _uSize, _uCapacity, _cursor, _cut;
  char _chLast;

  void _clear()
  {
    _chLast = _uSize = _cut = 0;
    _cursor = _uCapacity;
  }

  bool _rbuf()
  {
    if(_cursor >= _uCapacity){
      _uSize = fread(_buf, sizeof(char), _uCapacity, _fp);  
      _cursor = _cut = _chLast = 0;
    }
    return _cursor < _uSize;
  }

};

#endif
