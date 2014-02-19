#define _CRT_SECURE_NO_WARNINGS  // suppress warning of fopen in MSC

#include<cassert>
#include<cstring>
#include"bufio.h"
#include"new.h"
#include"scanner.h"
#include"token.h"

using namespace pel;

//-------------------------
//    Scanner impl
//-------------------------

// e.g. input valid char `n` return `\n`, or invalid char `c` return `c`.
// @return escaped char.
// TODO: support \xhh, \xhhhh, and \ooo
static int escape(int ch)
{
  switch(ch){
  case 'a':
    return '\a';
  case 'b':
    return '\b';
  case 'f':
    return '\f';
  case 'n':
    return '\n';
  case 'r':
    return '\r';
  case 't':
    return '\t';
  case '\\':
  case '\'':
  case '\"':
  default:
    return ch;
  }
}


Scanner::Scanner()
{
  this->fin     = 0;
  this->fname   = 0;
  this->reader  = 0;
  this->string  = 0;
  this->tabwidth = 4;
  this->line = 1;
  this->col = 1;
}

Scanner::~Scanner()
{
  if( this->fname && this->fin ){
    std::fclose(fin);
  }
  delete fname;
  delete reader;
}

Scanner& Scanner::Close()
{
  if( this->fname && this->fin ){ // by Open()
    std::fclose(this->fin);
    delete[] this->fname;
    this->fname = 0;
  }
  this->fin = 0; // From file stream
  this->string = 0; // From string stream
  delete this->reader;
  this->reader = 0;
  return *this;
}

