#include<cassert>
#include<cstdio>
#include<cstring>
#include"ast.h"
#include"exe.h"
#include"new.h"
#include"rtti.h"
#include"token.h"

//-------------------------
//    ASTree impl
//-------------------------
using namespace pel;

// ------------------------- ASTree ------------------------
void ASTree::Print(FILE *stream)const
{
  Vector<char> trunk; // to store and share trunk info
  trunk.Push('\0'); // empty string
  std::printf("%sPathExpr %p\n", trunk.Data(), this->root);
  assert(this->root);
  this->root->Print(trunk);
}

Token *ASTree::newToken()
{
  Token *tok = new Token();
  tokens.Push(tok);
  return tok;
}

ASTree::~ASTree()
{
  for(std::size_t i=0; i<nodes.Size(); ++i){
    nodes[i]->Destroy();
  }
  for(std::size_t i=0; i<tokens.Size(); ++i){
    delete tokens[i];
  }
}


// ---------------- ASTNode::Destroy() impl -----------------
static void delPathExpr(ASTNode *p){
  ASTPathExpr *q = dyn_cast<ASTPathExpr>(p);
  assert(q);
  delete q;
}

static void delPathElemExpr(ASTNode *p){
  ASTPathElemExpr *q = dyn_cast<ASTPathElemExpr>(p);
  assert(q);
  delete q;
}

static void delModelElemExpr(ASTNode *p){
  ASTModelElemExpr *q = dyn_cast<ASTModelElemExpr>(p);
  assert(q);
  delete q;
}

static void delTypeCastExpr(ASTNode *p){
  ASTypeCastExpr* q = dyn_cast<ASTypeCastExpr>(p);
  assert(q);
  delete q;
}

static void delNodeLabelExpr(ASTNode *p){
  ASTNodeLabelExpr* q = dyn_cast<ASTNodeLabelExpr>(p);
  assert(q);
  delete q;
}

static void delNodeCastExpr(ASTNode *p){
  ASTNodeCastExpr* q = dyn_cast<ASTNodeCastExpr>(p);
  assert(q);
  delete q;
}

static void delPathDecl(ASTNode *p){
  ASTPathDecl *q = dyn_cast<ASTPathDecl>(p);
  assert(q);
  delete q;
}

static void delCondExpr(ASTNode *p){
  ASTCondExpr *q = dyn_cast<ASTCondExpr>(p);
  assert(q);
  delete q;
}

static void delOrExpr(ASTNode *p){
  ASTOrExpr *q = dyn_cast<ASTOrExpr>(p);
  assert(q);
  delete q;
}

static void delAndExpr(ASTNode *p){
  ASTAndExpr *q = dyn_cast<ASTAndExpr>(p);
  assert(q);
  delete q;
}

static void delCondTerm(ASTNode *p){
  ASTCondTerm *q = dyn_cast<ASTCondTerm>(p);
  assert(q);
  delete q;
}

static void delBaseTerm(ASTNode *p){
  ASTBaseTerm *q = dyn_cast<ASTBaseTerm>(p);
  assert(q);
  delete q;
}

static void delRelOp(ASTNode *p){
  ASTRelOp *q = dyn_cast<ASTRelOp>(p);
  assert(q);
  delete q;
}

static void delFuncExpr(ASTNode *p){
  ASTFuncExpr *q = dyn_cast<ASTFuncExpr>(p);
  assert(q);
  delete q;
}

static void delRecurPathExpr(ASTNode *p){
  ASTRecurPathExpr *q = dyn_cast<ASTRecurPathExpr>(p);
  assert(q);
  delete q;
}

static void delConstExpr(ASTNode *p){
  ASTConstExpr *q = dyn_cast<ASTConstExpr>(p);
  assert(q);
  delete q;
}

static void delComplexLit(ASTNode *p){
  ASTComplexLit *q = dyn_cast<ASTComplexLit>(p);
  assert(q);
  delete q;
}

