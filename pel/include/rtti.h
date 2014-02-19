/*
 * File: rtti.h
 * Provides simple RTTI implemention to replace the std c++ one.
 * See examples in rtti.cpp,
 * and more details on http://llvm.org/doxygen/Casting_8h_source.html
 */

#ifndef PEL_RTTI_H
#define PEL_RTTI_H

//#define PEL_TRACE_RTTI

#ifdef PEL_TRACE_RTTI
  #include <cstdio>
  #define PEL_TRACE_RTTI_FUNC \
    std::printf("%s:%d:%s\n",__FILE__,__LINE__,__FUNCTION__)
#else
  #define PEL_TRACE_RTTI_FUNC
#endif


namespace pel{

// 1. Object
template<class To, class From> struct isa_impl{
  static bool doit(const From &val){
    PEL_TRACE_RTTI_FUNC;
    return To::classof(&val);
  }
};

// 2. Pointer
template<class To, class From> struct isa_impl<To, From*>{
  static bool doit(const From *val){
    PEL_TRACE_RTTI_FUNC;
    return isa_impl<To, From>::doit(*val);
  }
};

// 3. Point to const
template<class To, class From> struct isa_impl<To, From const*>{
  static bool doit(const From *val){
    PEL_TRACE_RTTI_FUNC;
    return isa_impl<To, From>::doit(*val);
  }
};

/*
// These traits are not necessary.

// 1.5. Const object
template<class To, class From> struct isa_impl<To, const From>{
  static bool doit(const From &val){
    PEL_TRACE_RTTI_FUNC;
    return isa_impl<To, From>::doit(val);
  }
};

// 4. Const pointer
template<class To, class From> struct isa_impl<To, From *const>{
  static bool doit(const From* const val){
    PEL_TRACE_RTTI_FUNC;
    return isa_impl<To, From>::doit(*val);
  }
};

// 5. Const pointer point to const
template<class To, class From> struct isa_impl<To, From const*const>{
  static bool doit(const From* const val){
    PEL_TRACE_RTTI_FUNC;
    return isa_impl<To, From>::doit(*val);
  }
};
*/

// isa
template <typename To, typename From>
inline bool isa(const From& val) {
  PEL_TRACE_RTTI_FUNC;
  return isa_impl<To, From>::doit(val);
}

template <typename To, typename From>
inline To* dyn_cast(From& val)
{
  PEL_TRACE_RTTI_FUNC;
  return isa<To>(val) ? static_cast<To*>(val) : 0;
}

}


#endif