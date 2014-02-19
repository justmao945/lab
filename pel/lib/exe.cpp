
#include<cassert>
#include<cstring>
#include"ctx.h"
#include"exe.h"
#include"rt.h"
#include"rtti.h"
#include"token.h"

using namespace pel;

//--------------------------------------------
//  Prepare units
//--------------------------------------------

// -- impl ExeUnit::Destroy() --
static void delExePush(ExeUnit *p){
  ExePush *q = dyn_cast<ExePush>(p);
  assert(q);
  delete q;
}

static void delExePop(ExeUnit *p){
  ExePop *q = dyn_cast<ExePop>(p);
  assert(q);
  delete q;
}

static void delExeModelIdent(ExeUnit *p){
  ExeModelIdent *q = dyn_cast<ExeModelIdent>(p);
  assert(q);
  delete q;
}

static void delExeModelType(ExeUnit *p){
  ExeModelType *q = dyn_cast<ExeModelType>(p);
  assert(q);
  delete q;
}

static void delExeModelLabel(ExeUnit *p){
  ExeModelLabel *q = dyn_cast<ExeModelLabel>(p);
  assert(q);
  delete q;
}

static void delExePathDecl(ExeUnit *p){
  ExePathDecl *q = dyn_cast<ExePathDecl>(p);
  assert(q);
  delete q;
}

static void delExeOr(ExeUnit *p){
  ExeOr *q = dyn_cast<ExeOr>(p);
  assert(q);
  delete q;
}

static void delExeAnd(ExeUnit *p){
  ExeAnd *q = dyn_cast<ExeAnd>(p);
  assert(q);
  delete q;
}

static void delExeLT(ExeUnit *p){
  ExeLT *q = dyn_cast<ExeLT>(p);
  assert(q);
  delete q;
}

static void delExeGT(ExeUnit *p){
  ExeGT *q = dyn_cast<ExeGT>(p);
  assert(q);
  delete q;
}

static void delExeLE(ExeUnit *p){
  ExeLE *q = dyn_cast<ExeLE>(p);
  assert(q);
  delete q;
}

static void delExeGE(ExeUnit *p){
  ExeGE *q = dyn_cast<ExeGE>(p);
  assert(q);
  delete q;
}

static void delExeEQ(ExeUnit *p){
  ExeEQ *q = dyn_cast<ExeEQ>(p);
  assert(q);
  delete q;
}

static void delExeNE(ExeUnit *p){
  ExeNE *q = dyn_cast<ExeNE>(p);
  assert(q);
  delete q;
}

static void delExeLike(ExeUnit *p){
  ExeLike *q = dyn_cast<ExeLike>(p);
  assert(q);
  delete q;
}

static void delExeFunc(ExeUnit *p){
  ExeFunc *q = dyn_cast<ExeFunc>(p);
  assert(q);
  delete q;
}

static void delExeRecur(ExeUnit *p){
  ExeRecur *q = dyn_cast<ExeRecur>(p);
  assert(q);
  delete q;
}

static void delExeVal(ExeUnit *p){
  ExeVal *q = dyn_cast<ExeVal>(p);
  assert(q);
  delete q;
}

static void delExeList(ExeUnit *p){
  ExeList *q = dyn_cast<ExeList>(p);
  assert(q);
  delete q;
}

static void delExeInter(ExeUnit *p){
  ExeInter *q = dyn_cast<ExeInter>(p);
  assert(q);
  delete q;
}

static void delExeToler(ExeUnit *p){
  ExeToler *q = dyn_cast<ExeToler>(p);
  assert(q);
  delete q;
}

static void delExeVar(ExeUnit *p){
  ExeVar *q = dyn_cast<ExeVar>(p);
  assert(q);
  delete q;
}

static void delExeMetaVar(ExeUnit *p){
  ExeMetaVar *q = dyn_cast<ExeMetaVar>(p);
  assert(q);
  delete q;
}

static void delExeIndex(ExeUnit *p){
  ExeIndex *q = dyn_cast<ExeIndex>(p);
  assert(q);
  delete q;
}

void ExeUnit::Destroy()
{
  static void(*vtbl[ExeUnit::ClassIDSize])(ExeUnit*) = 
  {
    0,
    delExePush,
    delExePop,
    delExeModelIdent,
    delExeModelType,
    delExeModelLabel,
    delExePathDecl,
    delExeOr,
    delExeAnd,
    delExeLT,
    delExeGT,
    delExeLE,
    delExeGE,
    delExeEQ,
    delExeNE,
    delExeLike,
    delExeFunc,
    delExeRecur,
    delExeVal,
    delExeList,
    delExeInter,
    delExeToler,
    delExeVar,
    delExeMetaVar,
    delExeIndex,
  };

  vtbl[ this->ID() ](this);
}

// ----  impl ExeUnit::Exe()  ---
Error ExeUnit::Exe(ExeUnit *thiz, Context& ctx)
{
  assert(thiz);

  static Error (*vtbl[ExeUnit::ClassIDSize])(ExeUnit*, Context&) = 
  {
    0,
    ExePush::Exe,
    ExePop::Exe,
    ExeModelIdent::Exe,
    ExeModelType::Exe,
    ExeModelLabel::Exe,
    ExePathDecl::Exe,
    ExeOr::Exe,
    ExeAnd::Exe,
    ExeLT::Exe,
    ExeGT::Exe,
    ExeLE::Exe,
    ExeGE::Exe,
    ExeEQ::Exe,
    ExeNE::Exe,
    ExeLike::Exe,
    ExeFunc::Exe,
    ExeRecur::Exe,
    ExeVal::Exe,
    ExeList::Exe,
    ExeInter::Exe,
    ExeToler::Exe,
    ExeVar::Exe,
    ExeMetaVar::Exe,
    ExeIndex::Exe,
  };

  return vtbl[ thiz->ID() ](thiz, ctx);
}