static void delValLit(ASTNode *p){
  ASTValLit *q = dyn_cast<ASTValLit>(p);
  assert(q);
  delete q;
}

static void delListLit(ASTNode *p){
  ASTListLit *q = dyn_cast<ASTListLit>(p);
  assert(q);
  delete q;
}

static void delInterLit(ASTNode *p){
  ASTInterLit *q = dyn_cast<ASTInterLit>(p);
  assert(q);
  delete q;
}

static void delTolerLit(ASTNode *p){
  ASTolerLit *q = dyn_cast<ASTolerLit>(p);
  assert(q);
  delete q;
}

static void delVarExpr(ASTNode *p){
  ASTVarExpr *q = dyn_cast<ASTVarExpr>(p);
  assert(q);
  delete q;
}

static void delMetaVarExpr(ASTNode *p){
  ASTMetaVarExpr *q = dyn_cast<ASTMetaVarExpr>(p);
  assert(q);
  delete q;
}

static void delIndexOp(ASTNode *p){
  ASTIndexOp *q = dyn_cast<ASTIndexOp>(p);
  assert(q);
  delete q;
}

void ASTNode::Destroy()
{
  static void(*vtbl[ASTNode::ClassIDSize])(ASTNode*) = 
  {
    0,
    delPathExpr,
    delPathElemExpr,
    delModelElemExpr,
    delTypeCastExpr,
    delNodeLabelExpr,
    delNodeCastExpr,
    delPathDecl,
    delCondExpr,
    delOrExpr,
    delAndExpr,
    delCondTerm,
    delBaseTerm,
    delRelOp,
    delFuncExpr,
    delRecurPathExpr,
    delConstExpr,
    delComplexLit,
    delValLit,
    delListLit,
    delInterLit,
    delTolerLit,
    delVarExpr,
    delMetaVarExpr,
    delIndexOp,
  };
  
  vtbl[ this->ID() ](this);
}

//---------------- Nodes Print() impl ------------

void ASTPathExpr::Print(Vector<char>& trunk) const
{
  trunk.Pop().Push<3>("|-");
  for(std::size_t i=0; i+1<elems.Size(); ++i){
    trunk.Back(1) = '-';
    std::printf("%sPathElemExpr %p\n", trunk.Data(), elems[i]);
    trunk.Back(1) = ' ';
    elems[i]->Print(trunk);
  }

  trunk.Back(1) = '-';
  if( ! elems.Empty() ){
    trunk.Back(2) = '`'; // the one before last one
    std::printf("%sPathElemExpr %p\n", trunk.Data(), elems.Back());
    trunk.Back(2) = ' ';
    trunk.Back(1) = ' ';
    assert(elems.Back());
    elems.Back()->Print(trunk);
  }else{
    assert(0);
  }
  trunk.Pop(3).Push('\0');
}

void ASTPathElemExpr::Print(Vector<char>& trunk)const
{
  trunk.Pop().Push<3>("|-");

  if( index != 0 ){
    std::printf("%sIndexOp %p ", trunk.Data(), index);
    assert(index->intv);
    index->intv->Print();
    std::putchar('\n');
  }
  
  trunk.Back(2) = '`'; // the one before last one

  switch( sel.ptr->ID() ){
  case ASTModelElemExpr_:
    std::printf("%sModelElemExpr %p ", trunk.Data(), sel.model);
    if( sel.model->ident ){
      assert(sel.model->ident);
      sel.model->ident->Print();
      std::putchar('\n');
    }
    trunk.Back(1) = ' ';
    trunk.Back(2) = ' ';
    sel.model->Print(trunk);
    break;

  case ASTFuncExpr_:
    std::printf("%sFuncExpr %p ", trunk.Data(), sel.func);
    assert(sel.func->ident);
    sel.func->ident->Print();
    std::putchar('\n');
    trunk.Back(1) = ' ';
    trunk.Back(2) = ' ';
    sel.func->Print(trunk);
    break;

  case ASTRecurPathExpr_:
    std::printf("%sRecurPathExpr %p %d\n", trunk.Data(), sel.path, sel.path->plusplus);
    trunk.Back(1) = ' ';
    trunk.Back(2) = ' ';
    sel.path->Print(trunk);
    break;

  case ASTConstExpr_:
    std::printf("%sConstExpr %p\n", trunk.Data(), sel.constt);
    trunk.Back(1) = ' ';
    trunk.Back(2) = ' ';
    assert(sel.constt);
    sel.constt->Print(trunk);
    break;

  default:
    std::printf("ClassID: %d\n", sel.ptr->ID());
    assert(0);
    break;
  }
  trunk.Pop(3).Push('\0');

}


