/*
 * File: rt_sqlite.h
 * Provides SQLite3 interface.
 */

#ifndef PEL_RT_SQLITE_H
#define PEL_RT_SQLITE_H

#include"rt.h"

namespace pel {

//---------------------------------------
// SQLite implementation of runtime.
//---------------------------------------
class RtSQLite: public Runtime{
public:
  RtSQLite(): Runtime(RtSQLite_){
  }

  ~RtSQLite(){
  }

  static bool classof(const Runtime* p){
    return p->ID() == RtSQLite_;
  }

  //---------------  implements interface --------

  // return field id
  // Model fields table: [ModelID]_fields
  // | field id [int] | field name (string) | field type (int) |
  //
  int AddField(int model, const char* field, int type);

  // return model id
  // Model stastics table: models
  // | model id [int] | model name (string) |
  int AddModel(const char* name);

  // return relation id
  // Relationship table: [srcID]_rels
  // | relID [int] | rel (string) | distID (int) |
  int AddRel(int src, int dist, const char* rel);

  // Connect to runtime
  int Connect(const char* addr, int flags);

  //
  void Disconnect();

  // get field id
  //
  int Field(int model, const char* field);

  // Get env var
  const char* Env(const char* name);

  // Set env var
  // Env table: env
  // | name [string] | value (string) |
  int Env(const char* name, const char* val);

  // Get model id
  // 
  int Model(const char* name);

  // return relation id
  // 
  int Rel(int model, const char* rel);


private:


private:
  RtSQLite(const RtSQLite&);
  RtSQLite& operator=(const RtSQLite&);
};


} // ~pel

#endif
