//
//  Main test file.
//  Every local defined TestXXX function will be called from here.
//

#include<cstdio>
#include"test.h"

namespace pel{

void TestAllocator(); // new.cpp
void TestMap(); // map.cpp
void TestParser(); // parser.cpp
void TestRTTI(); // rtti.cpp
void TestScanner();  // scanner.cpp
void TestVector(); // vector.cpp

}

void pel::TestBegin(const char* label)
{
  std::puts(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
  std::printf(">> Testing %s ...\n", label);
  std::puts(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
}


void pel::TestEnd()
{

}



int main(int argc, char* argv[])
{
  pel::TestAllocator();
  pel::TestMap();
  pel::TestParser();
  pel::TestRTTI();
  pel::TestScanner();
  pel::TestVector();
  
  return 0;
}