void ASTModelElemExpr::Print(Vector<char>& trunk)const
{
  trunk.Pop().Push<3>("|-");

  if( type ){
    if( label == 0 && node == 0 ){
      trunk.Back(2) = '`';
    }
    std::printf("%sTypeCastExpr %p ", trunk.Data(), type);
    assert(type->ident);
    type->ident->Print();
    std::putchar('\n');
  }

  if( label ){
    if( node == 0 ){
      trunk.Back(2) = '`';
    }
    std::printf("%sNodeLabelExpr %p ", trunk.Data(), label);
    assert(label->ident);
    label->ident->Print();
    std::putchar('\n');
  }


  if( node ){
    trunk.Back(2) = '`';
    std::printf("%sNodeCastExpr %p\n", trunk.Data(), node);
    trunk.Back(1) = ' ';
    trunk.Back(2) = ' ';
    node->Print(trunk);
  }

  assert( type || label || node  || ident);

  trunk.Pop(3).Push('\0');
}

void ASTypeCastExpr::Print(Vector<char>& trunk)const
{
}

void ASTNodeLabelExpr::Print(Vector<char>& trunk)const
{
}

void ASTNodeCastExpr::Print(Vector<char>& trunk)const
{
  trunk.Pop().Push<3>("|-");

  for(std::size_t i=0; i<paths.Size(); ++i){
    trunk.Back(1) = '-';
    std::printf("%sPathDecl %p ", trunk.Data(), paths[i]);
    assert(paths[i]->ident);
    paths[i]->ident->Print();
    std::putchar('\n');
    trunk.Back(1) = ' ';
    paths[i]->Print(trunk);
  }

  trunk.Back(2) = '`';
  std::printf("%sCondExpr %p\n", trunk.Data(), cond);
  trunk.Back(1) = ' ';
  trunk.Back(2) = ' ';
  cond->Print(trunk);

  trunk.Pop(3).Push('\0');
}

void ASTPathDecl::Print(Vector<char>& trunk)const
{
  trunk.Pop().Push<3>("`-");

  std::printf("%sPathExpr %p\n", trunk.Data(), path);
  trunk.Back(1) = ' ';
  trunk.Back(2) = ' ';
  path->Print(trunk);
  trunk.Pop(3).Push('\0');
}

void ASTCondExpr::Print(Vector<char>& trunk)const
{
  trunk.Pop().Push<3>("`-");

  std::printf("%sOrExpr %p\n", trunk.Data(), or_);
  trunk.Back(1) = ' ';
  trunk.Back(2) = ' ';
  or_->Print(trunk);
  trunk.Pop(3).Push('\0');
}

void ASTOrExpr::Print(Vector<char>& trunk)const
{
  trunk.Pop().Push<3>("|-");

  for(std::size_t i=0; i+1 < ands.Size(); ++i){
    std::printf("%sAndExpr %p\n", trunk.Data(), ands[i]);
    trunk.Back(1) = ' ';
    ands[i]->Print(trunk);
  }

  if( ! ands.Empty() ){
    trunk.Back(2) = '`';
    std::printf("%sAndExpr %p\n", trunk.Data(), ands.Back());
    trunk.Back(1) = ' ';
    trunk.Back(2) = ' ';
    ands.Back()->Print(trunk);
  }else{
    assert(0);
  }
  trunk.Pop(3).Push('\0');
}

