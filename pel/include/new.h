/*
 * File: new.h
 * Provides no excetion new/delete
 */

#ifndef PEL_NEW_H
#define PEL_NEW_H

#include<cstddef>

//namespace pel{ // in pel, avoid conflicting with std

void* operator new(std::size_t size);
void  operator delete(void* ptr);

void* operator new[](std::size_t size);
void  operator delete[](void* ptr);

inline void* operator new  (std::size_t size, void* ptr){return ptr;}
inline void* operator new[](std::size_t size, void* ptr){return ptr;}
inline void  operator delete  (void* ptr, void*){}
inline void  operator delete[](void* ptr, void*){}

//}

#endif