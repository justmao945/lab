
/*
 * File: vector.h
 * Provides a simplified auto-increase array.
 */

#ifndef PEL_VECTOR_H
#define PEL_VECTOR_H

#include<cassert>
#include<cstddef>
#include<cstdlib>
#include<cstring>
#include"new.h"

namespace pel{


// round to power of 2
std::size_t r2pow(std::size_t x);

/*
 * Class T needs to implement copy constructor, destructor and operator=.
 *
 */
template<class T>
class Vector{
public:
  explicit Vector(std::size_t capacity=1) {
    this->capacity = capacity;
    this->size = 0;
    this->data = this->alloc(this->capacity, 0, 0);
  }

  Vector(const Vector& vec){
    this->size = vec.size;
    this->capacity = r2pow(this->size);
    this->data = this->alloc(this->capacity, vec.data, vec.size);
  }

  ~Vector(){
    this->dealloc(this->data, this->size);
  }

  Vector& operator=(const Vector& vec){
    if( this != &vec ){
      this->dealloc(this->data, this->size);
      this->size = vec.size;
      this->capacity = r2pow(this->size);
      this->data = this->alloc(this->capacity, vec.data, vec.size);
    }
    return *this;
  }

  T& operator[](std::size_t index){
    assert(index < this->size);
    return this->data[index];
  }
  
  const T& operator[](std::size_t index) const{
    assert(index < this->size);
    return this->data[index];
  }

  T& Back(std::size_t n=0){
    assert(this->size >= 1+n);
    return this->data[this->size - (1+n)];
  }

  const T& Back(std::size_t n=0) const {
    assert(this->size >= 1+n);
    return this->data[this->size - (1+n)];
  }

  std::size_t Capacity() const{ return this->capacity; }

  Vector& Clean(){
    for(std::size_t i = 0; i < this->size; ++i){
      this->data[i].~T();
    }
    this->size = 0;
    return *this;
  }

  T* Data(){ return this->data; }

  const T* Data() const { return this->data; }

  bool Empty()const { return !this->size; }

  T& Front(std::size_t n=0){
    assert(this->size >= 1+n);
    return this->data[n];
  }

  const T& Front(std::size_t n=0) const{
    assert(this->size >= 1+n);
    return this->data[n];
  }

  // move data from v to this.
  // v will be cleaned after moving.
  Vector& Move(Vector& v){
    
    // first clean, and swap underlying storage.
    this->Clean();
    T *tdat = this->data;
    std::size_t tcap = this->capacity;

    this->data = v.data;
    this->capacity = v.capacity;
    this->size = v.size;
    
    v.data = tdat;
    v.capacity = tcap;  // exchanged
    v.size = 0;
    return *this;
  }

  Vector& Push(const T& val){
    if( this->size >= this->capacity ){
      this->Reserve(this->size << 1);
    }
    new(& this->data[this->size ++]) T(val);
    return *this;
  }

  template<std::size_t N>
  Vector& Push(const T vals[N]){
    return this->Push(vals, N);
  }

  Vector& Push(const T vals[], std::size_t N){
    std::size_t ns = this->size + N;
    if( ns > this->capacity ){
      ns = r2pow(ns);
      this->Reserve( ns );
    }

    for( std::size_t i=0; i<N; ++i ){
      new(& this->data[this->size + i]) T(vals[i]);
    }
    this->size += N;
    return *this;
  }

  Vector& Pop(std::size_t n=1){
    assert(this->size >= n);
    while(n--){
      this->data[-- this->size].~T();
    }
    return *this;
  }

  Vector& Reserve(std::size_t capacity){
    // alloc bigger size or shrink
    if( capacity > this->size && capacity != this->capacity ){
      this->capacity = capacity;
      T* data = this->alloc(this->capacity, this->data, this->size);
      this->dealloc(this->data, this->size);
      this->data = data;
    }
    // else nothing
    return *this;
  }

  std::size_t Size() const { return this->size; }

  T& Top(){
    assert(this->size >= 1);
    return this->data[this->size - 1];
  }

  const T& Top() const{
    assert(this->size >= 1);
    return this->data[this->size - 1];
  }

private:
  T* data;
  std::size_t capacity;
  std::size_t size;

private:
  // placement allocation
  T* alloc(std::size_t capacity, T* data, std::size_t size){
    if(capacity < size){
      capacity = size;
    }
    T* p = static_cast<T*>(std::malloc(capacity * sizeof(T)));
    
#ifndef NDEBUG // debug mode, init data with 0.
    std::memset(p, 0, capacity * sizeof(T));
#endif
    
    assert(p);
    for(std::size_t i = 0; i < size; ++i){
      new(&p[i]) T(data[i]);
    }
    return p;
  }

  void dealloc(T* data, size_t size){
    for(std::size_t i = 0; i < size; ++i){
      data[i].~T();
    }
    std::free(data);
  }

};

} // ~pel


#endif
