/*
 * File: rt.h
 * Provides common runtime interface for runner.
 */


#ifndef PEL_RT_H
#define PEL_RT_H

namespace pel{


//
//  Base class of runtime
//
class Runtime{
public:
  enum ClassID{
    Runtime_ = 0,
    RtSQLite_,
    RtMemory_,
    RtMySQL_,
    ClassIDSize,
  };

  ClassID ID()const { return id; }
  
  static bool classof(const Runtime* p){
    return p->ID() == Runtime_;
  }

  //!!! the only way to destory pointers.
  void Destroy();

  //------------------------------------------
  // Interfaces showed as below provided by runtime.
  
  // return field id
  int AddField(int model, const char* field, int type);

  // return model id
  int AddModel(const char* name);

  // return relation id
  int AddRel(int src, int dist, const char* rel);

  // Connect to runtime
  int Connect(const char* addr, int flags);

  //
  void Disconnect();

  // get field id
  int Field(int model, const char* field);

  // Get env var
  const char* Env(const char* name);

  // Set env var
  int Env(const char* name, const char* val);

  // Get model id
  int Model(const char* name);

  // return relation id
  int Rel(int model, const char* rel);



protected:
  Runtime(): id(Runtime_){}

  explicit Runtime(ClassID sid): id(sid){}

  ~Runtime(){} // empty

private:
  ClassID id;

private:
  Runtime(const Runtime&);
  Runtime& operator=(const Runtime&);
};



} // ~pel


#endif