void ASTAndExpr::Print(Vector<char>& trunk)const
{
  trunk.Pop().Push<3>("|-");

  for(std::size_t i=0; i+1 < terms.Size(); ++i){
    std::printf("%sCondTerm %p\n", trunk.Data(), terms[i]);
    trunk.Back(1) = ' ';
    terms[i]->Print(trunk);
  }

  if( ! terms.Empty() ){
    trunk.Back(2) = '`';
    std::printf("%sCondTerm %p\n", trunk.Data(), terms.Back());
    trunk.Back(1) = ' ';
    trunk.Back(2) = ' ';
    terms.Back()->Print(trunk);
  }else{
    assert(0);
  }
  trunk.Pop(3).Push('\0');
}

void ASTCondTerm::Print(Vector<char>& trunk)const
{
  trunk.Pop().Push<3>("`-");

  switch( sel.ptr->ID() ){
  case ASTBaseTerm_:
    std::printf("%sBaseTerm %p\n", trunk.Data(), sel.base);
    trunk.Back(1) = ' ';
    trunk.Back(2) = ' ';
    sel.base->Print(trunk);
    break;

  case ASTCondExpr_:
    std::printf("%sCondExpr %p\n", trunk.Data(), sel.cond);
    trunk.Back(1) = ' ';
    trunk.Back(2) = ' ';
    sel.cond->Print(trunk);
    break;

  default:
    std::printf("ClassID %d\n", sel.ptr->ID());
    assert(0);
    break;
  }

  trunk.Pop(3).Push('\0');
}

void ASTBaseTerm::Print(Vector<char>& trunk)const
{
  trunk.Pop().Push<3>("|-");

  std::printf("%sPathExpr %p\n", trunk.Data(), lpath);
  trunk.Back(1) = ' ';
  lpath->Print(trunk);

  trunk.Back(1) = '-';
  std::printf("%sRelOp %p ", trunk.Data(), op);
  op->op->Print();
  std::putchar('\n');

  trunk.Back(2) = '`';
  std::printf("%sPathExpr %p\n", trunk.Data(), rpath);
  trunk.Back(1) = ' ';
  trunk.Back(2) = ' ';
  rpath->Print(trunk);

  trunk.Pop(3).Push('\0');
}

void ASTRelOp::Print(Vector<char>& trunk)const
{
}

void ASTFuncExpr::Print(Vector<char>& trunk)const
{
  trunk.Pop().Push<3>("|-");

  for(std::size_t i=0; i+1 < paths.Size(); ++i){
    std::printf("%sPathExpr %p\n", trunk.Data(), paths[i]);
    trunk.Back(1) = ' ';
    paths[i]->Print(trunk);
  }

  if( ! paths.Empty() ){
    trunk.Back(2) = '`';
    std::printf("%sPathExpr %p\n", trunk.Data(), paths.Back());
    trunk.Back(1) = ' ';
    trunk.Back(2) = ' ';
    paths.Back()->Print(trunk);
  }else{
    assert(0);
  }

  trunk.Pop(3).Push('\0');
}

void ASTRecurPathExpr::Print(Vector<char>& trunk)const
{
  trunk.Pop().Push<3>("`-");

  std::printf("%sPathExpr %p\n", trunk.Data(), path);
  trunk.Back(1) = ' ';
  trunk.Back(2) = ' ';
  path->Print(trunk);
  trunk.Pop(3).Push('\0');
}

