#include <string>
#include <cstring>
#include <cctype>
#include <cstdarg>
#include <cstdio>
#include "utils.h"


int32
str2int(const char* szNum)
{ 
  if( szNum[0] == '\0' )
    throw TypeConvertErr();
  
  size_t uStart = szNum[0] == '-' ? 1 : 0;
  if(uStart == 1 && szNum[1] == '\0')
    throw TypeConvertErr();
  
  int32 nRes = 0;

  if( szNum[uStart] >= '0' && szNum[uStart] <= '9' &&
      tolower(szNum[uStart+1]) == 'x') {// HEX
    bool isNum = false; 
    for (size_t i=uStart+2, j=-1; szNum[i]!='\0'; i++){
      if( szNum[i] >= '0' && szNum[i] <= '9' )
        isNum = true;
      else if( tolower(szNum[i]) >= 'a' && tolower(szNum[i]) <= 'f' )
        isNum = false;
      else
        throw TypeConvertErr();
      if( szNum[i] != '0' && j == -1)
        j = i;
      // max int32 is 0x 7FFF_FFFF
      if( (i-j >= 8 && j != -1) ||
          (i-j == 7 && ( szNum[j] > '7' || tolower(szNum[j])>='a') ) ) 
        throw TypeConvertOverflow();
      nRes = (nRes << 4) + (isNum ? szNum[i]-'0' : tolower(szNum[i])-'a'+10);
    }
  }
  else if(szNum[uStart] == '0') {// OCT
    for ( size_t i=uStart, j=-1; szNum[i]!='\0'; i++ ){
      if( !(szNum[i] >= '0' && szNum[i] <= '7') )
        throw TypeConvertErr();
      if(szNum[i] != '0' && j == -1)
        j = i;
      // max int32 is 01_77777_77777
      if( (i-j >=11 && j != -1) || (i - j == 10 && szNum[i] > '1') )
        throw TypeConvertOverflow();
      nRes = (nRes << 3) + (szNum[i] - '0');
    } 
  }
  else{ // DEC
    char buf[11] = "0000000000";
    size_t i, j;
    for ( i=uStart, j=-1; szNum[i]!='\0'; i++){
      if( !(szNum[i] >= '0' && szNum[i] <= '9') )
        throw TypeConvertErr();
      if(szNum[i] != '0' && j == -1)
        j = i;
      nRes = nRes * 10 + (szNum[i]-'0');
      if(i - j < 10)
        buf[i-j] = szNum[i];
      else if( j != -1 )
        throw TypeConvertOverflow();
    }
    // max int32 is 2147483647
    if(i-j == 9 && j!= -1 && strcmp(buf, "2147483647") > 0)
      throw TypeConvertOverflow();
  }

  return (uStart ? -nRes : nRes);
}


u32
str2u(const char* szNum)
{ 
  if( szNum[0] == '\0' )
    throw TypeConvertErr();
  
  u32 uRes = 0;
  if( szNum[0] >= '0' && szNum[0] <= '9' &&
      tolower(szNum[1]) == 'x') {// HEX
    bool isNum = false; 
    for (size_t i=2, j=-1; szNum[i]!='\0'; i++){
      if( szNum[i] >= '0' && szNum[i] <= '9' )
        isNum = true;
      else if( tolower(szNum[i]) >= 'a' && tolower(szNum[i]) <= 'f' )
        isNum = false;
      else
        throw TypeConvertErr();
      if( szNum[i] != '0' && j == -1)
        j = i;
      // max u32 is 0x FFFF_FFFF
      if( i-j >= 8 && j != -1) 
        throw TypeConvertOverflow();
      uRes = (uRes << 4) + (isNum ? szNum[i]-'0' : tolower(szNum[i])-'a'+10);
    }
  }
  else if(szNum[0] == '0') {// OCT
    for ( size_t i=0, j=-1; szNum[i]!='\0'; i++ ){
      if( !(szNum[i] >= '0' && szNum[i] <= '7') )
        throw TypeConvertErr();
      if(szNum[i] != '0' && j == -1)
        j = i;
      // max u32 is 07_77777_77777
      if( i-j >=11 && j != -1 )
        throw TypeConvertOverflow();
      uRes = (uRes << 3) + (szNum[i] - '0');
    } 
  }
  else{ // DEC
    char buf[11] = "0000000000";
    size_t i, j;
    for ( i=0, j=-1; szNum[i]!='\0'; i++){
      if( !(szNum[i] >= '0' && szNum[i] <= '9') )
        throw TypeConvertErr();
      if(szNum[i] != '0' && j == -1)
        j = i;
      uRes = uRes * 10 + (szNum[i]-'0');
      if(i - j < 10)
        buf[i-j] = szNum[i];
      else if( j != -1 )
        throw TypeConvertOverflow();
    }
    // max u32 is 4294967295
    if(i-j == 9 && j!= -1 && strcmp(buf, "4294967295") > 0)
      throw TypeConvertOverflow();
  }

  return uRes;
}




