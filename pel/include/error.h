
#ifndef PEL_ERROR_H
#define PEL_ERROR_H

#ifdef __cplusplus

namespace pel{

class Error{
public:
  static const int OK = 0;
  static const int ERROR = 1;
  static const int WARNING = 2;

  Error();

  explicit Error(const char* msg, bool move=false);

  explicit Error(int err);

  Error(int err, const char* msg, bool move=false);

  Error(const Error& e);

  ~Error();

  Error& operator=(const Error& e);

  // Return true if error code is not OK, or false.
  operator bool(){
    return err != OK;
  }
  
  int Code()const{
    return err;
  }

  const char* What() const{
    return msg;
  }

private:
  int err;
  char *msg;
  int *used; // reference counter

  void cinit(int err, const char* msg, bool move);
  void destroy();
};

} // ~pel

#endif // ~__cplusplus

//....




#endif // ~PEL_ERROR_H
