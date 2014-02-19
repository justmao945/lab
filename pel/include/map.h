/*
 * File: map.h
 * Provides a simplified hash table.
 */

#ifndef PEL_MAP_H
#define PEL_MAP_H

#include<cstdio>
#include<cassert>
#include<cstddef>
#include<cstring>
#include<cmath>
#include"new.h"

namespace pel{

//------------------------------------
//  Hash functions
//------------------------------------

// Murmur2 hash function: https://sites.google.com/site/murmurhash
unsigned long murmur2(const void* key, std::size_t len);

template<class T>
struct Hash{
};

template<> struct Hash<char>{
  unsigned long operator()(char val)const{ return val; }
};

template<> struct Hash<unsigned char>{
  unsigned long operator()(unsigned char val)const{ return val; }
};

template<> struct Hash<signed char>{
  unsigned long operator()(signed char val)const{ return val; }
};

template<> struct Hash<short>{
  unsigned long operator()(short val)const{ return val; }
};

template<> struct Hash<unsigned short>{
  unsigned long operator()(unsigned short val)const{ return val; }
};

template<> struct Hash<int>{
  unsigned long operator()(int val)const{ return val; }
};

template<> struct Hash<unsigned int>{
  unsigned long operator()(unsigned int val)const{ return val; }
};

template<> struct Hash<long>{
  unsigned long operator()(long val)const{ return val; }
};

template<> struct Hash<unsigned long>{
  unsigned long operator()(unsigned long val)const{ return val; }
};

template<>
struct Hash<char*>{
  unsigned long operator()(const char* val) const{
    return murmur2(val, std::strlen(val));
  }
};

template<>
struct Hash<const char*>{
  unsigned long operator()(const char* val) const{
    return murmur2(val, std::strlen(val));
  }
};


//---------------------------------------
// Equal funciton
//---------------------------------------
template<class T>
struct Equal{
  bool operator()(const T& a, const T& b) const{
    return a == b;
  }
};

// ------------------------------------
// =,=
// ------------------------------------
template<>
struct Equal<const char*>{
  bool operator()(const char* a, const char* b) const{
    return std::strcmp(a, b) == 0;
  }
};

template<>
struct Equal<char*>{
  bool operator()(const char* a, const char* b) const{
    return std::strcmp(a, b) == 0;
  }
};


// Using binary search to round up to a prime number.
unsigned long r2prime(unsigned long x);


//-------------------------------------------------------------
//  Map.
//  Attention: when using 'const char*' as the key type,
//  must define a new Equal struct:
//      struct EqualCString{
//        bool operator()(const char* a, const char* b) const{
//          return std::strcmp(a, b) == 0;
//        }
//      };
//      Map<const char*, int, Hash<const char*>, EqualCString> m;
//
//  or create a template trait: !!! Now default support...(x.x)
//      template<> struct Equal<const char*>{
//        bool operator()(const char* a, const char* b) const{
//          return std::strcmp(a, b) == 0;
//        }
//      }
//
//  finally, must keep all C style keys alive during map operation.
//
//  Map require KT implements operator==() or rewrite a new Equal class.
//  Map only implements simple hash functions for internal types.
//-------------------------------------------------------------
template<class KT, class VT,
         class Hash  = Hash <KT>,
         class Equal = Equal<KT> >
class Map{
public:
  //-----------------------------------------
  // Box to hold key and value for map.
  //-----------------------------------------
  struct Box{
    Box(){
      next = prev = 0;
    }
    
    Box(const Box& b){
      next = b.next;
      prev = b.prev;
      key = b.key;
      val = b.val;
    }

    Box(const KT& key, const VT& val){
      this->key = key;
      this->val = val;
      next = 0;
      prev = 0;
    }
    
    ~Box(){
    
    }
    
    Box& operator=(const Box& b){
      if( this != &b ){
        next = b.next;
        prev = b.prev;
        key = b.key;
        val = b.val;
      }
    }
    
    // key can't be changed.
    const KT& Key() const{
      return key;
    }
    
    VT& Value(){
      return val;
    }
    
    const VT& Value() const{
      return val;
    }
    
    Box& Value(const VT& val){
      this->val = val;
    }
    
  private:
    friend class Map;
    
    Box *next; // every node are linked as a list
    Box *prev;
    KT key;
    VT val;
  };

public:
  Map(){
    bkt = 0;
    bktsize = 0;
    size = 0;
    mlf = 1.0f;
  }

  Map(const Map& m){
    bkt = 0;
    bktsize = 0;
    size = 0;
    mlf = 1.0f;
    
    this->Rehash(m.bktsize);
    this->cpbkt(this->bkt, this->bktsize, m.bkt, m.bktsize);
    this->size = m.size;
    this->mlf = m.mlf;
    this->hash = m.hash;
    this->eq = m.eq;
  }

  ~Map(){
    this->Clean();
    delete[] this->bkt;
  }

  Map& operator=(const Map& m){
    if(&m != this){
      this->Clean();
      if( this->bktsize < m.bktsize ){
        this->Rehash(m.bktsize);
      }
      this->cpbkt(this->bkt, this->bktsize, m.bkt, m.bktsize);
      this->size = m.size;
      this->mlf = m.mlf;
      this->hash = m.hash;
      this->eq = m.eq;
    }
    return *this;
  }
  