void ASTConstExpr::Print(Vector<char>& trunk)const
{
  trunk.Pop().Push<3>("`-");

  switch( sel.ptr->ID() ){
  case ASTComplexLit_:
    std::printf("%sComplexLit %p\n", trunk.Data(), sel.complex_);
    trunk.Back(1) = ' ';
    trunk.Back(2) = ' ';
    sel.complex_->Print(trunk);
    break;

  case ASTVarExpr_:
    std::printf("%sVarExpr %p ", trunk.Data(), sel.var);
    assert(sel.var->ident);
    sel.var->ident->Print();
    std::putchar('\n');
    break;

  case ASTMetaVarExpr_:
    std::printf("%sMetaVarExpr %p ", trunk.Data(), sel.meta);
    assert(sel.meta->ident);
    sel.meta->ident->Print();
    std::putchar('\n');
    break;

  default:
    std::printf("ClassID %d\n", sel.ptr->ID());
    assert(0);
    break;
  }

  trunk.Pop(3).Push('\0');
}

void ASTComplexLit::Print(Vector<char>& trunk)const
{
  trunk.Pop().Push<3>("`-");

  switch( sel.ptr->ID() ){
  case ASTValLit_:
    std::printf("%sValLit %p ", trunk.Data(), sel.val);
    assert(sel.val->val);
    sel.val->val->Print();
    std::putchar('\n');
    break;

  case ASTListLit_:
    std::printf("%sListLit %p [", trunk.Data(), sel.list);
    if( !sel.list->vals.Empty() ){
      assert(sel.list->vals.Front()->val);
      sel.list->vals.Front()->val->Print();
    }
    for( std::size_t i=1; i<sel.list->vals.Size(); ++i ){
      std::printf(", ");
      assert(sel.list->vals[i]->val);
      sel.list->vals[i]->val->Print();
    }
    std::printf("]\n");
    break;

  case ASTInterLit_:
    std::printf("%sInterLit %p [", trunk.Data(), sel.inter);
    assert(sel.inter->l->val);
    sel.inter->l->val->Print();
    std::printf("..");
    assert(sel.inter->r->val);
    sel.inter->r->val->Print();
    break;

  default:
    std::printf("ClassID %d\n", sel.ptr->ID());
    assert(0);
    break;
  }

  trunk.Pop(3).Push('\0');
}

void ASTVarExpr::Print(Vector<char>& trunk)const
{
}

void ASTMetaVarExpr::Print(Vector<char>& trunk)const
{
}

void ASTIndexOp::Print(Vector<char>& trunk)const
{
}


//  Prepare executor impl
//-----------------------------------------------------------
void ASTPathExpr::Prepare(Executor& exe)const
{
  for(std::size_t i=0; i<elems.Size(); ++i){
    elems[i]->Prepare(exe);
  }
}

void ASTPathElemExpr::Prepare(Executor& exe)const
{
  assert(sel.ptr);

  switch( sel.ptr->ID() ){
  case ASTModelElemExpr_:
    sel.model->Prepare(exe);
    break;
    
  case ASTFuncExpr_:
    sel.func->Prepare(exe);
    break;
    
  case ASTRecurPathExpr_:
    sel.path->Prepare(exe);
    break;
    
  case ASTConstExpr_:
    sel.constt->Prepare(exe);
    break;
    
  default:
    assert(0);
    break;
  }
}

void ASTModelElemExpr::Prepare(Executor& exe)const
{
  // | res | <-top
  if( ident ){
    ExeModelIdent *e = new ExeModelIdent(); // operates on stack top
    e->ident = this->ident;
    exe.AddUnit(e);
  }

  //
  // FIXME: new ExeXXX() change to no new? Memory optimization.
  // 

  if( type ){
    type->Prepare(exe);
  }

  if( label ){
    label->Prepare(exe);
  }

  if( node ){
    node->Prepare(exe);
  }
}

void ASTypeCastExpr::Prepare(Executor& exe)const
{
  // | res | <-top
  ExeModelType *e = new ExeModelType();// operates on stack top
  e->ident = this->ident;
  exe.AddUnit(e);
}

