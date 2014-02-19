%{
#include <stdio.h>
#include "utils.h"

#define YYSTYPE_IS_DECLARED

typedef Token YYSTYPE;
void yyerror(const char* s);
extern int yylex(void);
extern char* yytext;

static void out(const Token* x);

%}

%token IVAL FVAL

%%

command: command exp '\n' { out(&$2); }
  | exp '\n' { out(&$1); }
  | error '\n' { yyclearin; yyerrok; }
  ;

exp: exp '+' term { add(&$$, &$1, &$3); }
  | exp '-' term { sub(&$$, &$1, &$3); }
  | term { cp(&$$, &$1); }
  | /* e */ { zero(&$$); }
  ;

term: term '*' factor { mul(&$$, &$1, &$3); }
  | factor { cp(&$$, &$1); }
  ;

factor: number { cp(&$$, &$1); }
  | '(' exp ')' { cp(&$$, &$2); }
  | '+' factor { cp(&$$, &$2); }
  | '-' factor { neg(&$$, &$2); }
  ;

number: IVAL { cp(&$$, &$1); $$.type = INT; }
  | FVAL { cp(&$$, &$1); $$.type = FLOAT; }
  ;

%%

void yyerror(const char* s)
{
  const char* p = yytext;
  fprintf(stderr, "%s, error token:", s);
  while( *p ){
    if( *p == '\n' )
      printf("\\n");
    else
      putchar(*p);
    p++;
  }
  putchar('\n');
}


void out(const Token* x)
{
  putchar('>');
  print(x);
  putchar('\n');
}
