/////////////////////////////////////////////////////////////////////
///  @file psrdrv.cc implements parser tree, token, and a symbol table.
///  ATTENTION:
///   1. This file extern variables from parser.cc(created by bison)
///       Token psr::troot;
///       std::string treeToString(Token *root);
///
////////////////////////////////////////////////////////////////////

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <string>
#include "psrdrv.h"
#include "parser.h"
#include "parser-ext.h"  // extend functions from parser.y

extern int yyparse();

namespace psr{

//  class Token implementation.  BEGIN
Token::Token()
{
  clear();
}

Token::Token(const std::string& yytext, int token, int filter)
{
  this->yytext = yytext;
  this->token = token;
  this->lineno = 0;
  this->filter = filter;
  clrptr();
}

Token::~Token()
{
}


Token& Token::operator=(const Token& token)
{
  return copy(token);
}

Token* Token::leftmost()
{
  Token* p = this;
  while(p->l)
    p = p->l;
  return p;
}

Token* Token::rightmost()
{
  Token* p = this;
  while(p->r)
    p = p->r;
  return p;
}


Token* Token::lrNth(int n)
{ 
  int i = 0;
  Token* p = this;

  if( n > 0 ){
    for( i=0; i<n && p; i++, p=p->r ){}
  } else if ( n < 0 ){
    n = -n;
    for( i=0; i<n && p; i++, p=p->l ){}
  } else{
    return this;
  }
  if( i < n ) return NULL; // breaked
  else return p;
}


Token* Token::leftNth(int n)
{
  return lrNth(-n);
}

Token* Token::rightNth(int n)
{
  return lrNth(n);
}



Token*  Token::lastSibling()
{
  Token* p = this;
  while(p->sibling)
    p = p->sibling;
  return p;
}

Token& Token::copy(const Token& tok)
{
  // Don't use std::memcpy, because of std::string 
  token = tok.token;
  yytext = tok.yytext;
  child = tok.child;
  sibling = tok.sibling;
  parent = tok.parent;
  lsibling = tok.lsibling;
  lineno = tok.lineno;
  sym = tok.sym;
  filter = tok.filter;

  l = tok.l;
  r = tok.r;
  return *this;
}


Token* Token::clear()
{
  yytext.clear();
  lineno = token = 0;
  filter = Filter::NA;
  return clrptr();
}

Token* Token::clrptr()
{
  child = sibling = NULL;
  parent = lsibling = NULL;
  sym = NULL;
  l = r = NULL;
  return this;
}

Token* Token::dup()const
{
  Token* p = poolOne();
  *p = *this;
  p->clrptr();
  return p;
}

Token& Token::operator <(Token& tok)
{
  this->r = tok.leftmost();
  this->r->l = this;
  return tok;
}

Token& Token::operator <(Token* ptok)
{
  if(ptok)
    return *this < *ptok;
  else
    return *this;
}

Token& Token::operator >(Token& tok)
{
  Token* rm = this->rightmost();
  rm->r = &tok;
  rm->r->l = this;
  return *rm->r;
}

Token& Token::operator >(Token* ptok)
{
  if(ptok)
    return *this > *ptok;
  else
    return *this;
}

Token& Token::operator <<(Token& tok)
{
  tok.parent = this;
  return *(child = &tok);
}

Token& Token::operator <<(Token* ptok)
{
  if(ptok)
    return *this << *ptok;
  else
    return *this;
}

Token& Token::operator >>(Token& tok)
{
  Token * lstsib = lastSibling();
  tok.lsibling = lstsib;
  return *(lstsib->sibling = &tok);
}

Token& Token::operator >>(Token* ptok)
{
  if(ptok)
    return *this >> *ptok;
  else
    return *this;
}

Token* Token::genSym(Sym* tab)
{
  Token* ret = NULL, *p = NULL;
  Sym* iouter;

  if( sym == NULL ) {
    if ( tab == NULL ){
      sym = Sym::poolOne();
    } else{
      sym = tab;
    }
  }
  iouter = sym;

  for(Token* sib=this; sib!=NULL; sib=sib->sibling){
    sib->sym = sym;
    if( sib->filter & Filter::EX_DECL_STM ){
      ret = sym->add(sib);
      if(ret != NULL)
      fprintf(stderr, "[%d] error: redefinition of `%s`,"
          " previous definition is in line [%d]\n",
          sib->lineno, sib->yytext.c_str(), ret->lineno);
    }
    // link inner symbol table outer ptr to this symbol table
    if( sib->child ) sib->child->_setSiblingSymOuter(iouter);
    if( sib->filter & Filter::EX_FUNC_STM ){
      if( (p = getFuncBody(sib)) ) p->_setSiblingSymOuter(iouter);
      if( (p = getFuncParams(sib)) ) p->_setSiblingSymOuter(iouter);
      //if( (p = getCallArgs(sib)) ) p->_setSiblingSymOuter(iouter);
      // Dup with getFuncParams.
    }
    if( sib->filter & Filter::EX_ARRAY_STM ) { 
      if( (p = getArraySize(sib)) ) p->_setSiblingSymOuter(iouter);
    }
    if( sib->filter == Filter::SELE_STM ){
      printf("If stm <%s> %p %p\n", sib->yytext.c_str(), sym, iouter);
      if( (p = getIfCond(sib)) )  p->_setSiblingSymOuter(iouter);
      if( (p = getIfTrueBody(sib)) ) p->_setSiblingSymOuter(iouter);
      if( (p = getIfFalseBody(sib)) ) p->_setSiblingSymOuter(iouter);
    }
    if( sib->filter == Filter::ITER_STM ){
      if( (p = getWhileCond(sib)) ) p->_setSiblingSymOuter(iouter);
      if( (p = getWhileBody(sib)) ) p->_setSiblingSymOuter(iouter);
    }
    if( sib->filter == Filter::RET_STM ){
      if( (p = getRet(sib)) ) p->_setSiblingSymOuter(iouter);
    }
    if( sib->filter == Filter::OP_STM ){

    }
  }
  return this;
}

void Token::_setSiblingSymOuter(Sym *outer)
{
  Token *p = this;
  while( p ){
    if( !p->sym ){
      p->genSym();
    }
    p->sym->outer = outer;
    p = p->sibling;
  }
}

// class Token implementation.  END



//  class Sym implementation.  BEGIN
Sym::Sym()
{
  outer = NULL;
}

Sym::~Sym()
{
}

Token* Sym::get(const char* sym)
{
  return get(std::string(sym));
}

Token* Sym::get(const std::string& sym)
{
  std::map<std::string, Token*>::iterator it = _tab.find(sym);
  if(it != _tab.end())
    return it->second;
  else
    return NULL;
}

Token* Sym::add(Token& sym)
{
  std::pair<std::map<std::string, Token*>::iterator,bool> ret =
      _tab.insert(std::make_pair(sym.yytext, &sym));
  if(ret.second)
    return NULL;
  else
    return ret.first->second;
}

Token* Sym::add(Token* sym)
{
  return add(*sym);
}

void Sym::clear()
{
  outer = NULL;
  _tab.clear();
}
//  class Sym implementation.  END




//  class Tree implementation.  BEGIN
Tree::Tree()
{
  _root = NULL;
}

Tree::~Tree()
{
  clear();
}

void Tree::_bindPools()
{
  Token::pool = &_tokenPool;    // init pools
  Sym::pool = &_symPool;
}


Token* Tree::gen(const char* file)
{
  FILE *fre = NULL;
  if(file){
    fre = std::freopen(file, "r", stdin);
    if(fre == NULL){
      fprintf(stderr, "Can't open file %s: %s\n", file, strerror(errno));
      return NULL;
    }
  }

  clear();
  _bindPools();
  
  d_printf("preparing to parse...\n");
    
  yyparse();

  d_printf("parse done.\n");
  _root = troot;

  if(file && fre)
    fclose(stdin);
  
  return _root;
}

Token* Tree::get() const
{
  return _root;
}

std::string Tree::toString()const
{
  return psr::treeToString(_root);
}

void Tree::clear()
{
  _root = NULL;
  _tokenPool.clear();
  _tokenPool.clear();
  _symPool.clear();
}
//  class Tree implementation.  END


void* _dummy(const void *a, ...)
{
  return NULL;
}

};  // end of namespace.