  // Add <key, val> to map.
  // @return 0 if add successfully, or a box hold existing key and value.
  Box* Add(const KT& key, const VT& val){
    this->Reserve(++this->size);
    return this->ckadd2bkt(this->bkt, this->bktsize, key, val);
  }
  
  std::size_t BucketCount()const{
    return this->bktsize;
  }

  Map& Clean(){
    Box *head = 0, *next = 0;
    for(std::size_t i=0; i<bktsize; ++i){
      head = bkt[i];
      while(head){
        next = head->next;
        delete head;
        head = next;
      }
      bkt[i] = 0;
    }
    this->size = 0;
    return *this;
  }

  //
  //  class Binary{
  //  public:
  //    void operator()(const KeyType& key, ValueType& value){
  //      ....
  //    }
  //  };
  template<class Binary>
  void Each(Binary& op){
    Box *head = 0;
    for(std::size_t i=0; i<bktsize; ++i){
      head = bkt[i];
      while(head){
        op(head->Key(), head->Value());
        head = head->next;
      }
    }
  }

  bool Empty()const{
    return this->size == 0;
  }

  Map& Erase(Box* box){
    if(box){
      if( box->prev == 0 ){ // the first one
        // may not be very efficient
        Box **ph = &bkt[ hash(box->key) % bktsize ];
        assert(*ph == box);
        *ph = box->next; // reset the bucket ptr.
      }else{
        box->prev->next = box->next;
      }
      if(box->next){
        box->next->prev = box->prev;
      }
      delete box;
      this->size --;
    }
    return *this;
  }

  Map& Erase(const KT& key){
    return Erase( Find(key) );
  }

  // 
  // @return 0 if not found, or a map box.
  Box *Find(const KT& key) const{
    Box *head = bkt[ hash(key) % bktsize ];
    while( head && ! eq(head->key, key) ){
      head = head->next;
    }
    return head;
  }

  float LoadFactor()const{
    return bktsize == 0 ? 0.f : 1.f * size / bktsize;
  }

  float MaxLoadFactor()const{
    return this->mlf;
  }

  Map& MaxLoadFactor(float mlf){
    this->mlf = mlf;
    return *this;
  }

  Map& Rehash( std::size_t nbuckets )
  {
    std::size_t nbktsize = r2prime(nbuckets);
    
    // require difference size, reserve space or shrink size.
    if( nbktsize != this->bktsize ){
      Box** nbkt = new Box*[nbktsize];
      std::memset(nbkt, 0, nbktsize * sizeof(Box*)); // ! clear buckets
      this->cpbkt(nbkt, nbktsize, this->bkt, this->bktsize);
      delete[] this->bkt;
      this->bkt = nbkt;
      this->bktsize = nbktsize;
    }
    return *this;
  }

  // Reserve bigger space or shrink size.
  Map& Reserve(std::size_t n){
    return Rehash(std::ceil(n / this->mlf));
  }

  std::size_t Size() const{
    return this->size;
  }
  

private:
  Box **bkt; // underlying bucket
  std::size_t bktsize; // bucket size
  std::size_t size; // record all key-val size
  float mlf; // max load factor
  Hash hash;  // hash function
  Equal eq; // equal function

private:

  // deep copy bucket src to dist bucket.
  void cpbkt(Box* dist[], std::size_t dsize, Box* src[], std::size_t ssize){
    if( ssize == 0 ){
      return;
    }
    
    assert(dist);
    assert(src);
    assert(dsize > 0);
    
    Box *head = 0;
    for(std::size_t i=0; i<ssize; ++i){
      head = src[i];
      while( head ){
        this->add2bkt(dist, dsize, head);
        head = head->next;
      }
    }
  }
  
  // add a box to bucket.
  void add2bkt(Box* bkt[], std::size_t bktsize, Box* box){
    assert(bkt);
    if( box == 0 ){
      return;
    }
    
    Box **ph = &bkt[ this->hash(box->key) % bktsize ], *head = *ph;
    
    if( head ){ // bucket[..] is not empty, and insert box before head
      box->next = head;
      head->prev = box;
    }
    // !!! Important: box->prev is 0, indicates the first one in the list.
    
    *ph = box;
  }

  // check and add a box to bucket.
  // If key is duplicated, then return the duplicated box otherwise 0.
  Box* ckadd2bkt(Box* bkt[], std::size_t bktsize, const KT& key, const VT& val){
    assert(bkt);
    
    Box **ph = &bkt[ this->hash(key) % bktsize ], *head = *ph;
    
    // check duplicated key
    while(head){
      if( this->eq(key, head->key) ){
        return head;
      }
      head = head->next;
    }
    
    Box *box = new Box(key, val);
    if( head ){ // bucket[..] is not empty, and insert box before head
      box->next = head;
      head->prev = box;
    }
    // !!! Important: box->prev is 0
    
    *ph = box;
    return 0;
  }
};


} // ~pel


#endif

