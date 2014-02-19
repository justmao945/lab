
#include<cassert>
#include<cstring>
#include"token.h"

using namespace pel;

//-------------------------
//    Token impl
//-------------------------
void Token::cinit()
{
  this->col = this->line = 1;
  this->text = 0;
  this->textlen = this->textsize = 0;
  this->rep = TOK_nil;
  this->errcol = 0;
  this->errmsg = 0;
}

void Token::copy(const Token& token)
{
  this->line = token.line;
  this->col  = token.col;
  this->rep  = token.rep;
  this->text = new char[token.textlen+1];
  std::memcpy(this->text, token.text, token.textlen);
  this->text[token.textlen] = 0;
  this->textlen = token.textlen;
}

Token::Token()
{
  cinit();
}

Token::Token(const char *text, std::size_t len)
{
  cinit();
  this->Text(text, len);
}

Token::Token(const Token& token)
{
  copy(token);
}

Token& Token::operator=(const Token& token)
{
  if(this != &token){
    delete[] this->text;
    copy(token);
  }
  return *this;
}

Token::~Token()
{
  delete[] this->text;
}


std::size_t Token::Col() const
{
  return col;
}

Token& Token::Clear()
{
  this->line = this->col = 0;
  this->textlen = 0;
  this->rep = TOK_nil;
  if(this->text){
    this->text[0] = 0;
  }
  return *this;
}

std::size_t Token::ErrCol()const
{
  return col;
}

Token& Token::ErrCol(std::size_t errcol)
{
  this->errcol = errcol;
  return *this;
}

const char* Token::ErrMsg()const
{
  return errmsg;
}

Token& Token::ErrMsg(const char* msg)
{
  this->errmsg = msg;
  return *this;
}

std::size_t Token::Line() const
{
  return line;
}

Token& Token::Pos(std::size_t line, std::size_t col)
{
  this->line = line;
  this->col = col;
  return *this;
}

void Token::Print(FILE *stream)const
{
#ifdef _MSC_VER  // Only work in libcmt or msvcrt.
  std::fprintf(stream, "%Iu:%Iu %s", line, col, text);
#else
  std::fprintf(stream, "%zu:%zu %s", line, col, text);
#endif
}

TokenRep Token::Rep() const
{
  return rep;
}

Token& Token::Rep(TokenRep rep)
{
  this->rep = rep;
  return *this;
}

const char* Token::Text() const
{
  return text;
}

Token& Token::Text(const char *text, std::size_t len)
{
  if( len == 0 ){
    len = std::strlen(text);
  }
  if(this->textlen < len){
    this->textlen = this->textsize = len;
    delete[] this->text;
    this->text = new char[len + 1];
  }
  std::memcpy(this->text, text, len);
  this->text[len] = 0;
  return *this;
}

Token& Token::TextAppend(int ch)
{
  if(this->textlen >= this->textsize){
    if(this->textsize < 16){
      this->textsize = 16;
    }else{
      this->textsize <<= 1;
    }
    char *newtext = new char[this->textsize + 1];
    std::memcpy(newtext, this->text, this->textlen);
    delete[] this->text;
    this->text = newtext;
  }
  this->text[this->textlen++] = ch;
  this->text[this->textlen] = 0;
  return *this;
}
