
#include<cassert>
#include"rt.h"
#include"rt_mem.h"
#include"rt_mysql.h"
#include"rt_sqlite.h"
#include"rtti.h"

using namespace pel;

static void delRtSQLite(Runtime* p)
{
  RtSQLite *q = dyn_cast<RtSQLite>(p);
  assert(q);
  delete q;
}
void Runtime::Destroy()
{
  static void(*vtbl[Runtime::ClassIDSize])(Runtime*)  = 
  {
    0,
    delRtSQLite,
    0, // mem
    0, // mysql
  };

  vtbl[this->ID()](this);
}


static int AddFieldSQLite(Runtime *thiz, int model, const char* field, int type)
{
  RtSQLite* q = dyn_cast<RtSQLite>(thiz);
  assert(q);
  return q->AddField(model, field, type);
}
int Runtime::AddField(int model, const char* field, int type)
{
  static int(*vtbl[ClassIDSize])(Runtime*, int, const char*, int) = 
  {
    0,
    AddFieldSQLite,
    0,
    0,
  };
  return vtbl[ this->ID() ](this, model, field, type);
}


static int AddModelSQLite(Runtime* thiz, const char* name)
{
  RtSQLite *q = dyn_cast<RtSQLite>(thiz);
  assert(q);
  return q->AddModel(name);
}
int Runtime::AddModel(const char* name)
{
  static int(*vtbl[ClassIDSize])(Runtime*,const char*) = 
  {
    0,
    AddModelSQLite,
    0,
    0,
  };
  return vtbl[this->ID()](this, name);
}


static int AddRelSQLite(Runtime* thiz, int src, int dist, const char* rel)
{
  RtSQLite* q = dyn_cast<RtSQLite>(thiz);
  assert(q);
  return q->AddRel(src, dist, rel);
}
int Runtime::AddRel(int src, int dist, const char* rel)
{
  static int(*vtbl[ClassIDSize])(Runtime*, int,int,const char*) = 
  {
    0,
    AddRelSQLite,
    0,
    0,
  };
  return vtbl[this->ID()](this, src, dist, rel);
}


static int ConnectSQLite(Runtime* thiz, const char* addr, int flags)
{
  RtSQLite* q = dyn_cast<RtSQLite>(thiz);
  assert(q);
  return q->Connect(addr, flags);
}
int Runtime::Connect(const char* addr, int flags)
{
  static int(*vtbl[ClassIDSize])(Runtime*,const char*,int) =
  {
    0,
    ConnectSQLite,
    0,
    0,
  };

  return vtbl[this->ID()](this, addr, flags);
}


static void DisconnectSQLite(Runtime* thiz)
{
  RtSQLite* q = dyn_cast<RtSQLite>(thiz);
  assert(q);
  q->Disconnect();
}
void Runtime::Disconnect()
{
  static void (*vtbl[ClassIDSize])(Runtime*) = 
  {
    0,
    DisconnectSQLite,
    0,
    0,
  };

  vtbl[this->ID()](this);
}


static int FieldSQLite(Runtime* thiz, int model, const char* field)
{
  RtSQLite* q = dyn_cast<RtSQLite>(thiz);
  assert(q);
  return q->Field(model, field);
}
int Runtime::Field(int model, const char* field)
{
  static int (*vtbl[ClassIDSize])(Runtime*,int,const char*) =
  {
    0,
    FieldSQLite,
    0,
    0,
  };

  return vtbl[this->ID()](this, model, field);
}


static const char* GetEnvSQLite(Runtime* thiz, const char* name)
{
  RtSQLite* q = dyn_cast<RtSQLite>(thiz);
  assert(q);
  return q->Env(name);
}
const char* Runtime::Env(const char* name)
{
  static const char*(*vtbl[ClassIDSize])(Runtime*,const char*) = 
  {
    0,
    GetEnvSQLite,
    0,
    0,
  };
  return vtbl[this->ID()](this, name);
}


static int SetEnvSQLite(Runtime* thiz, const char* name, const char* val)
{
  RtSQLite* q = dyn_cast<RtSQLite>(thiz);
  assert(q);
  return q->Env(name, val);
}
int Runtime::Env(const char* name, const char* val)
{
  static int(*vtbl[ClassIDSize])(Runtime*,const char*,const char*) = 
  {
    0,
    SetEnvSQLite,
    0,
    0,
  };
  return vtbl[this->ID()](this, name, val);
}


static int ModelSQLite(Runtime* thiz, const char* name)
{
  RtSQLite* q = dyn_cast<RtSQLite>(thiz);
  assert(q);
  return q->Model(name);
}
int Runtime::Model(const char* name)
{
  static int(*vtbl[ClassIDSize])(Runtime*,const char*) = 
  {
    0,
    ModelSQLite,
    0,
    0,
  };
  return vtbl[this->ID()](this, name);
}


static int RelSQLite(Runtime* thiz, int model, const char* rel)
{
  RtSQLite* q = dyn_cast<RtSQLite>(thiz);
  assert(q);
  return q->Rel(model, rel);
}
int Runtime::Rel(int model, const char* rel)
{
  static int (*vtbl[ClassIDSize])(Runtime*,int,const char*) =
  {
    0,
    RelSQLite,
    0,
    0,
  };
  return vtbl[this->ID()](this, model, rel);
}