void ASTNodeLabelExpr::Prepare(Executor& exe)const
{
  // | res | <-top
  ExeModelLabel *e = new ExeModelLabel();// operates on stack top
  e->ident = this->ident;
  exe.AddUnit(e);
}

void ASTNodeCastExpr::Prepare(Executor& exe)const
{
  for(std::size_t i=0; i<paths.Size(); ++i){
    paths[i]->Prepare(exe);
  }

  if(cond){
    cond->Prepare(exe);
  }
}

void ASTPathDecl::Prepare(Executor& exe)const
{
  // | res |
  // | res | <-top
  exe.AddUnit( new ExePush() ); // save stack top

  // | res  |
  // | path | <-top
  path->Prepare(exe); // generate new result to stack top

  ExePathDecl *decl = new ExePathDecl();// operates on stack top
  decl->ident = this->ident;
  exe.AddUnit( decl );  // assign new result to ident

  // | res |
  // | res | <-top
  exe.AddUnit( new ExePop() ); // restore stack top
}

void ASTCondExpr::Prepare(Executor& exe)const
{
  return or_->Prepare(exe);
}

void ASTOrExpr::Prepare(Executor& exe)const
{
  // After preparing BaseTerm:
  //
  // | res |
  // | lop |
  // | rop | <- top
  //
  // ExeOr needs two operands
  assert( ands.Size() % 2 == 1 );
  ands.Front()->Prepare(exe);

  for(std::size_t i=1; i<ands.Size(); ++i){
    ands[i]->Prepare(exe);
    exe.AddUnit( new ExeOr() );// operates on stack top
  }
}

void ASTAndExpr::Prepare(Executor& exe)const
{
  // After preparing BaseTerm:
  //
  // | res |
  // | lop |
  // | rop | <- top
  //
  // ExeAnd needs two operands
  assert( terms.Size() % 2 == 1 );
  terms.Front()->Prepare(exe);

  for(std::size_t i=1; i<terms.Size(); ++i){
    terms[i]->Prepare(exe);
    exe.AddUnit( new ExeAnd() );
  }
}

void ASTCondTerm::Prepare(Executor& exe)const
{
  switch( sel.ptr->ID() ){
  case ASTBaseTerm_:
    sel.base->Prepare(exe);
    break;
    
  case ASTCondExpr_:
    sel.cond->Prepare(exe);
    break;
    
  default:
    assert(0);
    break;
  }
}

void ASTBaseTerm::Prepare(Executor& exe)const
{
  // | res |
  // | res | <- top
  exe.AddUnit( new ExePush() ); // save stack top

  // | res   | 
  // | lpath |
  // | lpath | <- top
  lpath->Prepare(exe);
  exe.AddUnit( new ExePush() ); // save lpath result

  // | res   | 
  // | lpath |
  // | rpath | <- top
  rpath->Prepare(exe); // prepare to stack top

  // | res   | 
  // | lpath |
  // | rpath | <- top
  // op operates on above 3 operands and save to old result.
  assert(op);
  assert(op->op);
  switch( op->op->Rep() ){
  case TOK_lt:
    exe.AddUnit( new ExeLT() );
    break;
    
  case TOK_gt:
    exe.AddUnit( new ExeGT() );
    break;
    
  case TOK_le:
    exe.AddUnit( new ExeLE() );
    break;
    
  case TOK_ge:
    exe.AddUnit( new ExeGE() );
    break;
    
  case TOK_eq:
    exe.AddUnit( new ExeEQ() );
    break;
    
  case TOK_ne:
    exe.AddUnit( new ExeNE() );
    break;
    
  case TOK_like:
    exe.AddUnit( new ExeLike() );
    break;
    
  default:
    assert(0);
    break;
  }

  // | res | <-top
  exe.AddUnit( new ExePop() );
  exe.AddUnit( new ExePop() );
}

void ASTRelOp::Prepare(Executor& exe)const
{
  // empty
}

