#include <stdio.h>
#include <yacl/table.h>

typedef struct {
  cstring_t key;
  cstring_t value;
}kv_t;

typedef def_object_z(kv_t, void) kv_z;

kv_t* kv_initcp(kv_t *self, const kv_t* other)
{
  CString._initcp_(&self->key, &other->key);
  CString._initcp_(&self->value, &other->value);
  return self;
}

size_t kv_hash(const kv_t *self)
{
  return CString._hash_(&self->key);
}

const kv_z KV = 
{
  ._sizeof_ = sizeof(kv_t),
  ._init_ = NULL,
  ._initcp_ = kv_initcp,
  ._del_ = NULL,
};

void pr(table_ref_t ref)
{
  printf("\"%s\" @ %p\n", *(cstring_t*)ref->value, ref);
}


void pr_kv(table_ref_t ref)
{
  printf("%s: %s\n", ((kv_t*)ref->value)->key, ((kv_t*)ref->value)->value);
}

int main()
{
  cstring_t keys[] = {"hello", "world", "haha", "table"};
  table_t table;

  kv_t kvs[] = {{"hello", "world"}, {"haha", "ffff"}, {"heiei", "haha"}};

  make(Table, &table, (objcmp_z*)&CString, NULL, NULL);
  Table.insert_n(&table, sizeof(keys)/sizeof(cstring_t), keys);
  pr(Table.find(&table, &keys[0]));
  pr(Table.find(&table, &keys[1]));
  pr(Table.find(&table, &keys[2]));
  pr(Table.find(&table, &keys[3]));
  clean(Table, &table);

  kv_t kv;
  make(Table, &table, (objcmp_z*)&KV, (hash_f*)CString._hash_, (cmp_f*)CString._eq_);
  Table.insert_n(&table, sizeof(kvs)/sizeof(kv_t), kvs);
  
  kv.key = "heiei";
  pr_kv(Table.find(&table, &kv));
  kv.key = "haha";
  pr_kv(Table.find(&table, &kv));
  kv.key = "hello";
  pr_kv(Table.find(&table, &kv));
  clean(Table, &table);
  return 0;
}


