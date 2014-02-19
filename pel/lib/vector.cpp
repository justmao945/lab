
#include<cstdio>
#include"vector.h"


//---------------------------------------
//  Vector impl
//--------------------------------------

std::size_t pel::r2pow(std::size_t x)
{
  if( (x & (x-1)) != 0 ){
    if( sizeof(std::size_t) == 4 ){
      assert(x < 0x80000000U); // BUG?
    }else if( sizeof(std::size_t) == 8 ){
      assert(x < 0x8000000000000000U);
    }else{
      assert(!"std::size_t has not supported size.");
    }
    
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    if( sizeof(std::size_t) == 8 ){
      x |= x >> 4 * sizeof(std::size_t);
    }
    x++;
  }
  return x;
}



//---------------------------------------
//  Test vector
//--------------------------------------
#include"test.h"

namespace pel{

void TestVector()
{
  TestBegin("Vector");

  Vector<int> v;
  for(size_t i=0; i<20; ++i){
    v.Push(i);
    printf("%d ", v[i]);
  }
  putchar('\n');
  v[1] = 100;
  printf("v1 = %d ", v[1]);

  v.Pop();
  printf("v.size %d\n", v.Size());

  int iv[] = {1,2,3,4,5,6,7,8,9,10};
  v.Push<9>( iv );

  for(size_t i=0; i<v.Size(); ++i){
    printf("%d ", v[i]);
  }
  
  putchar('\n');

  TestEnd();
}

} //~pel