std::string
int2str( const int32 nNum)NO_THROW
{
  // max int32 is 2147483647
  char szNum[11];
  int32 div = 1000000000, tmp, num = nNum;
  bool  bHaveNonZero = false;
  size_t uStart = 0;
  if(num < 0){
    uStart = 1;
    szNum[0] = '-';
    num = -num;
  }
  while(div){
    tmp = num / div;
    if(tmp != 0 || bHaveNonZero){
      szNum[uStart++] = tmp + '0';
      num %= div;
    }
    if(tmp != 0)
      bHaveNonZero = true;
    div /= 10;
  }
  szNum[uStart] = '\0';
  if(szNum[0] == '\0'){
    szNum[0] = '0';
    szNum[1] = '\0';
  }
  return std::string(szNum);
}

std::string
u2str( const u32 uNum)NO_THROW
{
  // max u32 is 4294967295
  char szNum[11];
  u32 div = 1000000000, tmp, num = uNum;
  bool  bHaveNonZero = false;
  size_t uStart = 0;
  while(div){
    tmp = num / div;
    if(tmp != 0 || bHaveNonZero){
      szNum[uStart++] = tmp + '0';
      num %= div;
    }
    if(tmp != 0)
      bHaveNonZero = true;
    div /= 10;
  }
  szNum[uStart] = '\0';
  if(szNum[0] == '\0') {
    szNum[0] = '0';
    szNum[1] = '\0';
  }
  return std::string(szNum);
}

std::string 
int2bin(u32 uBitWidth, const int32 nNumber)NO_THROW
{
  char szT[33];
  u32 i;
  
  if(uBitWidth > 32)
    uBitWidth = 32;
  
  for (i=0; i<uBitWidth; ++i)
    szT[i] = (nNumber>>(uBitWidth - i - 1) & 1) + '0';
  szT[i] = '\0';
  return std::string(szT);
}

std::string
u2bin(u32 uBitWidth, const u32 uNumber)NO_THROW
{
  char szT[33];
  u32 i = 0;
  if(uBitWidth > 32)
    uBitWidth = 32;

  for ( u32 i=0; i<uBitWidth; ++i)
    szT[i] = (uNumber>>(uBitWidth - i - 1) & 1) + '0';
  szT[i] = '\0';
  return std::string(szT);
}


int32
bin2int(const char* szBin)
{
  int32 nNum = 0;
  size_t i = 0;
  if(szBin[0] == '\0')
    throw TypeConvertErr();
  else if(szBin[0] == '-'){
    i = 1;
    if(szBin[1] == '\0')
      throw TypeConvertErr();
  }
  
  for (size_t j=-1; szBin[i]!='\0'; i++){
    if( szBin[i] == '1' && j == -1 )
      j = i;
    if(szBin[i] != '0' && szBin[i] != '1')
      throw TypeConvertErr();
    else if( i-j >= 31 && j != -1)
      throw TypeConvertOverflow();
    nNum = (nNum << 1) + (szBin[i] - '0');
  }
  return szBin[0] == '-' ? -nNum : nNum;
}

