%{

#include <stdio.h>
#include "utils.h"

#define YYSTYPE_IS_DECLARED
typedef Token YYSTYPE;

#include "bison.h"

%}

integer_value   0[xX][0-9a-fA-F]+|[1-9][0-9]*|[0-7]+
float_value     ([0-9]+\.|\.[0-9])?[0-9]*([eE][-+]?[0-9]+)?

%%

{integer_value}  {sscanf(yytext, "%lli", &yylval.ival); return IVAL;}
{float_value}    {sscanf(yytext, "%lf", &yylval.fval); return FVAL;}

\+        {return '+';}
-         {return '-';}
\*        {return '*';}
\(        {return '(';}
\)        {return ')';}
\n        {return '\n';} 
[\t ]+    {;}
.         {fprintf(stderr,"Unrecognized character: '%s'\n", yytext);}

%%

int yywrap(void)
{
  return 1;
}
  

