/*
 * File: token.h
 * Provides tokens' representation for scanner.
 */


#ifndef PEL_TOKEN_H
#define PEL_TOKEN_H

#include<cstddef>
#include<cstdio>

namespace pel{

enum TokenRep{
  TOK_dollar = '$', // 36
  TOK_lpar   = '(', // 40
  TOK_rpar   = ')', // 41
  TOK_star   = '*', // 42
  TOK_plus   = '+', // 43
  TOK_comma  = ',', // 44
  TOK_minus  = '-', // 45
  TOK_dot    = '.', // 46
  TOK_colon  = ':', // 58
  TOK_scolon = ';', // 59
  TOK_lt     = '<', // 60
  TOK_asi    = '=', // 61
  TOK_gt     = '>', // 62
  TOK_lbra   = '[', // 91
  TOK_rbra   = ']', // 93
  TOK_lcur   = '{', // 123
  TOK_rcur   = '}', // 125
  TOK_eq = 256, // ==
  TOK_le,   // <=  257
  TOK_ge,   // >=  258
  TOK_ne,   // !=  259
  TOK_like, // =~  260
  TOK_and,  // &&  261
  TOK_or,   // ||  262
  TOK_pasi, // +=  263
  TOK_masi, // -=  264
  TOK_null, // null   265
  TOK_int,  // default is 10, dec   266
  TOK_int8, // oct  267
  TOK_int10,// dec  268
  TOK_int16,// hex  269
  TOK_float,//  270
  TOK_string,// 271
  TOK_ident, // 272
  TOK_nil, // empty token 273
  TOK_err, // unknown token   274
  TOK_eof, // end of file or string  275
};


class Token{
public:
  Token();

  // Ctor.
  // @text Token literal
  // @len Length of text, if equals to 0 then treat text as a C string.
  explicit Token(const char *text, std::size_t len=0);

  Token(const Token& token);

  Token& operator=(const Token& token);

  ~Token();

  // @return First char of token column number.
  std::size_t Col() const;

  // Erase all data of token.
  Token& Clear();

  // @return column of error occured.
  std::size_t ErrCol()const;

  Token& ErrCol(std::size_t errcol);

  // don't free msg returned.
  const char* ErrMsg()const;

  // only accept literal string.
  Token& ErrMsg(const char* msg);

  // @return Line number of token.
  std::size_t Line() const;

  // Set token position.
  // @return self
  Token& Pos(std::size_t line, std::size_t col);
  
  // print to stdout
  void Print(FILE *stream=stdout) const;

  // @return representation
  TokenRep Rep() const;

  // Set representation(aka enumeration value) of token
  // @return self
  Token& Rep(TokenRep rep);

  // @return interal text, don't modify or delete it.
  const char* Text() const;

  // Set literal text of token.
  // @text Token literal
  // @len Length of text, if equals to 0 then treat text as a C string.
  // @return self
  Token& Text(const char *text, std::size_t len=0);

  // Append char `ch` at the end of text.
  Token& TextAppend(int ch);

private:
  std::size_t   line;
  std::size_t   col;
  char         *text;
  std::size_t   textlen;
  std::size_t   textsize;
  TokenRep      rep;

  std::size_t   errcol; // error column, valid only when rep is TOK_err
  const char   *errmsg; // error message, same as errcol, and only accept literal stirng

  void cinit(); // common initialization
  void copy(const Token&);
};

} // ~pel

#endif