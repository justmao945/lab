#include <string>
#include <vector>
#include <iostream>
#include "utils.h"

using namespace std;
int main(int argc, const char *argv[])
{
  try{
    string strT;
    cout<<"atoi:"<<str2int("0")<<endl;
    cout<<"atoi:"<<str2int("123")<<endl;
    cout<<"atoi:"<<str2int("-123")<<endl;
    cout<<"atoi:"<<str2int("0123")<<endl;
    cout<<"atoi:"<<str2int("-0123")<<endl;
    cout<<"atoi:"<<str2int("0xff")<<endl;
    cout<<"atoi:"<<str2int("-0xfe")<<endl;
    //cout<<"atoi:"<<str2int("-0xxxfe")<<endl;
    //cout<<"atoi:"<<str2int("0xffffffff")<<endl;
    
    cout<<"itob:"<<int2bin(32, 1)<<endl;
    cout<<"btoi:"<<bin2int("01100")<<endl;
    cout<<"btou:"<<bin2u("01100")<<endl;
    cout<<"stou:"<<str2u("1023")<<endl;
    cout<<"str:"<<str("hello %5b %s %s %d %u world",0,"xxx","",1231,-1)<<endl;
    
    cout<<"lstrip:"<<lstrip("  xx  ")<<endl;
    cout<<"rstrip:"<<rstrip("  xx  ")<<endl;
    cout<<"strip:"<<strip("  xx  ")<<endl;
    
    vector<string> vs;
    split(vs, "10 addi $a0 $a1 -20");
    cout<<"split:"<<endl;
    for ( vector<string>::iterator it=vs.begin(); it!=vs.end(); ++it ) 
      cout<<*it<<endl;

  }catch(TypeConvertErr){
    cout<<"TypeConvertErr"<<endl;
  }catch(TypeConvertOverflow){
    cout<<"TypeConvertOverflow"<<endl;
  }catch(StrFormatErr){
    cout<<"StrFormatErr"<<endl;
  }
  return 0;
}
