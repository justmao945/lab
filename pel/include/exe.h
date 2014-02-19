
/*
 * File: exe.h
 * Provides execution units by travaling AST after analysis.
 */

#ifndef PEL_EXE_H
#define PEL_EXE_H

#include"error.h"
#include"vector.h"

namespace pel{

class ASTree; // ast.h
class Context; // ctx.h
class Token; // token.h


//-----------------------------------------
//  Base class of Execution Units
//-----------------------------------------
class ExeUnit{
public:
  enum ClassID{
    ExeUnit_ = 0, // IDs consists of class name and _
    ExePush_ ,
    ExePop_ ,
    ExeModelIdent_ ,
    ExeModelType_ ,
    ExeModelLabel_ ,
    ExePathDecl_ ,
    ExeOr_ ,
    ExeAnd_ ,
    ExeLT_ ,
    ExeGT_ ,
    ExeLE_ ,
    ExeGE_ ,
    ExeEQ_ ,
    ExeNE_ ,
    ExeLike_ ,
    ExeFunc_ ,
    ExeRecur_ ,
    ExeVal_ ,
    ExeList_ ,
    ExeInter_ ,
    ExeToler_ ,
    ExeVar_ ,
    ExeMetaVar_ ,
    ExeIndex_ ,
    ClassIDSize, // indicates size of class ids
  };

  ClassID ID() const { return id; }
  
  static bool classof(const ExeUnit *p){
    return p->ID() == ExeUnit_;
  }
  
  // The only way to destroy derived classes by base pointers.
  void Destroy();

  // Executes units on the context @ctx, ctx may be changed after execution.
  // @thiz Just same this.
  // @ctx Context to execute.
  static Error Exe(ExeUnit* thiz, Context& ctx);

protected:
  ExeUnit(): id(ExeUnit_){}
  
  explicit ExeUnit(ClassID sid): id(sid){}

  ~ExeUnit(){ } // empty

private:
  ClassID id;

private:
  // disable copy
  ExeUnit(const ExeUnit&);
  ExeUnit& operator=(const ExeUnit&);
};


//--------------------------------------------------
// Every unit operates the result on a stack.
// There's only one element in the stack, which is the initial result.
//--------------------------------------------------


class ExePush: public ExeUnit{
public:
  ExePush(): ExeUnit(ExePush_){
  }

  ~ExePush(){
  }

  // Reimplement Exe() in every unit.
  static Error Exe(ExeUnit* thiz, Context& ctx);

  static bool classof(const ExeUnit* p){
    return p->ID() == ExePush_;
  }
};


class ExePop: public ExeUnit{
public:
  ExePop(): ExeUnit(ExePop_){
    n = 1;
  }

  explicit ExePop(std::size_t n): ExeUnit(ExePop_){
    this->n = n;
  }

  ~ExePop(){
  }

  static Error Exe(ExeUnit* thiz, Context& ctx);

  static bool classof(const ExeUnit* p){
    return p->ID() == ExePop_;
  }

  std::size_t n; // pop n steps
};


// Ident of ModelElemExpr
class ExeModelIdent: public ExeUnit{
public:
  ExeModelIdent(): ExeUnit(ExeModelIdent_){
    ident = 0;
  }

  ~ExeModelIdent(){
  }

  static Error Exe(ExeUnit* thiz, Context& ctx);

  static bool classof(const ExeUnit* p){
    return p->ID() == ExeModelIdent_;
  }

  const Token *ident;
};


// TypeCastExpr
class ExeModelType: public ExeUnit{
public:
  ExeModelType(): ExeUnit(ExeModelType_){
    ident = 0;
  }

  ~ExeModelType(){
  }

  static Error Exe(ExeUnit* thiz, Context& ctx);

  static bool classof(const ExeUnit* p){
    return p->ID() == ExeModelType_;
  }
  
  const Token *ident;
};


// NodeLabelExpr
class ExeModelLabel: public ExeUnit{
public:  
  ExeModelLabel(): ExeUnit(ExeModelLabel_){
    ident = 0;
  }

  ~ExeModelLabel(){
  }

  static Error Exe(ExeUnit* thiz, Context& ctx);

  static bool classof(const ExeUnit *p){
    return p->ID() == ExeModelLabel_;
  }

  const Token *ident;
};


class ExePathDecl: public ExeUnit{
public:
  ExePathDecl(): ExeUnit(ExePathDecl_){
    ident = 0;
  }

  ~ExePathDecl(){
  }

  static Error Exe(ExeUnit* thiz, Context& ctx);

  static bool classof(const ExeUnit *p){
    return p->ID() == ExePathDecl_;
  }
  
  const Token *ident;
};


class ExeOr: public ExeUnit{
public:
  ExeOr(): ExeUnit(ExeOr_){
  }

  ~ExeOr(){
  }

  static Error Exe(ExeUnit* thiz, Context& ctx);

  static bool classof(const ExeUnit* p){
    return p->ID() == ExeOr_;
  }
};

class ExeAnd: public ExeUnit{
public:
  ExeAnd(): ExeUnit(ExeAnd_){
  }

  ~ExeAnd(){
  }

  static Error Exe(ExeUnit* thiz, Context& ctx);

  static bool classof(const ExeUnit* p){
    return p->ID() == ExeAnd_;
  }
};

class ExeLT: public ExeUnit{
public:
  ExeLT(): ExeUnit(ExeLT_){
  }

  ~ExeLT(){
  }

  static Error Exe(ExeUnit* thiz, Context& ctx);

