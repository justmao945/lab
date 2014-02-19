

#include<cstddef>
#include<cstring>
#include"error.h"
#include"new.h"

using namespace pel;

void Error::cinit(int err, const char* msg, bool move)
{
  if( msg ){
    if( move ){
      this->msg = const_cast<char*>(msg);
    }else{
      std::size_t len = std::strlen(msg);
      this->msg = new char[len + 1];
      std::memcpy(this->msg, msg, len * sizeof(char));
    }
  }else{
    this->msg = 0;
  }
  this->err = err;

  used = new int;
  *used = 1;
}

void Error::destroy()
{
  if( *used == 1 ){
    delete[] msg;
    delete used;
  }else if( *used > 1 ){
    --(*used);
  }
}

Error::Error()
{
  cinit(OK, 0, false);
}

Error::Error(const char* msg, bool move)
{
  cinit(ERROR, msg, move);
}

Error::Error(int err){
  cinit(err, 0, false);
}

Error::Error(int err, const char* msg, bool move)
{
  cinit(err, msg, move);
}

Error::Error(const Error& e)
{
  err = e.err;
  msg = e.msg;
  used = e.used;
  ++(*used);
}

Error::~Error()
{
  destroy();
}

Error& Error::operator=(const Error& e)
{
  if( this != &e ){
    destroy();
    
    err = e.err;
    msg = e.msg;
    used = e.used;
    ++(*used);
  }
  return *this;
}

