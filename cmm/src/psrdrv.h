/////////////////////////////////////////////////////////////////////
///  @file psrdrv.h provides definition of parser tree, tokens
///  , and symbol table.
///
///  ATTENTIONS:
///    1. This driver is *NOT* thread safe.
///    2. Operations in Token class require right recursive YACC grammar
///       e.g. 
///           foo: foo bar { ... }; will cause ptr overriding.
///           foo: bar foo { ... }; is the *right* way.
///
/////////////////////////////////////////////////////////////////////


#ifndef PSRDRV_H_
#define PSRDRV_H_

#include <string>
#include <vector>
#include <map>
#include <cassert>

//#define DEBUG_PSR
#ifndef DEBUG_PSR
  #define d_printf  psr::_dummy
#else
  #define d_printf  printf
#endif

namespace psr{

class Tree;
class Token;
class  Sym;
template<class T>class Pool;
template<class T>class PoolImpl;


/////////////////////////////////////////////////////////////////////////
/// Define a class that provides a 2D memory pool, which manages memory
/// automatically. Allocated memory in the pool should *NOT* be released 
/// until destory the pool. And it is impossible to change the capacity 
/// in X dimension after creation. But the pool should increase 
/// automatically in Y dimension.
////////////////////////////////////////////////////////////////////////
template <class T> class Pool{
  std::vector< T* > _yDim;
  T* _xDim;
  size_t _size, _xDimSize;
  size_t _cap, _xDimCap;

  void _yInc(){
    if(_size == _cap){
      _cap += _xDimCap;
      _xDimSize = 0;
      _xDim = new(std::nothrow) T[_xDimCap];
      assert(_xDim != NULL);
      _yDim.push_back(_xDim);
    }
  }

public:

  /// Default constructor.
  /// @param capacity X dimension always use this capacity, 
  ///     default capacity is 1K.
  Pool(size_t capacity=1024){
    if(capacity == 0)
      capacity = 1024;

    _xDim = new T[capacity];
    _yDim.push_back(_xDim);
    _size = _xDimSize = 0;
    _cap = _xDimCap = capacity;
  }
  
  ~Pool(){
    for(size_t i=0; i<_yDim.size(); i++){
      delete[] _yDim[i];
    }
  }
  
  /// Get an new object from pool.
  /// @return New object from pool.
  T* one(){
    _yInc();
    if(_cap <= _size)
      return NULL;
    else{
      _size++;
      return _xDim + _xDimSize++;
    }
  }

  /// Clear the pool, but not release any memory.
  /// @return
  void clear(){
    _xDimSize = _size = 0;
    _xDim = _yDim[0];
  }
  
  /// Get total size of pool.
  /// @return Size of the pool
  size_t size()const{
    return _size;
  }
  
  /// Get an element from pool.
  /// @param x X dimension index.
  /// @param y Y dimension index.
  /// @return Ptr to index [x][y], or NULL if not exist.
  T* operator()(size_t x, size_t y){
    if(y >= _yDim.size() || x >= _xDimCap
        || (y == _yDim.size()-1 && x >= _xDimSize))
      return NULL;
    else
      return _yDim[y][x];
  }
};



///////////////////////////////////////////////////////////////////
/// Pool implementation base class, which providing a class member pool 
/// to manage all objects in the same parser tree.
/// Be careful to *bind* the pool ptr before call the poolOne().
//////////////////////////////////////////////////////////////////
template<class T> class PoolImpl{
public:
  PoolImpl(){}
  virtual ~PoolImpl(){}

  static Pool<T> *pool;

   /// Get an new instance from the pool.
   /// @return NULL if failed or new instance from pool,
   ///         does *NOT* need to free manually.
  static T* poolOne(){
    if(pool){
      return pool->one();
    }else
      return NULL;
  }

  /// Clear the pool.
  /// @return
  static void poolClear(){
    if(pool)
      pool->clear();
  }
};
template<class T> Pool<T>* PoolImpl<T>::pool = NULL;




////////////////////////////////////////////////////////////////////////
/// Define a class to represents tokens in parser tree. A token has a 
/// child ptr and a sibling ptr to find children and siblings.
/// Parser tree consists of tokens, and some attributes are linked in
/// a double linked list of node, using "l" and "r" pointer.
////////////////////////////////////////////////////////////////////////
class Token : public PoolImpl<Token>{
public:
  // Default constructor
  Token();