// Execution units impl
Error ExePush::Exe(ExeUnit* thiz, Context& ctx)
{
  assert( dyn_cast<ExePush>(thiz) );
  ctx.Push(0); // push nothing, just move stack pointer
  return Error();
}

Error ExePop::Exe(ExeUnit* thiz, Context& ctx)
{
  ExePop *e = dyn_cast<ExePop>(thiz);
  assert(e);
  ctx.Pop( e->n );
  return Error();
}

Error ExeModelIdent::Exe(ExeUnit* thiz, Context& ctx)
{
  ExeModelIdent *exe = dyn_cast<ExeModelIdent>(thiz);
  assert( exe );
  
  Runtime *rt = ctx.Rt();
  int modid = ctx.CurID();
  
  if( modid == -1 ){
    return Error("Model ID not set in context");
  }

  // 1. model field
  assert(exe->ident);
  int fid = rt->Field(modid, exe->ident->Text());
  if( fid != -1 ){ // field

  }
  
  // 2. relationship between models
  
  
  // 3. defined label
  

  return Error();
}

Error ExeModelType::Exe(ExeUnit* thiz, Context& ctx)
{
  assert( dyn_cast<ExeModelType>(thiz) );

  return Error();
}

Error ExeModelLabel::Exe(ExeUnit* thiz, Context& ctx)
{
  assert( dyn_cast<ExeModelLabel>(thiz) );

  return Error();
}

Error ExePathDecl::Exe(ExeUnit* thiz, Context& ctx)
{
  assert( dyn_cast<ExePathDecl>(thiz) );

  return Error();
}

Error ExeOr::Exe(ExeUnit* thiz, Context& ctx)
{
  assert( dyn_cast<ExeOr>(thiz) );

  return Error();
}

Error ExeAnd::Exe(ExeUnit* thiz, Context& ctx)
{
  assert( dyn_cast<ExeAnd>(thiz) );

  return Error();
}

Error ExeLT::Exe(ExeUnit* thiz, Context& ctx)
{
  assert( dyn_cast<ExeLT>(thiz) );

  return Error();
}

Error ExeGT::Exe(ExeUnit* thiz, Context& ctx)
{
  assert( dyn_cast<ExeGT>(thiz) );

  return Error();
}

Error ExeLE::Exe(ExeUnit* thiz, Context& ctx)
{
  assert( dyn_cast<ExeLE>(thiz) );

  return Error();
}

Error ExeGE::Exe(ExeUnit* thiz, Context& ctx)
{
  assert( dyn_cast<ExeGE>(thiz) );

  return Error();
}

Error ExeEQ::Exe(ExeUnit* thiz, Context& ctx)
{
  assert( dyn_cast<ExeEQ>(thiz) );

  return Error();
}

Error ExeNE::Exe(ExeUnit* thiz, Context& ctx)
{
  assert( dyn_cast<ExeNE>(thiz) );

  return Error();
}

Error ExeLike::Exe(ExeUnit* thiz, Context& ctx)
{
  assert( dyn_cast<ExeLike>(thiz) );

  return Error();
}

Error ExeFunc::Exe(ExeUnit* thiz, Context& ctx)
{
  assert( dyn_cast<ExeFunc>(thiz) );

  return Error();
}

Error ExeRecur::Exe(ExeUnit* thiz, Context& ctx)
{
  assert( dyn_cast<ExeRecur>(thiz) );

  return Error();
}

Error ExeVal::Exe(ExeUnit* thiz, Context& ctx)
{
  assert( dyn_cast<ExeVal>(thiz) );

  return Error();
}

Error ExeList::Exe(ExeUnit* thiz, Context& ctx)
{
  assert( dyn_cast<ExeList>(thiz) );

  return Error();
}

Error ExeInter::Exe(ExeUnit* thiz, Context& ctx)
{
  assert( dyn_cast<ExeInter>(thiz) );

  return Error();
}

Error ExeToler::Exe(ExeUnit* thiz, Context& ctx)
{
  assert( dyn_cast<ExeToler>(thiz) );

  return Error();
}

Error ExeVar::Exe(ExeUnit* thiz, Context& ctx)
{
  assert( dyn_cast<ExeVar>(thiz) );

  return Error();
}

Error ExeMetaVar::Exe(ExeUnit* thiz, Context& ctx)
{
  assert( dyn_cast<ExeMetaVar>(thiz) );

  return Error();
}

Error ExeIndex::Exe(ExeUnit* thiz, Context& ctx)
{
  assert( dyn_cast<ExeIndex>(thiz) );

  return Error();
}


//--------------------------------------------
//  Executor impl
//--------------------------------------------

Executor::Executor()
{
}

Executor::Executor(const Executor& e)
{
}

Executor::~Executor()
{
}

Executor& Executor::operator=(const Executor& e)
{
  if(this != &e){

  }
  return *this;
}

Executor& Executor::AddUnit(const ExeUnit *u)
{
  assert(u);
  units.Push(u);
  return *this;
}
