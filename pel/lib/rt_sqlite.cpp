
#include<cassert>
#include<sqlite3.h> // require SQLite3
#include"rt_sqlite.h"


using namespace pel;

int RtSQLite::AddField(int model, const char* field, int type)
{
  return -1;
}

int RtSQLite::AddModel(const char* name)
{
  return -1;
}

int RtSQLite::AddRel(int src, int dist, const char* rel)
{
  return -1;
}

int RtSQLite::Connect(const char* addr, int flags)
{
  return -1;
}

void RtSQLite::Disconnect()
{
  return;
}

int RtSQLite::Field(int model, const char* field)
{
  return -1;
}

const char* RtSQLite::Env(const char* name)
{
  return 0;
}

int RtSQLite::Env(const char* name, const char* val)
{
  return -1;
}

int RtSQLite::Model(const char* name)
{
  return -1;
}

int RtSQLite::Rel(int model, const char* rel)
{
  return -1;
}



