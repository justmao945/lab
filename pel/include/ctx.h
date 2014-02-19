/*
 * File: ctx.h
 * Provides context for execution units.
 */


#ifndef PEL_CTX_H
#define PEL_CTX_H

#include<cassert>
#include<cstddef>
#include"error.h"
#include"map.h"
#include"vector.h"

namespace pel{

class Runtime; // rt.h  

//--------------------------------------------
//  Stack value class
//--------------------------------------------
class StkVal{
public:
  enum TypeRep{
    Null = 0,
    Int,
    Float,
    String,
  };

  union Value{
    int intv;
    double floatv;
    const char* cstring;
    char* string;
  };

  StkVal(std::size_t rows, std::size_t columns);

  ~StkVal();
  
  std::size_t Cols() const{
    return cols;
  }

  StkVal& CurID(int id){
    curid = id;
    return *this;
  }

  int CurID()const{
    return curid;
  }

  Value& Data(std::size_t row, std::size_t col){
    assert(row < rows);
    assert(col < cols);
    return data[row][col];
  }

  const Value& Data(std::size_t row, std::size_t col) const{
    assert(row < rows);
    assert(col < cols);
    return data[row][col];
  }

  std::size_t Rows() const{
    return rows;
  }

  TypeRep& Type(std::size_t col){
    assert(col < cols);
    return types[col];
  }

  const TypeRep& Type(std::size_t col) const {
    assert(col < cols);
    return types[col];
  }

private:
  int curid; // current model/field/relation id
  std::size_t rows;
  std::size_t cols;
  Value** data;  // data[row][column]  -> Value
  TypeRep* types;  // types[column] -> Type

private:
  StkVal(const StkVal&);
  StkVal& operator=(const StkVal&);
};


//---------------------------------------------------
//  Executor retrives runtime by Context.
//---------------------------------------------------
class Context{
public:

  Context(Runtime* rt);

  ~Context();

  // Clean all things excepts runtime.
  Context& Clean();

  int CurID() const;

  // !!! Load context from stored data through runtime.
  // This function should be call before using context.
  // This will call Clean() before loading.
  Error Load();

  // Get declared path.
  StkVal* Path(const char* ident);

  // Add path declaration
  // @ident
  // @val
  // @return 0 if successfully, or the duplication.
  StkVal* Path(const char* ident, StkVal* val);

  // Pop n values from stack, default pop 1.
  Context& Pop(std::size_t n=1);

  // Push val to stack
  Context& Push(StkVal *val);

  // Get runtime
  Runtime *Rt();

  // Get top n value, 0 means the top one, 1 is the one
  // below the top one. Default is the top one.
  StkVal* Top(std::size_t n=0);

private:
  Vector<StkVal*> stk; // stack for execution units
  Map<const char*, StkVal*> paths; // Path declarations
  
  Runtime *rt; // runtime
  int rootmod; // Root model

private:
  // disable copy
  Context(const Context& ctx);
  Context& operator=(const Context& ctx);
};


}// ~pel


#endif
