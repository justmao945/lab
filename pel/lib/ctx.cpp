
#include<cassert>
#include<cstdio>
#include<cstring>
#include"ctx.h"
#include"new.h"
#include"rt.h"

using namespace pel;

// ------ impl StkVal

StkVal::StkVal(std::size_t rows, std::size_t columns)
{
  this->curid = -1;
  this->rows = rows;
  this->cols = columns;
  this->data = new Value*[rows];
  for(std::size_t i=0; i<rows; ++i){
    this->data[i] = new Value[columns];
    
    // Must init to 0.
    std::memset(this->data[i], 0, sizeof(Value) * columns);
  }

  this->types = new TypeRep[columns];
  std::memset(this->types, 0, sizeof(TypeRep) * columns); // Null is 0
}

StkVal::~StkVal()
{
  for(std::size_t i=0; i<rows; ++i){
    delete[] data[i];
  }
  delete[] data;
  delete[] types;
}


// ----  Impl Context  -----

struct CleanPath{
  void operator()(const char* key, StkVal* val) {
    // keys are stored in AST.
    delete val;
  }
};

Context::Context(Runtime *rt)
{
  assert(rt);
  this->rt = rt;
  rootmod = -1;
}

Context::~Context()
{
  Clean();
}

Context& Context::Clean()
{
  for(std::size_t i=0; i<stk.Size(); ++i){
    delete stk[i];
  }
  
  static CleanPath clp;
  paths.Each(clp);
  
  stk.Clean();
  paths.Clean();
  return *this;
}

int Context::CurID()const
{
  assert( !stk.Empty() );
  if( stk.Top() == 0 ){
    return rootmod;
  }else{
    return stk.Top()->CurID();
  }
}

Error Context::Load()
{
  Clean();

  // Prepare stack to hold one empty element as the top,
  // because every operation is based on stack top.
  stk.Push(0);

  assert(rt);
  const char* root = rt->Env("ROOT");
  if(root == 0){
    return Error("Undefined variable ROOT");
  }

  rootmod = rt->Model(root);
  if( rootmod == -1 ){
    return Error("Unknown root model name");
  }

  return Error();
}

StkVal* Context::Path(const char* ident)
{
  Map<const char*, StkVal*>::Box *b = paths.Find(ident);
  return b ? b->Value() : 0;
}

StkVal* Context::Path(const char* ident, StkVal* val)
{
  Map<const char*, StkVal*>::Box *b = paths.Add(ident, val);
  return b ? b->Value() : 0;
}

Context& Context::Pop(std::size_t n)
{
  while(n--){
    delete stk.Top();
    stk.Pop();
  }
  return *this;
}

Context& Context::Push(StkVal *val)
{
  stk.Push(val);
  return *this;
}

Runtime* Context::Rt()
{
  return rt;
}

StkVal* Context::Top(std::size_t n)
{
  assert(n < stk.Size() );
  return stk.Back(n);
}