  /// Constructor.
  /// @param yytext: Token yytext, a copy of YACC yytext.
  /// @param token: Token enumerator, see YACC.y::%token.
  /// @param filter: Token filter,Token::Filter.
  /// @see psr::Token::Filter
  Token(const std::string& yytext, int token, int filter=Filter::NA);

  // Destructor
  ~Token();

  Token* l;    // prefix decorator
  Token* r;    // followed decorator

  int token;    // token enumerator, see YACC.y::%token
  int lineno;   // If is a termial, then has line number 
  std::string yytext;   // dup from yytext
  Token *child;     // ptr to child
  Token *sibling;   // ptr to sibling (  right hand )
  Token *parent;    // ptr to parent node.
  Token *lsibling;  // ptr to left sibling.
  Sym* sym; // symbol table
  int filter; // help to filter the token.


  /// Enumerators to filter tokens faster.
  struct Filter{
    enum {
      NA = 0,      // Not available.
      //STRUCT_STM,  // Structured (keyword Foo {}) statement. XXX
      //CONST_STM,   // Constant assign statement. XXX
      //PTR_STM,     // Pointer statement. XXX
      SELE_STM,    // selection statement. ( generally if-else )
      ITER_STM,    // iteration statement. ( while )
      RET_STM,     // return statement. ( return )
      OP_STM,      // token contain ops: =, <, >, !=, ....

      EX_DECL_STM = 1 << 8,    // Declaration statement.
      EX_FUNC_STM = 1 << 9,    // Function(operation?) statement.
      EX_ARRAY_STM = 1 << 10,  // has array size attribute.
    };
  };


  ///  Get the last sibling node. If has no sibling, then return self.
  ///  @return The last sibling.
  Token*  lastSibling();

   /// Get the most left decorator token.
   /// If has no left decorator, then return self.
   /// @return Left most decorator.
  Token* leftmost();

  /// Get the most right decorator token.
  /// If has no right decorator, then return self.
  /// @return Right most decorator.
  Token* rightmost();
  
  /// Get n-th element in .l->(n<0) or .r->(n>0) list.
  /// @param n Index of the element, 0 to get self.
  /// @return N-th element or NULL of not exist.
  Token* lrNth(int n);

  /// Get n-th element in .l-> list.
  /// @param n Index of the element, 0 to get self.
  /// @return N-th element or NULL of not exist.
  Token* leftNth(int n);

  /// Get n-th element in .r-> list.
  /// @param n Index of the element, 0 to get self.
  /// @return N-th element or NULL of not exist.
  Token* rightNth(int n);

  /// Copy from another Token. (Not deep copy.)
  ///  @param tok Another token
  ///  @return *this 
  Token& copy(const Token& tok);

  /// Copy from another Token. (Not deep copy.)
  /// @param token Another token
  /// @return *this 
  Token& operator=(const Token& token);

  /// Clear the token, zero every thing.
  /// @return this
  Token*  clear();

  /// Only clear pointers, but not other data of token.
  /// @return this
  Token* clrptr();

  /// Duplicated from pool, but not copy ptrs, only data.
  /// @return Duplicated instance.
  Token*  dup()const;

  /// Set this token as the leftmost decorator of tok.
  /// @param tok Token to set.
  /// @return tok.
  Token&  operator<(Token& tok);

  /// Set this token as the leftmost decorator of ptok.
  /// @param ptok Token to set.
  /// @return Token(ptok) or *this if ptok is null.
  Token&  operator<(Token* ptok);

  /// Set token tok as the rightmost decorator of this token.
  /// @param tok Token to set.
  /// @return Token(tok).
  Token&  operator>(Token& tok);

