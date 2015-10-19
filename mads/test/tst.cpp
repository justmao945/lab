#include <string>
#include <vector>
#include <iostream>
#include "../mads2/utils.h"
#include <cstdio>
#include "../mads2/storage.h"
#include<cstring>

using namespace std;
int main(int argc, const char *argv[])
{
  StorageIn sin;
  char buf[15];
  cout<<"open "<<sin.open("tst.sxx")<<endl;
  cout<<"eof "<<sin.eof()<<endl;
  string s;
  sin.read(buf, 14);
  buf[14] = 0;
  cout<<buf<<endl;
  sin.close();
  /*
  try{
    cout<<strcmp("7", "12")<<endl;
    string strT;
    cout<<"atoi:"<<str2int("0")<<endl;
    cout<<"atoi:"<<str2int("7")<<endl;
    cout<<"atoi:"<<str2u("7")<<endl;
    cout<<"atoi:"<<str2int("-123")<<endl;
    cout<<"atoi:"<<str2int("0123")<<endl;
    cout<<"atoi:"<<str2int("-0123")<<endl;
    cout<<"atoi:"<<str2int("0xff")<<endl;
    cout<<"atoi:"<<str2int("-0xfe")<<endl;
    //cout<<"atoi:"<<str2int("-0xxxfe")<<endl;
    //cout<<"atoi:"<<str2int("0xffffffff")<<endl;
    
    cout<<"itob:"<<int2bin(32, 1)<<endl;
    cout<<"btoi:"<<bin2int("11")<<endl;
    cout<<"btoi:"<<bin2int("001100")<<endl;
    cout<<"btoi:"<<bin2int("1010")<<endl;
    cout<<"btou:"<<bin2u("0111111111111111111111111111110")<<endl;
    cout<<"stou:"<<str2u("1023")<<endl;
    cout<<"str:"<<str("hello %5b %s %s %d %u world",0,"xxx","",1231,-1)<<endl;
    
    cout<<"lstrip:"<<lstrip("  xx  ")<<endl;
    cout<<"rstrip:"<<rstrip("  xx  ")<<endl;
    cout<<"strip:"<<strip("  xx  ")<<endl;
    
    vector<string> vs;
    split(vs, "  fff bbb   dd  ");
    cout<<"split:"<<vs.size()<<endl;
    for ( vector<string>::iterator it=vs.begin(); it!=vs.end(); ++it ) 
      cout<<*it<<endl;

  }catch(TypeConvertErr){
    cout<<"TypeConvertErr"<<endl;
  }catch(TypeConvertOverflow){
    cout<<"TypeConvertOverflow"<<endl;
  }catch(StrFormatErr){
    cout<<"StrFormatErr"<<endl;
  }
  */
  return 0;
}