void ASTFuncExpr::Prepare(Executor& exe)const
{
  // | res |
  // | res | <-top
  exe.AddUnit( new ExePush() ); // save result

  // | res   |
  // | param | 1
  // | param | 2
  //    ...    ..
  // | param | n
  // | param | n <-top
  for( std::size_t i=0; i<paths.Size(); ++i ){
    paths[i]->Prepare(exe);
    exe.AddUnit( new ExePush() ); // save params
  }

  // | res   |
  // | param | 1
  // | param | 2
  //    ...    ..
  // | param | n <-top
  exe.AddUnit( new ExePop() );  // pop the last dup param

  // save result to old res
  exe.AddUnit( new ExeFunc() ); 
  
  // | res | <-top
  exe.AddUnit( new ExePop( paths.Size() ) ); // pop n params
}

void ASTRecurPathExpr::Prepare(Executor& exe)const
{
  // | res |
  // | res | <-top
  exe.AddUnit( new ExePush() ); // save result

  // | res  |
  // | path | <-top
  path->Prepare(exe); //FIXME: How to loop?

  exe.AddUnit( new ExePop() );
}

void ASTConstExpr::Prepare(Executor& exe)const
{
  assert(sel.ptr);
  switch( sel.ptr->ID() ){
  case ASTComplexLit_:
    sel.complex_->Prepare(exe);
    break;
    
  case ASTValLit_:
    sel.var->Prepare(exe);
    break;
    
  case ASTMetaVarExpr_:
    sel.meta->Prepare(exe);
    break;
    
  default:
    assert(0);
    break;
  }
}

void ASTComplexLit::Prepare(Executor& exe)const
{
  assert(sel.ptr);
  switch( sel.ptr->ID() ){
  case ASTValLit_:
    sel.val->Prepare(exe);
    break;
    
  case ASTListLit_:
    sel.list->Prepare(exe);
    break;
    
  case ASTInterLit_:
    sel.inter->Prepare(exe);
    break;
    
  case ASTolerLit_:
    sel.toler->Prepare(exe);
    break;
    
  default:
    assert(0);
    break;
  }
}

void ASTValLit::Prepare(Executor& exe)const
{
  // | ??? | <-top
  ExeVal *e = new ExeVal();
  e->val = this->val;
  exe.AddUnit(e);  // save value to stack top
}

void ASTListLit::Prepare(Executor& exe)const
{
  // | ??? | <-top
  ExeList *e = new ExeList();
  for(std::size_t i=0; i<vals.Size(); ++i){
    e->vals.Push( this->vals[i]->val );
  }

  exe.AddUnit( e ); // save list to stack top
}

void ASTInterLit::Prepare(Executor& exe)const
{
  // | ??? | <-top
  ExeInter *e = new ExeInter();
  e->l = this->l->val;
  e->r = this->r->val;

  exe.AddUnit( e ); // save inter to stack top
}

void ASTolerLit::Prepare(Executor& exe)const
{
  // | ??? | <-top
  ExeToler *e = new ExeToler();
  e->l = this->l->val;
  e->r = this->r->val;

  exe.AddUnit( e ); // save toler to stack top
}

void ASTVarExpr::Prepare(Executor& exe)const
{
  // | ??? | <-top
  ExeVar *e = new ExeVar();
  e->ident = this->ident;

  exe.AddUnit( e ); // save var to stack top
}

void ASTMetaVarExpr::Prepare(Executor& exe)const
{
  // | ??? | <-top
  ExeMetaVar *e = new ExeMetaVar();
  e->ident = this->ident;

  exe.AddUnit( e ); // save meta var to stack top
}

void ASTIndexOp::Prepare(Executor& exe)const
{
  // | res | <-top
  ExeIndex *e = new ExeIndex();
  e->intv = this->intv;

  // | res | <-top
  exe.AddUnit( e ); // overwrite res with selectd index.
}
