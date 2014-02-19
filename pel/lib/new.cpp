
#include<cassert>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include"new.h"

//#define PEL_TRACE_ALLOCATOR

void* operator new(std::size_t size)
{
  if(size == 0){
    size = 1;
  }
  void *p = std::malloc(size); //  only malloc once, if failed just down...

// in debug mode, all data initialized with 0.
#ifndef NDEBUG
  std::memset(p, 0, size);
#endif

#ifdef PEL_TRACE_ALLOCATOR
  std::printf("new@ %p", p);
  #ifdef _MSC_VER
    std::printf(": %Iu\n", size);
  #else
    std::printf(": %zu\n", size);
  #endif
#endif
  assert(p); // FIXME
  return p;
}

void  operator delete(void* ptr)
{
  std::free(ptr);
#ifdef PEL_TRACE_ALLOCATOR
  std::printf("delete@ %p\n", ptr);
#endif
}

void* operator new[](std::size_t size)
{
  return operator new(size);
}

void  operator delete[](void* ptr)
{
  operator delete(ptr);
}


//--------------------------------
//   Allocator test
//--------------------------------
#include"test.h"

namespace pel{

class TestClass{
public:
  int x;
  int y;

  TestClass(){
    x = 1;
    y = 2;
    std::printf("TestClass Ctor\n");
  }

  ~TestClass(){
    std::printf("TestClass Dtor\n");
  }
};

void TestAllocator()
{
  TestBegin("Allocator");

  std::printf("new an ins\n");
  TestClass * t = new TestClass();
  delete t;

  std::printf("new an array\n");
  t = new TestClass[3];
  delete[] t;

  TestEnd();
}

}