  static bool classof(const ExeUnit* p){
    return p->ID() == ExeLT_;
  }
};

class ExeGT: public ExeUnit{
public:
  ExeGT(): ExeUnit(ExeGT_){
  }

  ~ExeGT(){
  }

  static Error Exe(ExeUnit* thiz, Context& ctx);

  static bool classof(const ExeUnit* p){
    return p->ID() == ExeGT_;
  }
};

class ExeLE: public ExeUnit{
public:
  ExeLE(): ExeUnit(ExeLE_){
  }

  ~ExeLE(){
  }

  static Error Exe(ExeUnit* thiz, Context& ctx);

  static bool classof(const ExeUnit* p){
    return p->ID() == ExeLE_;
  }
};

class ExeGE: public ExeUnit{
public:
  ExeGE(): ExeUnit(ExeGE_){
  }

  ~ExeGE(){
  }

  static Error Exe(ExeUnit* thiz, Context& ctx);

  static bool classof(const ExeUnit* p){
    return p->ID() == ExeGE_;
  }
};

class ExeEQ: public ExeUnit{
public:
  ExeEQ(): ExeUnit(ExeEQ_){
  }

  ~ExeEQ(){
  }

  static Error Exe(ExeUnit* thiz, Context& ctx);

  static bool classof(const ExeUnit* p){
    return p->ID() == ExeEQ_;
  }
};

class ExeNE: public ExeUnit{
public:
  ExeNE(): ExeUnit(ExeNE_){
  }

  ~ExeNE(){
  }

  static Error Exe(ExeUnit* thiz, Context& ctx);

  static bool classof(const ExeUnit* p){
    return p->ID() == ExeNE_;
  }
};

class ExeLike: public ExeUnit{
public:
  ExeLike(): ExeUnit(ExeLike_){
  }

  ~ExeLike(){
  }

  static Error Exe(ExeUnit* thiz, Context& ctx);

  static bool classof(const ExeUnit* p){
    return p->ID() == ExeLike_;
  }
};

class ExeFunc: public ExeUnit{
public:
  ExeFunc(): ExeUnit(ExeFunc_){
    ident = 0;
    params = 0;
  }

  ~ExeFunc(){
  }

  static Error Exe(ExeUnit* thiz, Context& ctx);

  static bool classof(const ExeUnit *p){
    return p->ID() == ExeFunc_;
  }

  const Token *ident;
  std::size_t params;
};

class ExeRecur: public ExeUnit{
public:
  ExeRecur(): ExeUnit(ExeRecur_){
    plusplus = false;
  }

  ~ExeRecur(){
  }

  static Error Exe(ExeUnit* thiz, Context& ctx);

  static bool classof(const ExeUnit *p){
    return p->ID() == ExeRecur_;
  }

  bool plusplus;
};

class ExeVal: public ExeUnit{
public:
  ExeVal(): ExeUnit(ExeVal_){
    val = 0;
  }
  
  ~ExeVal(){
  }

  static Error Exe(ExeUnit* thiz, Context& ctx);

  static bool classof(const ExeUnit* p){
    return p->ID() == ExeVal_;
  }

  const Token *val;
};

class ExeList: public ExeUnit{
public:
  ExeList(): ExeUnit(ExeList_){
  }
  
  ~ExeList(){
  }

  static Error Exe(ExeUnit* thiz, Context& ctx);

  static bool classof(const ExeUnit* p){
    return p->ID() == ExeList_;
  }

  Vector<const Token*> vals;
};

class ExeInter: public ExeUnit{
public:
  ExeInter(): ExeUnit(ExeInter_){
    l = r = 0;
  }

  ~ExeInter(){
  }

  static Error Exe(ExeUnit* thiz, Context& ctx);

  static bool classof(const ExeUnit *p){
    return p->ID() == ExeInter_;
  }

  const Token *l, *r;
};

class ExeToler: public ExeUnit{
public:
  ExeToler(): ExeUnit(ExeToler_){
    l = r = 0;
  }

  ~ExeToler(){
  }

  static Error Exe(ExeUnit* thiz, Context& ctx);

  static bool classof(const ExeUnit *p){
    return p->ID() == ExeToler_;
  }

  const Token *l, *r;
};

class ExeVar: public ExeUnit{
public:
  ExeVar(): ExeUnit(ExeVar_){
    ident = 0;
  }

  ~ExeVar(){
  }

  static Error Exe(ExeUnit* thiz, Context& ctx);

  static bool classof(const ExeUnit *p){
    return p->ID() == ExeVar_;
  }

  const Token *ident;
};

class ExeMetaVar: public ExeUnit{
public:
  ExeMetaVar(): ExeUnit(ExeMetaVar_){
    ident = 0;
  }

  ~ExeMetaVar(){
  }

  static Error Exe(ExeUnit* thiz, Context& ctx);

  static bool classof(const ExeUnit *p){
    return p->ID() == ExeMetaVar_;
  }

  const Token *ident;
};

class ExeIndex: public ExeUnit{
public:
  ExeIndex(): ExeUnit(ExeIndex_){
    intv = 0;
  }

  ~ExeIndex(){
  }

  static Error Exe(ExeUnit* thiz, Context& ctx);

  static bool classof(const ExeUnit *p){
    return p->ID() == ExeIndex_;
  }
  
  const Token *intv;
};



//---------------------------------------------
//  Executor
//---------------------------------------------
class Executor{

public:
  Executor();

  Executor(const Executor&);

  ~Executor();

  Executor& operator=(const Executor&);

  // Add a unit
  Executor& AddUnit(const ExeUnit* u);

private:
  Vector<const ExeUnit*> units;
};


} // ~pel


#endif