u32
bin2u(const char* szBin)
{
  u32 uNum = 0;

  if(szBin[0] == '\0')
    throw TypeConvertErr();
  
  for (size_t i=0, j=-1; szBin[i]!='\0'; i++){
    if( szBin[i] == '1' && j == -1 )
      j = i;
    if(szBin[i] != '0' && szBin[i] != '1')
      throw TypeConvertErr();
    else if( i-j >= 32 && j != -1)
      throw TypeConvertOverflow();
    uNum = (uNum << 1) + (szBin[i] - '0');
  }
  return uNum;
}



std::string
str(const char* szFormat, ...)
{
	/* Eh, here ... parameter in C++ is strange, that is a C style parameter
	   and must use none C++ class in it. Or in MSVC will occur some error!! 
		 Eh, GCC works well.
	*/
  
	va_list arg_ptr;
  std::string strTemp;
  strTemp.reserve(256);
  bool bPrePercentSign = false;
  u32 uBitWidth = 0;
  size_t s = 0;
  const char* it;
	va_start(arg_ptr, szFormat);
  for ( it=szFormat; *it!='\0'; ++it ) {
    if( bPrePercentSign ) {
      if( *it == 's' ) {
        const char* szTemp = va_arg(arg_ptr, const char*);
				strTemp += szTemp;
        bPrePercentSign = false;
      }
      else if( *it == 'c' ){
        strTemp.push_back(va_arg(arg_ptr, int32));
        bPrePercentSign = false;
      }
      else if( *it == 'd' ){
        strTemp += int2str(va_arg(arg_ptr, int32));
        bPrePercentSign = false;
      }
      else if( *it == 'u' ){
        strTemp += u2str(va_arg(arg_ptr, u32));
        bPrePercentSign = false;
      }
      else if( *it == 'b' ){
        try{
          strTemp += int2bin(uBitWidth, va_arg(arg_ptr, int32));
        }catch(TypeConvertErr){
          throw StrFormatErr();
        }
        uBitWidth = 0;
        bPrePercentSign = false;
      }
      else if( *it >= '0' && *it <= '9' )
        uBitWidth = uBitWidth * 10 + *it - '0';
      else 
        throw StrFormatErr();
    }
    else if( *it == '%' ){
      bPrePercentSign = true;
      strTemp += std::string(it-s, s);
      s = 0;
    }
    else
      s++;
  }
  va_end(arg_ptr);
  if(s)
    strTemp += std::string(it-s, s);
  return strTemp;
}

std::vector<std::string>&
split(std::vector<std::string>& vRes,
      const char* szOri,
      const char chFlag)NO_THROW
{
  vRes.reserve(10);
  vRes.clear();
  size_t s = 0;
  const char* it;
  for ( it=szOri; *it!='\0'; ++it ) {
    if(*it != chFlag)
      s++;
    else if(s){
      vRes.push_back(std::string(it-s, s));
      s = 0;
    }
  }
  if( s )
    vRes.push_back(std::string(it-s,s));
  return vRes;
}

std::string
lstrip(const char* szOri, const char chFlag)NO_THROW
{
  size_t i;
  
  for (i=0; szOri[i]!='\0'; ++i ) 
    if(szOri[i] != chFlag)
      break;
  return std::string(szOri + i, strlen(szOri)-i);
}

std::string
rstrip(const char* szOri, const char chFlag)NO_THROW
{
  size_t i = strlen(szOri);
  
  while (1) {
    i--;
    if(szOri[i] != chFlag)
      break;
    else if(i == 0) {
      i--;
      break;
    }
  }
  i++;
  return std::string(szOri, i);
}


std::string
strip(const char* szOri, const char chFlag)NO_THROW
{
  return rstrip( C(lstrip(szOri, chFlag)), chFlag );
}
