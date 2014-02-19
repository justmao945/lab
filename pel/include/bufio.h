/*
 * File: bufio.h
 * Provides basic buffered input and output.
 * TODO: utf8 support
 */

#ifndef PEL_BUFIO_H
#define PEL_BUFIO_H

#include<cstddef>
#include<cstdio>

namespace pel{

class Reader{
public:
  ~Reader();

  // Read from standard IO system, and don't perform cursor related operations
  // on stream `in` outside of reader.
  // @in FILE stream to read.
  explicit Reader(FILE *in);

  // Read from string, please keep the input string alive while reading.
  // @in Char array to read
  // @len Length of char array, if equals to 0 then treat in as a C string.
  explicit Reader(const char *in, std::size_t len=0);

  // @return buffer size.
  std::size_t BufSize();

  // Set a new bigger size for buffer, default is 4096 bytes.
  // This operation causes memory reallocated if new size is bigger 
  // than current buffer size.
  // @size
  // @return self
  Reader& BufSize(std::size_t size);

  // Read a char from reader.
  // @return EOF if end of input or a char
  int ReadChar();

  // Put `ch` back to buffer after a call of ReadChar.
  // @see ungetc in C.
  // @return true if successfully or false.
  bool UnReadChar(int ch);

private:
  char         *buf;
  std::size_t   bufsize; // max capacity
  std::size_t   bufcur;  // cursor of buffer
  std::size_t   buflen;  // valid size of buffer

  FILE         *fin;    // file stream
  long int      fpos;   // record file postion
  const char   *strin;  // string stream
  std::size_t   strlen; // length of string stream

private:
  Reader(); // empty reader is meaningless
  Reader(const Reader&);  // copy reader is disabled
  Reader operator=(const Reader&);
  void cinit();  // common initialization
  void refill(); // refill buffer
};

}

#endif // ~PEL_BUFIO_H