const Token* Scanner::NextToken(Token *tok)
{
  assert(this->reader);

  enum ScannerState{
    SS_start,
    SS_done,
    SS_ident,
    SS_zero, // 0
    SS_nzero,  // 1-9
    SS_dec, // 123
    SS_oct, // 012
    SS_hex,   // first hex digit after X, a in 0Xabef
    SS_hex_1, // following hex digit after above case, bef in 0Xabef
    SS_float0, // digit before .  123 in 123.422E876
    SS_float1, // first digit after .   4 in 123.422E876
    SS_float1_5, // digits following above case, 22 in 123.422E876
    SS_float2,   // + or - after E
    SS_float2_1, // first digit after + or - or E, 8 in 123.422E876
    SS_float2_2, // digits following above case, 76 in 123.422E876
    SS_squot, // single-quotes  '
    // SS_squotesc, escape char in '' is not supported, aka raw string
    SS_dquot, // double-quotes  "
    SS_dquotesc, // escape char in ""
    SS_asi,// = in == =~ =
    SS_lt, // < in <= <
    SS_gt, // > in >= >
    SS_ne, // ! in !=
    SS_and,  // & in &&
    SS_or,   // | in ||
    SS_plus, // + in += and +
    SS_minus, // - in -= and -
    SS_enter, // \r
    SS_dot,   // .  float or dot operator
  } 
  state = SS_start;   // always begins at start.

  if( tok == 0 ){ // create new token
    tok = new Token();
  }else{
    tok->Clear();
  }
  bool save = true;       // flag to indicate save to result string.
  unsigned int ccol = this->col; // char col
  tok->Pos(this->line, this->col); // init token position

  while( state != SS_done ){
    int c = this->reader->ReadChar();
    save = true;
    switch(state){
    case SS_start:
      if(c == '_' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')){
        state = SS_ident;
        ccol++;
      }else if( c == '0' ){
        state = SS_zero;
        ccol++;
      }else if( c >= '1' && c <= '9' ){
        state = SS_nzero;
        ccol++;
      }else if( c == ' ' ){
        save = false;
        ccol++;
        this->col = ccol;
        tok->Pos(this->line, this->col);
      }else if( c == '\t' ){
        save = false;
        ccol += this->tabwidth;
        this->col = ccol;
        tok->Pos(this->line, this->col);
      }else if( c == '\n' ){
        save = false;
        this->line++;
        this->col = ccol = 1;
        tok->Pos(this->line, this->col);
      }else if( c == '\r' ){
        save = false;
        state = SS_enter;
      }else if( c == '\'' ){
        save = false;
        ccol++;
        state = SS_squot;
      }else if( c == '\"' ){
        save = false;
        ccol++;
        state = SS_dquot;
      }else if( c == '=' ){
        ccol++;
        state = SS_asi;
      }else if( c == '<'){
        ccol++;
        state = SS_lt;
      }else if( c == '>' ){
        ccol++;
        state = SS_gt;
      }else if( c == '!' ){
        ccol++;
        state = SS_ne;
      }else if( c == '&' ){
        ccol++;
        state = SS_and;
      }else if( c == '|' ){
        ccol++;
        state = SS_or;
      }else if( c == '+' ){
        ccol++;
        state = SS_plus;
      }else if( c == '-' ){
        ccol++;
        state = SS_minus;
      }else if( c == '.' ){
        ccol++;
        state = SS_dot;
      }else{
        state = SS_done;
        switch(c){
        case EOF:
          save = false;
          tok->Rep(TOK_eof);
          break;
        case '$':
        case '(':
        case ')':
        case '*':
        case ',':
        case ':':
        case ';':
        case '[':
        case ']':
        case '{':
        case '}':
          tok->Rep(static_cast<TokenRep>(c));
          ccol++;
          break;
        default:
          tok->Rep(TOK_err).ErrCol(ccol).ErrMsg("Unknown token");
          ccol++;
          break;
        }
      } // ~ if-else
      break; // ~ case SS_start

    case SS_ident:
      if(!( c == '_' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))){
        this->reader->UnReadChar(c);
        save = false;
        state = SS_done;
        if(std::strncmp("null", tok->Text(), 4) == 0 ){
          tok->Rep(TOK_null);
        }else{
          tok->Rep(TOK_ident);
        }
      }else{
        ccol++;
      }
      break;

    case SS_float0:
      if( c == '.' ){
        state = SS_float1;
        ccol++;
      }else if( c == 'e' || c == 'E' ){
        state = SS_float2;
        ccol++;
      }else if( !(c >= '0' && c <= '9') ){
        state = SS_done;
        save = false;
        tok->Rep(TOK_int); // here return int but not  float !!
        this->reader->UnReadChar(c);
      }else{
        ccol++;
      }
      break;

    case SS_float1:
      if( !(c >= '0' && c <= '9') ){
        state = SS_done;
        save = false;
        this->reader->UnReadChar(c);
        tok->Rep(TOK_float); // have dot at end
      }else{
        ccol++;
        state = SS_float1_5;
      }
      break;

    case SS_float1_5:
      if( c == 'e' || c == 'E' ){
        state = SS_float2;
        ccol++;
      }else if( !(c >= '0' && c <= '9') ){
        state = SS_done;
        save = false;
        this->reader->UnReadChar(c);
        tok->Rep(TOK_float);
      }else{
        ccol++;
      }
      break;

    case SS_float2:
      state = SS_float2_1;
      if( c == '-' || c == '+' ){
        ccol++;
      }else{
        save = false;
        this->reader->UnReadChar(c);
      }
      break;

    case SS_float2_1:
      if( c >= '0' && c <= '9' ){
        ccol++;
        state = SS_float2_2;
      }else{
        save = false;
        state = SS_done;
        this->reader->UnReadChar(c);
        tok->Rep(TOK_err).ErrCol(ccol).ErrMsg("Unknown token");
      }
      break;

    case SS_float2_2:
      if( c >= '0' && c <= '9' ){
        ccol++;
      }else{
        save = false;
        state = SS_done;
        this->reader->UnReadChar(c);
        tok->Rep(TOK_float);
      }
      break;

    case SS_hex:
      if( (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F') ){
        state = SS_hex_1;
        ccol++;
      }else{
        state = SS_done;
        save = false;
        this->reader->UnReadChar(c);
        tok->Rep(TOK_err).ErrCol(col).ErrMsg("Unknown token");
      }
      break;

    case SS_hex_1:
      if( (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F') ){
        ccol++;
      }else{
        state = SS_done;
        save = false;
        this->reader->UnReadChar(c);
        tok->Rep(TOK_int16);
      }
      break;

    case SS_oct:
      if( c >= '0' && c <= '7' ){
        ccol++;
      }else if( c == '8' || c == '9' ){
        state = SS_float0;
        ccol++;
      }else if( c == '.' ){
        state = SS_float1;
        ccol++;
      }else if( c == 'e' || c == 'E' ){
        state = SS_float2;
        ccol++;
      }else{
        state = SS_done;
        save = false;
        this->reader->UnReadChar(c);
        tok->Rep(TOK_int8);
      }
      break;

    case SS_dec:
      if( c >= '0' && c <= '9' ){
        ccol++;
      }else if( c == '.' ){
        state = SS_float1;
        ccol++;
      }else if( c == 'e' || c == 'E' ){
        state = SS_float2;
        ccol++;
      }else{
        state = SS_done;
        save = false;
        this->reader->UnReadChar(c);
        tok->Rep(TOK_int10);
      }
      break;

    case SS_zero:
      if( c == 'x' || c == 'X' ){
        state = SS_hex;
        ccol++;
      }else if( c == '.' ){
        state = SS_float1;
        ccol++;
      }else if( c == 'e' || c == 'E' ){
        state = SS_float2;
        ccol++;
      }else if( c >= '0' && c <= '7' ){
        state = SS_oct;
        ccol++;
      }else if( c == '8' || c == '9' ){
        state = SS_float0;
        ccol++;
      }else{
        this->reader->UnReadChar(c);
        save = false;
        state = SS_done;
        tok->Rep(TOK_int);
      }
      break;

    case SS_nzero:
      if( c >= '0' && c <= '9' ){
        state = SS_dec;
        ccol++;
      }else if( c == '.' ){
        state = SS_float1;
        ccol++;
      }else if( c == 'e' || c == 'E' ){
        state = SS_float2;
        ccol++;
      }else{
        this->reader->UnReadChar(c);
        save = false;
        state = SS_done;
        tok->Rep(TOK_int);
      }
      break;

    case SS_enter:
      if( c != '\n' ){
        this->reader->UnReadChar(c);
      }
      save = false;
      state = SS_start;
      this->line++;
      this->col = ccol = 1;
      tok->Pos(this->line, this->col); // reset position
      break;

    case SS_squot:
      if( c == '\'' ){
        save = false;
        state = SS_done;
        tok->Rep(TOK_string);
        ccol++;
      }else if( c == '\n' || c == '\r' ){
        save = false;
        state = SS_done;
        tok->Rep(TOK_err).ErrCol(ccol).ErrMsg("String breaked by newline");
        this->line++;
        this->col = ccol = 1;
      }else { // valid literal
        ccol++;
      }
      break;

    case SS_dquot:
      if( c == '\\' ){
        save = false;
        state = SS_dquotesc;
        ccol++;
      }else if( c == '\"' ){
        save = false;
        state = SS_done;
        tok->Rep(TOK_string);
        ccol++;
      }else if( c == '\n' || c == '\r' ){
        save = false;
        state = SS_done;
        tok->Rep(TOK_err).ErrCol(ccol).ErrMsg("String breaked by newline");
        this->line++;
        this->col = ccol = 1;
      }else { // valid literal
        ccol++;
      }
      break;

    case SS_dquotesc:
      //TODO: add multiple line string support.
      state = SS_dquot;
      c = escape(c);
      break;

    case SS_asi:
      state = SS_done;
      if( c == '=' ){
        tok->Rep(TOK_eq);
        ccol++;
      }else if( c == '~' ){
        tok->Rep(TOK_like);
        ccol++;
      }else{
        this->reader->UnReadChar(c);
        save = false;
        tok->Rep(TOK_asi);
      }
      break;

    case SS_lt:
      state = SS_done;
      if( c == '=' ){
        tok->Rep(TOK_le);
        ccol++;
      }else{
        this->reader->UnReadChar(c);
        save = false;
        tok->Rep(TOK_lt);
      }
      break;

    case SS_gt:
      state = SS_done;
      if( c == '=' ){
        tok->Rep(TOK_ge);
        ccol++;
      }else{
        this->reader->UnReadChar(c);
        save = false;
        tok->Rep(TOK_gt);
      }
      break;
      
    case SS_ne:
      state = SS_done;
      if( c == '=' ){
        tok->Rep(TOK_ne);
        ccol++;
      }else{
        this->reader->UnReadChar(c);
        save = false;
        tok->Rep(TOK_err).ErrCol(ccol).ErrMsg("Unknown token");
      }
      break;

    case SS_and:
      state = SS_done;
      if( c == '&' ){
        tok->Rep(TOK_and);
        ccol++;
      }else{
        this->reader->UnReadChar(c);
        save = false;
        tok->Rep(TOK_err).ErrCol(ccol).ErrMsg("Unknown token");
      }
      break;

    case SS_or:
      state = SS_done;
      if( c == '|' ){
        tok->Rep(TOK_or);
        ccol++;
      }else{
        this->reader->UnReadChar(c);
        save = false;
        tok->Rep(TOK_err).ErrCol(ccol).ErrMsg("Unknown token");
      }
      break;

    case SS_plus:
      state = SS_done;
      if( c == '=' ){
        tok->Rep(TOK_pasi);
        ccol++;
      }else{
        this->reader->UnReadChar(c);
        save = false;
        tok->Rep(TOK_plus);
      }
      break;

    case SS_minus:
      state = SS_done;
      if( c == '=' ){
        tok->Rep(TOK_masi);
        ccol++;
      }else{
        this->reader->UnReadChar(c);
        save = false;
        tok->Rep(TOK_minus);
      }
      break;

    case SS_dot:
      if( c >= '0' && c <= '9' ){
        state = SS_float1;
        ccol++;
      }else{
        state = SS_done;
        save = false;
        this->reader->UnReadChar(c);
        tok->Rep(TOK_dot);
      }
      break;

    case SS_done:
    default: // should never happen
      fprintf(stderr, "Scanner bug: state=%d\n", state);
      state = SS_done;
      tok->Rep(TOK_err).ErrCol(ccol).ErrMsg("Scanner bug");
      break;
    } // ~ switch(state)

    if(save){
      tok->TextAppend(c);
    }
  } // ~ while
  this->col = ccol;  // update col at last for next token, ccol updated in each loop
  return tok;
}

Scanner& Scanner::Open(const char *file, bool *ok)
{
  assert(file && file[0] != 0);

  // Close file if already opened.
  this->Close();

  this->fin = std::fopen(file, "rb");
  if(this->fin == NULL){
    std::perror("std::fopen @ Scanner::Open");
  }else{
    std::size_t len = std::strlen(file);
    this->fname = new char[len+1];
    std::memcpy(this->fname, file, len);
    this->reader = new Reader(this->fin);
  }

  if(ok){
    *ok = ( this->fin != NULL );
  }
  return *this;
}

const char* Scanner::File() const
{
  return this->fname;
}

const char* Scanner::From() const
{
  return this->string;
}

Scanner& Scanner::From(FILE* stream)
{
  assert(stream);
  this->Close();
  this->fin = stream;
  this->reader = new Reader(this->fin);
  return *this;
}

Scanner& Scanner::From(const char* string, std::size_t len)
{
  assert(string);
  this->Close();
  this->reader = new Reader(string, len);
  this->string = string;
  return *this;
}

unsigned int Scanner::TabWidth() const
{
  return this->tabwidth;
}

Scanner& Scanner::TabWidth(unsigned int width)
{
  this->tabwidth = width;
  return *this;
}


//--------------------------------
//   Scanner test
//--------------------------------
#include"test.h"

namespace pel{

void TestScanner()
{
  TestBegin("Scanner");

  Scanner s;
  Token tok;

  s.From(
    "    \t\n"
    "  \ta.b = 1;\n"
    "  > < == <= >=  != =~ && || . , : ; [ ] ( ) { } = + += -= *\r\n"
    "123 0123 0xafbDDdd00\r"
    "01233999 00. 1E9 1E-9 1e+9 1.1e900\n"
    "\'fasdfa/#@$@@@\\n\'\n"
    "\"fjalksdfjal#@$@#$@#\\n\"\n"
    "_abcd\r"
  );

  while( s.NextToken(&tok) && tok.Rep() != TOK_eof ){
    tok.Print();
    std::putchar('\n');
  }

  TestEnd();
}

} // ~pel
