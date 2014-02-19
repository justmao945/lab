%{
#include <new>
#include <string>
#include <cstdio>
#include <cstring>
#include "psrdrv.h"

#define YYSTYPE_IS_DECLARED
using namespace psr;

typedef Token* YYSTYPE;

#undef __act__
#define __act__(x) { \
    yylval = Token::poolOne(); \
    yylval->lineno = lineno; \
    yylval->yytext = yytext; \
    yylval->token = x; \
    return (x); \
  }


//#define DEBUG_SCANNER
#if defined  DEBUG_SCANNER && defined DEBUG_PSR
#define _act_(x) { printf(" %s ", yytext); __act__(x) }
#else
#define _act_(x)  __act__(x)
#endif
  
#include "parser.h"  // file created by bison.
  
// Count lines
// @param s String to count
// @return lines in string s 
static int countLine(const char* s);


extern YYSTYPE yylval;
int lineno = 1;

%}

d   [0-9]
h   [0-9a-fA-F]
a   [a-z]
A   [A-Z]
aA  [a-zA-Z]

newline         \n
whitespace      [ \t]+
identifier      [_a-zA-Z][_a-zA-Z0-9]*
integer_suffix  [uU]((ll|LL)|[lL]?)|([lL]|(ll|LL))[uU]?
integer_value   (0[xX][0-9a-fA-F]+|[1-9][0-9]*|[0-7]+){integer_suffix}?
float_value     ([0-9]+\.|\.[0-9])?[0-9]*([eE][-+]?[0-9]+)?[fF]?
char_value      \'(.|\\(['"?\\abfnrtv]|([0-7]+|0[xX][0-9a-fA-F]+)))\'
wchar_value     \'\\[uU]{h}{h}{h}{h}({h}{h}{h}{h})?\'
string_value    L?\".*\"
comments        \/\/.*[\n\r]|\/\*\/*([^*/]|[^*]\/|\*[^/])*\**\*\/


%%
"auto"           _act_(AUTO);
"break"          _act_(BREAK);
"case"           _act_(CASE);
"char"           _act_(CHAR);
"const"          _act_(CONST);
"continue"       _act_(CONTINUE);
"default"        _act_(DEFAULT);
"do"             _act_(DO);
"double"         _act_(DOUBLE);
"else"           _act_(ELSE);
"enum"           _act_(ENUM);
"extern"         _act_(EXTERN);
"float"          _act_(FLOAT);
"for"            _act_(FOR);
"goto"           _act_(GOTO);
"if"             _act_(IF);
"int"            _act_(INT);
"long"           _act_(LONG);
"register"       _act_(REGISTER);
"return"         _act_(RETURN);
"short"          _act_(SHORT);
"signed"         _act_(SIGNED);
"sizeof"         _act_(SIZEOF);
"static"         _act_(STATIC);
"struct"         _act_(STRUCT);
"switch"         _act_(SWITCH);
"typedef"        _act_(TYPEDEF);
"union"          _act_(UNION);
"unsigned"       _act_(UNSIGNED);
"void"           _act_(VOID);
"volatile"       _act_(VOLATILE);
"while"          _act_(WHILE);

">>="    _act_(SHIFT_RIGHT_ASSIGN);
"<<="    _act_(SHIFT_LEFT_ASSIGN);
"+="     _act_(ADD_ASSIGN);
"-="     _act_(SUB_ASSIGN);
"*="     _act_(MUL_ASSIGN);
"/="     _act_(DIV_ASSIGN);
"%="     _act_(MOD_ASSIGN);
"&="     _act_(AND_ASSIGN);
"^="     _act_(XOR_ASSIGN);
"|="     _act_(OR_ASSIGN);
">>"     _act_(RR_OP);
"<<"     _act_(LL_OP);
"++"     _act_(INC_OP);
"--"     _act_(DEC_OP);
"->"     _act_(PTR_OP);
"&&"     _act_(AND_OP);
"||"     _act_(OR_OP);
"<="     _act_(LE_OP);
">="     _act_(GE_OP);
"=="     _act_(EQ_OP);
"!="     _act_(NE_OP);
".*"     _act_(DOT_STAR);
"->*"    _act_(PTR_STAR);

";"      _act_(';');
"{"      _act_('{');
"}"      _act_('}');
","      _act_(',');
":"      _act_(':');
"="      _act_('=');
"("      _act_('(');
")"      _act_(')');
"["      _act_('[');
"]"      _act_(']');
"."      _act_('.');
"&"      _act_('&');
"!"      _act_('!');
"~"      _act_('~');
"-"      _act_('-');
"+"      _act_('+');
"*"      _act_('*');
"/"      _act_('/');
"%"      _act_('%');
"<"      _act_('<');
">"      _act_('>');
"^"      _act_('^');
"|"      _act_('|');
"?"      _act_('?');

"\\"     _act_('\\');
"\""     _act_('\"');
"\'"     _act_('\'');


{identifier}      _act_(IDENTIFIER);
{integer_value}   _act_(INTEGER_VALUE);
{float_value}     _act_(FLOAT_VALUE);
{char_value}      _act_(CHAR_VALUE);
{wchar_value}     _act_(WCHAR_VALUE);
{string_value}    _act_(STRING_VALUE);
{comments}        {lineno += countLine(yytext);} /* ormit comments */
{whitespace}      {;} /* ormit white space */
{newline}         {lineno++;}
%%




/// When the scanner receives an end-of-file indication from YY_INPUT, 
/// it then checks the yywrap() function. If yywrap() returns false (zero),
/// then it is assumed that the function has gone ahead and set up `yyin' 
/// to point to another input file, and scanning continues. If it returns
/// true (non-zero), then the scanner terminates, returning 0 to its caller.
int yywrap(void)
{
  return 1;
}

/// Count \r\n (DOS format) or \r (MAC format) or \n (UNIX format)
/// in string. FIXME may have bugs..
/// @param s String to check.
/// @return Number of lines in string.
int countLine(const char* s)
{
  int c = 0;
  while(*s != '\0'){
    if(*s == '\r' && *(s+1) == '\n'){
      c++;
      s++;
    }
    else if(*s == '\r' || *s == '\n'){
      c++;
    }
    s++;
  }
  return c;
}
