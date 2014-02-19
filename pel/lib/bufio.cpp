
#include<cassert>
#include<cstring>
#include"bufio.h"
#include"new.h"

#define PEL_READER_BUFSIZE 4096   // default buffer size

using namespace pel;

template<class T>
static T min(T a, T b)
{
  return ( a < b ? a : b );
}

void Reader::cinit()
{
  this->buf = new char[PEL_READER_BUFSIZE];
  assert(this->buf);
  this->bufsize = PEL_READER_BUFSIZE;
  this->bufcur = this->buflen = 0;
  this->fpos = 0;
}

Reader::Reader(FILE *in)
{
  assert(in);
  this->fin = in;
  this->strin = 0;
  this->strlen = 0;
  this->cinit();
}

Reader::Reader(const char *in, std::size_t len)
{
  assert(in);
  this->fin = 0;
  this->strin = in;
  if(len == 0){
    this->strlen = std::strlen(in);
  }else{
    this->strlen = len;
  }
  this->cinit();
}

Reader::~Reader()
{
  delete[] this->buf;
}

std::size_t Reader::BufSize()
{
  return this->bufsize;
}

Reader& Reader::BufSize(std::size_t size)
{
  if(size > this->bufsize){
    char *newBuf = new char[size];
    assert(newBuf);
    std::memcpy(newBuf, this->buf, this->buflen);
    delete[] this->buf;
    this->buf = newBuf;
    this->bufsize = size;
  }
  return *this;
}

int Reader::ReadChar()
{
  assert(this->buf);
  if( this->bufcur >= this->buflen ){
    this->refill();
    if( this->buflen == 0 ){
      return EOF;
    }
  }
  return this->buf[this->bufcur++];
}

bool Reader::UnReadChar(int ch)
{
  if(this->bufcur == 0)
    return false;

  this->buf[--this->bufcur] = ch;
  return true;
}

void Reader::refill()
{
  if(this->fin){ // file stream
    if( std::feof(this->fin) ){
      this->buflen = this->bufcur = 0;
      return;
    }
    std::fseek(this->fin, this->fpos, SEEK_SET);
    this->buflen = std::fread(this->buf, sizeof(char), this->bufsize, this->fin);
    this->fpos = std::ftell(this->fin);

  }else if(this->strin){ // string stream
    if( this->strlen == 0 ){
      this->buflen = this->bufcur = 0;
      return;
    }
    this->buflen = min(this->bufsize, this->strlen);
    std::memcpy(this->buf, this->strin, this->buflen);
    this->strin += this->buflen;
    this->strlen -= this->buflen;

  }else{
    this->buflen = 0;
    assert(!"invalid input stream");
  }
  this->bufcur = 0;
}