  /// Set token ptok as the rightmost decorator of this token.
  /// @param ptok Token to set.
  /// @return Token(ptok) or *this if ptok is null.
  Token&  operator>(Token* ptok);

  /// Set an token tok as the child of this token.
  /// @param tok Token to set as child.
  /// @return Child token tok.
  Token& operator<<(Token& tok);

  /// Set an token ptok as the child of this token.
  /// @param ptok Pointer of token to set as child.
  /// @return Child token Token(ptok) or *this if ptok is null.
  Token& operator<<(Token* ptok);

  /// Set token tok as the last sibling of this token.
  /// @param tok Token to set as the last sibling.
  /// @return Sibling token tok.
  Token& operator>>(Token& tok);

  /// Set token ptok as the last sibling of this token.
  /// @param ptok Token to set as the last sibling.
  /// @return Sibling token Token(ptok) or *this if ptok is null.
  Token& operator>>(Token* ptok);


  /// Generating a symbol table. This function will add all *DECL_STM*
  /// siblings and update all related symbol table pointers.
  /// TODO: ignore same declarations but not definitions.
  /// @param tab Another symbol table to generate on, or NULL to use 
  ///         an new symbol table.
  /// @return this.
  Token* genSym(Sym* tab=NULL);


private:

  // Set all siblings outer to @param outer.
  // @param outer Outer ptr to set.
  void _setSiblingSymOuter(Sym* outer);
};


////////////////////////////////////////////////////////////////////////
/// Define a class that provides a simple symbol table, based on
/// std::\<map\>. A symbol table only contains symbols in its scope, and
/// using outer ptr to achive symbols in the outer scope.
/// FIXME: Using hash map for better performence?
////////////////////////////////////////////////////////////////////////
class Sym : public PoolImpl<Sym>{
public:
  Sym* outer; // outer level symbol table

  Sym();
  ~Sym();

  /// Get symbol from current scope in Token representation.
  /// @param sym Symbol represented in C string.
  /// @return NULL if not found or the Token in current scope.
  Token* get(const char* sym);

  /// Get symbol from current scope in Token representation.
  /// @param sym Symbol represented in string.
  /// @return NULL if not found or the Token in current scope.
  Token* get(const std::string& sym);

  /// Add an new symbol token into the table
  /// @param sym  Symbol to be added into table.
  /// @return NULL if add **successfully**, or the predefined token.
  Token* add(Token* sym);

  /// Add a new symbol token into the table
  /// @param sym  Symbol to be added into table.
  /// @return NULL if add **successfully**, or the predefined token.
  Token* add(Token& sym);

  /// Clear symbol map table and set outer ptr to NULL.
  /// @return
  void clear();

private:
  std::map<std::string, Token*> _tab;
};



////////////////////////////////////////////////////////////////////////
/// Define a class to represent a parser tree.
/// Every tree would have two pools: token pool and symbol pool.
/// Token pool(Pool) stores all tokens in this tree.
/// Sym pool (Pool) stores all symbols in this tree.
/// And every opertion must be based on its own pool.
////////////////////////////////////////////////////////////////////////
class Tree{
public:
  Tree();
  ~Tree();

  /// Generating an new parser tree.
  /// @param file Source file name, or NULL for stdin.
  /// @return Root of this parser tree.
  Token* gen(const char* file=NULL);

  /// Get root of tree.
  /// @return Root of this tree.
  Token* get()const;

  /// Get string based tree.
  /// @return Parser tree represented in string. 
  std::string toString()const;

  /// Destory tree of root.
  /// Free all space of root and sub tree, also clear pools.
  void clear();

private:
  Token *_root;
  Pool<Token> _tokenPool;
  Pool<Sym> _symPool;

  /// Bind token pool and symbol pool to class Token and Sym.
  /// Be sure pools are binded before run yyparse()
  void _bindPools();
};

// Do nothing.
// @return NULL;
void* _dummy(const void *a, ...);

}; // namespace psr 



#endif // YYCOM_H_ 
