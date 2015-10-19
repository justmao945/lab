#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>
#include <vector>
#include "adef.h"
#include "except.h"

/*
Because int32/u32 convert to string and the result string 
is no more then 10 bits. So return std::string is acceptable.
But like split(), put the result into the reference parameter
will be better, without coping the vector object.
(avoid using the C++0x rvalue new feature).
strip() will use return value optimization;
*/

int32 
str2int(const char* szNum);
// convert c_string to int32, support oct, dec, hex
// here may be overflow when the result is too large
// and will throw an exception

u32 
str2u(const char* szNum);
// convert c_string to u32, support oct, dec, hex
// here may be overflow when the result is too large
// and will throw an exception

std::string
int2str( const int32 nNum)NO_THROW;
// in DEC format

std::string
u2str( const u32 uNum)NO_THROW;
// in DEC format

std::string
int2bin(u32 uBitWidth, const int32 nNumber)NO_THROW;
// convert INT to BIN(represent in std::string)
// uBitWidth is the length of BIN
  
int32 
bin2int(const char* szBin);
// Convert BIN to INT, Sign extended
// here may be overflow when the result is too large
// will throw an exception

std::string
u2bin(u32 uBitWidth, const u32 uNumber)NO_THROW;
// convert UNSIGNED to BIN(represent in std::string)
// uBitWidth is the length of BIN
  
u32
bin2u(const char* szBin);
// convert BIN to UNSIGNED
// here may be overflow when the result is too large
// will throw an exception

std::string 
str(const char* strFormat, ...);
// format std::string. which likes printf£¬support %xb, %s, %d, %u, %c
// x: length of binary


std::vector<std::string>&
split(std::vector<std::string>& vRes,
      const char* strOri,
      const char chFlag=' ')NO_THROW;
// split c_string into std::vector<std::string>
// default splited by space ' '
// If have duplicate separators in the original string
// then all duplicates will be removed

std::string
strip(const char* szOri, const char chFlag=' ')NO_THROW;
// strip some char at the beginning and ending in original std::string
// default flag is space ' '

std::string
lstrip(const char* szOri,  const char chFlag=' ')NO_THROW;
// left strip c_string, default flag is space ' '

std::string
rstrip(const char* szOri, const char chFlag=' ')NO_THROW;
// right strip c_string, default flag is space ' '


#endif
