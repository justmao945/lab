
#include<cassert>
#include<cstdio>
#include"rtti.h"
#include"test.h"

//--------------------------------
//   RTTI test
//--------------------------------
namespace pel{

class Base{
public:
  enum ClassID{ // class id to simulate rtti
    Base_,
    D1_,
    D2_,
    D3_,
  };

  ClassID ID() const { return id; }

  // *static method* e.g. used by isa<T>(x)
  static bool classof(const Base* b){
    return b->id == Base_;
  }

  void Destroy();

protected:
  Base(){
    id = Base_;
  }

  // used by sub classes
  explicit Base(ClassID id){
    this->id = id;
  }

  // not used
  ~Base(){ printf("~Base()\n"); }

private:
  ClassID id;
};

class D1: public Base{
public:

  // every ctor should init its ID through base ctor.
  D1() :Base(D1_) {
  }
  ~D1(){
    printf("~D1()\n");
  }
  
  // test
  static bool classof(const Base* b){
    return b->ID() == D1_;
  }
};

class D2: public Base{
public:
  D2(): Base(D2_){}
  ~D2(){
    printf("~D2()\n");
  }

  static bool classof(const Base* b){
    return b->ID() == D2_;
  }
};


static void delD1(Base *p)
{
  printf("delD1\n");
  delete dyn_cast<D1>(p);
}

static void delD2(Base *p)
{
  printf("delD2\n");
  delete dyn_cast<D2>(p);
}

void Base::Destroy()
{
  static void(*vtbl[3])(Base*) = {
    0,
    delD1,
    delD2,
  };
  vtbl[ this->ID() ](this);
}



void TestRTTI()
{
  TestBegin("RTTI");

  D1 d1;
  D2 d2;

  const D1 cd1;

  D1 *pd1 = &d1;
  D2 *pd2 = &d2;

  D1 **ppd1 = &pd1; // this is valid =,=  bug?

  const D1* cpd1 = pd1;
   D2* const pcd2 = pd2;
  const D2* const cpcd2 = pd2;

  std::printf("d1 %d\n", isa<Base>(d1)); // 0
  std::printf("cd1 %d\n", isa<D2>(cd1));  // 0
  std::printf("pd1 %d\n", isa<D1>(pd1));  // 1
  std::printf("cpd1 %d\n", isa<D2>(cpd1)); // 0
  std::printf("pcd2 %d\n", isa<D2>(pcd2)); // 1
  std::printf("cpcd2 %d\n", isa<D2>(cpcd2)); // 1
  std::printf("ppd1 %d\n", isa<D1>(ppd1)); // 1
  std::putchar('\n');

  Base *b = pd1;
  pd1 = dyn_cast<D1>(b);
  std::printf("pd1 %p\n", pd1); // 0x???
  pd2 = dyn_cast<D2>(b);
  std::printf("pd2 %p\n", pd2); // 0

  std::putchar('\n');
  std::printf("d1.ID() = %d, d2.ID() = %d\n", d1.ID(), d2.ID());

  Base *b1 = new D1;
  b1->Destroy();

  TestEnd();
}


} // ~pel
