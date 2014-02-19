/**  @file parser.y
 *  This file aims to provide a parser...
 *
 * */

%{
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "psrdrv.h"
#include "parser-ext.h"

#define YYSTYPE_IS_DECLARED
using namespace psr;

typedef Token* YYSTYPE;
void yyerror(const char* s);
extern int yylex(void); // extern form lexer.cc
extern int lineno;   // line number extern form lexer.cc
extern char* yytext; // extern form lexer.cc

namespace psr
{
  Token* troot = NULL;
 // root of parser tree, would be updated after call yyparse().
};

%}

/*  output header file name  */
%defines "parser.h"

/* create parser.output file */
%verbose

/* more detailed error info */
%error-verbose



/* Here will produce 1 shift/reduce conflict, but bison should use 
   shift to resolve the conflict, and that is what we want, so just 
   ignore the warnning.

state 105

   31 selection_stmt: IF '(' expression ')' statement .
   32               | IF '(' expression ')' statement . ELSE statement

    ELSE  shift, and go to state 108

    ELSE      [reduce using rule 31 (selection_stmt)]
    $default  reduce using rule 31 (selection_stmt)

*/
%expect 1


%token AUTO BREAK CASE CHAR CONST CONTINUE DEFAULT DO DOUBLE ELSE ENUM EXTERN FLOAT FOR GOTO IF INT LONG REGISTER RETURN SHORT SIGNED SIZEOF STATIC STRUCT SWITCH TYPEDEF UNION UNSIGNED VOID VOLATILE WHILE
%token SHIFT_RIGHT_ASSIGN SHIFT_LEFT_ASSIGN ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN
%token DIV_ASSIGN MOD_ASSIGN AND_ASSIGN XOR_ASSIGN OR_ASSIGN 
%token RR_OP LL_OP INC_OP DEC_OP PTR_OP AND_OP OR_OP LE_OP GE_OP EQ_OP NE_OP
%token DOT_STAR PTR_STAR
%token IDENTIFIER INTEGER_VALUE FLOAT_VALUE CHAR_VALUE WCHAR_VALUE STRING_VALUE

%%


//  troot is NULL or
//  {*troot*} .child-> declaration_list
//
program:
  declaration_list
    {
      if( $1 != NULL ){
        troot = Token::poolOne();
        *troot << $1->genSym(NULL);
        d_printf("troot gen sym\n");
      }
    }
  | empty 
    {
      troot = NULL;
    }
  ;



//  declaration_list: a list with .sibling ptr.
//    declaration .sibling-> declaration....
//
declaration_list:
  declaration declaration_list   /* right recursive */
    {
      if( $1 != NULL ) {
        *( $$ = $1 ) >> $2;
        d_printf("add declaration %i::<%s>\n",
            $1->lineno,
            $1->yytext.c_str() );
      }
    }
  | declaration 
    {
      $$ = $1;
      if( $1 != NULL ) {
        d_printf("add declaration %i::<%s>\n",
            $1->lineno, $1->yytext.c_str() );
      }
    }
  ;


// Add Filter::EX_DECL_STM to declarations.
//
declaration:
  var_declaration
  | fun_declaration
  ;



// var_declaration: {*IDENTIFIER*}
// 1. Simple declaration: type var;
//   {*IDENTIFIER*} .l-> type_specifier
//
// 2. Array declaration:  type var [ intv ];
//   {*IDENTIFIER*} .l-> type_specifier
//   {*IDENTIFIER*} .r-> {[} .r-> {INTEGER_VALUE} .r-> {]}
//
var_declaration:
  type_specifier IDENTIFIER ';'
    {
      *$1 < ($$ = $2);
      $$->filter = Token::Filter::EX_DECL_STM;
    }
  | type_specifier IDENTIFIER '[' INTEGER_VALUE ']' ';'
    {
      *$1 < ($$ = $2) > $3 > $4 > $5;
      $$->filter = Token::Filter::EX_ARRAY_STM | Token::Filter::EX_DECL_STM;
    }
  | error ';'
    {
      $$ = NULL;
      yyerrok;
    }
  ;
 

type_specifier:
  INT 
  | VOID
  ;


// fun_declaration: {*IDENTIFIER*}
//  {*IDENTIFIER*} .l-> type_specifier
//  {*IDENTIFIER*} .r-> {(} .r-> {)}
//  {(} .child-> params
//  {)} .child-> compound_stmt
//  Finally add Filter::EX_FUNC_STM to token.
//
fun_declaration:
  type_specifier IDENTIFIER '(' params ')' compound_stmt
    {
      Sym *sym = NULL;

      *$1 < ($$ = $2) > $3 > $5;
      *$3 << $4;
      *$5 << $6;
      
      if( $4 &&  $4->token != VOID ){
        sym = $4->genSym()->sym;
        d_printf( "gen param sym on line %i\n", $4->lineno );
      }
      
      if ( $6 != NULL ) {
        $6->genSym(sym);
        d_printf( "gen compound sym on line %i\n", $6->lineno );
      }
      $$->filter = Token::Filter::EX_FUNC_STM 
                | Token::Filter::EX_DECL_STM ;
    }
  ;


// params is VOID or a list ==> param_list
//
params:
  param_list
  | VOID
  ;

// param_list:  a list linked with .sibling ptr.
//   param .sibling-> param .sibling-> ...
//
param_list:
  param_list ',' param 
    {
      *($$ = $1) >> $3;
    }
  | param
  | error ','
    {
      $$ = NULL;
      yyerrok;
    }
  ;


// param:  {*IDENTIFIER*}
// 1. Simple param
//  {*IDENTIFIER*} .l-> type_specifier 
// 
// 2. Array param
//  {*IDENTIFIER*} .l-> type_specifier 
//  {*IDENTIFIER*} .r-> {[} .r-> {]}
//
param:
  type_specifier IDENTIFIER
    {
      *$1 < ($$ = $2);
      $$->filter = Token::Filter::EX_DECL_STM;
    }
  | type_specifier IDENTIFIER '[' ']'
    {
      *$1 < ($$ = $2) > $3 > $4;
      $$->filter = Token::Filter::EX_ARRAY_STM | 
                   Token::Filter::EX_DECL_STM ;
    }
  ;

//  compound_stmt: a list linked with .sibling ptr.
//    local_declarations  .sibling->  statement_list
//
compound_stmt:
  '{' local_declarations statement_list '}'
    {
      // $$, $2, $3 may be NULL.
      if( $2 != NULL)
        *($$ = $2) >> $3;
      else
        $$ = $3;
    }
  ;

//  local_declarations:  a list linked with .sibling ptr.
//    var_declaration .sibling-> var_declaration .sibling-> ...
//
local_declarations:
  var_declaration local_declarations  /* right recursive */
    {
     // $1 may be NULL
     if ( $1 != NULL )
        *($$ = $1) >> $2;
     else 
        $$ = NULL;
    }
  | empty
    {
      $$ = NULL;
    }
  ;

// e
empty:
  ;


//  statement_list: a list linked with .sibling ptr.
//    statement .sibling-> statement .sibling-> ...
//
statement_list:
  statement statement_list /* right recursive */
    {
      // $$, $1, $2(compound_stmt/expression) may be NULL
      if( $1 != NULL )
        *( $$ = $1 ) >> $2;
      else
        $$ = $2;
    }
  | empty 
    {
      $$ = NULL;
    }
  ;


statement:
  expression_stmt
  | compound_stmt
  | selection_stmt
  | iteration_stmt
  | return_stmt
  ;


expression_stmt:
  expression ';'
  | ';'
    {
      $$ = NULL;
    }
  ;


//  selection_stmt: {*IF*}
//  {*IF*} .r-> {(} .r-> {)}
//  {(} .child-> expression
//  {)} .child-> statement
//  
//  or has else statement.
//    {)} .r-> {ELSE}
//    {ELSE} .child-> statement
//
selection_stmt:
  IF '(' expression ')' statement
    {
      *( $$ = $1 ) > $2 > $4;
      *$2 << $3;
      *$4 << $5;
      $$->filter = Token::Filter::SELE_STM;
      d_printf("add selection statement IF: %i\n", $1->lineno);
      if( $5 != NULL ){
        $5->genSym();
        d_printf("gen sym on selection_stmt on line:%i\n", $5->lineno);
      }
    }
  | IF '(' expression ')' statement ELSE statement
    {
      
      *( $$ = $1 ) > $2 > $4 > $6;
      *$2 << $3;
      *$4 << $5;
      *$6 << $7;
      $$->filter = Token::Filter::SELE_STM;
      d_printf("add selection statement IF ELSE: %i\n", $1->lineno);
      if( $5 != NULL ){
        $5->genSym();
        d_printf("gen sym on selection_stmt on line:%i\n", $5->lineno);
      }
      if( $7 != NULL ){
        $7->genSym();
        d_printf("gen sym on selection_stmt on line:%i\n", $7->lineno);
      }
    }
  ;


// iteration_stmt: {*WHILE*}
//   {*WHILE*} .r-> {(} .r-> {)}
//   {(} .child-> expression
//   {)} .child-> statement
//
iteration_stmt:
  WHILE '(' expression ')' statement
    {
      *($$ = $1) > $2 > $4;
      *$2 << $3;
      *$4 << $5;
      $$->filter = Token::Filter::ITER_STM;
      d_printf("add iteration statement WHILE : %i\n", $1->lineno);
      if( $5 != NULL ){
        $5->genSym();
        d_printf("gen sym on iteration_stmt on line:%i\n", $4->lineno);
      }
    }
  ;


// return_stmt: {*RETURN*}
//   {*RETURN*} .child-> expression
return_stmt:
  RETURN ';'
    {
      $$ = $1;
      $$->filter = Token::Filter::RET_STM;
      d_printf("add statement RETURN: %i\n", $1->lineno);
    }
  | RETURN expression ';'
    {
      *($$ = $1) << $2;
      $$->filter = Token::Filter::RET_STM;
      d_printf("add statement RETURN: %i\n", $1->lineno);
    }
  ;

//  expression: {*=*}, this is an operation token.
//    {*=*}  .child-> var
//    var  .sibling-> expression
//
expression:
  var '=' expression
    {
      *($$ = $2) << $1 >> $3;
      $$->filter = Token::Filter::OP_STM;
      d_printf("add assign statement: %i \n", $2->lineno);
    }
  | simple_expression
    {
      $$ = $1;
      d_printf("add simple_expression statement: %i \n", $1->lineno);
    }
  ;

//  var:  {*IDENTIFIER*}
//    {*IDENTIFIER*} [ .r-> {[} .r-> expression .r-> {]} ]
//
var:
  IDENTIFIER
  | IDENTIFIER '[' expression ']'
    {
      *($$ = $1) > $2 > $3 > $4;
      $$->filter = Token::Filter::EX_ARRAY_STM;
    }
  ;


//  simple_expression: relop
//    relop .child-> additive_expression
//    additive_expression .sibling-> additive_expression
//
simple_expression:
  additive_expression relop additive_expression
    {
      *($$ = $2) << $1 >> $3;
      $$->filter = Token::Filter::OP_STM;
    }
  | additive_expression
  ;


relop:
  LE_OP | '<' | '>' | GE_OP | EQ_OP | NE_OP
  ;

//  additive_expression:  addop
//    addop .child-> term
//    term .sibling-> additive_expression
//
additive_expression:
  additive_expression addop  term 
    {
      *($$ = $2) << $1 >> $3;
      $$->filter = Token::Filter::OP_STM;
    }
  | term
  ;


addop: '+' | '-'
  ;


// term: mulop
//   mulop .child-> factor
//   factor .sibling-> term
//
term:
  term  mulop factor 
    {
      *($$ = $2) << $1 >> $3;
      $$->filter = Token::Filter::OP_STM;
    }
  | factor
  ;


mulop:
  '*' 
  | '/' 
/*  
  | '%' 
    {
      // FIXME check value type
    }
*/    
  ;


factor:
  '(' expression ')' 
    {
      $$ = $2;
    }
  | var
  | call
  | INTEGER_VALUE
/*  | FLOAT_VALUE
  | CHAR_VALUE; */
  ;


//
// call: {*IDENTIIFER*}
//    {*IDENTIFIER*} .r-> {(}  .r-> {)}
//    {(} .child-> args
//  and add Filter::EX_FUNC_STM
//
call:
  IDENTIFIER '(' args ')' 
    {
      *( $$ = $1 ) > $2 > $4;
      *$2 << $3;
      $$->filter = Token::Filter::EX_FUNC_STM;
    }
  ;

args:
  arg_list
  | empty
    {
      $$ = NULL;
    }
  ;


// arg_list: a list linked with .sibling ptr.
//   expression .sibling-> expression .sibling-> ...
//
arg_list:
  expression ',' arg_list 
    {
      *( $$ = $1 ) >> $3;
    }
  | expression
  | error ','
    {
      $$ = NULL;
      yyerrok;
    }
  ;



%%
/////////////////  END OF BNF  /////////////////


/// Orignial error message: 
///   syntax error, unexpected IDENTIFIER, expecting IMPORT or PACKAGE
/// And translate this into:
///   syntax error, unexpected "???", expecting "import" or "package"
///
void yyerror(const char* s)
{
  std::string err(s);
  size_t unexptPos = err.find("unexpected");
  
  if (unexptPos != std::string::npos){
    fprintf(stderr, "[%d] syntax error, unexpected \"%s\"", lineno, yytext);
    size_t exptPos = err.find("expecting", unexptPos);
    
    if(exptPos != std::string::npos){
      size_t pos = exptPos + sizeof("expecting ")-1;
      size_t n = err.find(" or", exptPos) - pos;
      std::string exptIn(err, pos, n);
      std::map<std::string,const char*>::iterator it= tokenMap.find(exptIn);
      
      if(it != tokenMap.end())
        fprintf(stderr, ", expecting \"%s\"", it->second);
      else
        fprintf(stderr, ", expecting %s", exptIn.c_str());
      
      pos = exptPos;
      while( (pos = err.find("or ", pos)) != std::string::npos ){
        pos += sizeof("or ")-1;
        n = err.find(" or", pos) - pos;
       
        std::string exptIn(err, pos, n);
        it = tokenMap.find(exptIn);
        
        if(it != tokenMap.end())
          fprintf(stderr, " or \"%s\"", it->second);
        else
          fprintf(stderr, " or %s", exptIn.c_str());
      }
    }
    fputc('\n', stderr);
  }else{
    fprintf(stderr, "[%d] %s\n", lineno, s);
  }
}


/////////////////////////////////////////////////////////
///  @file parser-ext.cc {fake file}
///  This file aims to provid a layer between parser and 
///  parser driver.
////////////////////////////////////////////////////////
namespace psr{

const char* TokenRawMap[] =
{
    "AUTO",       "auto",
    "BREAK",      "break",
    "CASE",       "case",
    "CHAR",       "char",
    "CONST",      "const",
    "CONTINUE",   "continue",
    "DEFAULT",    "default",
    "DO",         "do",
    "DOUBLE",     "double",
    "ELSE",       "else",
    "ENUM",       "enum",
    "EXTERN",     "extern",
    "FLOAT",      "float",
    "FOR",        "for",
    "GOTO",       "goto",
    "IF",         "if",
    "INT",        "int",
    "LONG",       "long",
    "REGISTER",   "register",
    "RETURN",     "return",
    "SHORT",      "short",
    "SIGNED",     "signed",
    "SIZEOF",     "sizeof",
    "STATIC",     "static",
    "STRUCT",     "struct",
    "SWITCH",     "switch",
    "TYPEDEF",    "typedef",
    "UNION",      "union",
    "UNSIGNED",   "unsigned",
    "VOID",       "void",
    "VOLATILE",   "volatile",
    "WHILE",      "while",
    "SHIFT_RIGHT_ASSIGN",   ">>=",
    "SHIFT_LEFT_ASSIGN",    "<<=",
    "ADD_ASSIGN",   "&=",
    "SUB_ASSIGN",   "-=",
    "MUL_ASSIGN",   "*=",
    "DIV_ASSIGN",   "/=",
    "MOD_ASSIGN",   "%=",
    "AND_ASSIGN",   "&=",
    "XOR_ASSIGN",   "^=",
    "OR_ASSIGN",    "|=",
    "RR_OP",    ">>",
    "LL_OP",    "<<",
    "INC_OP",   "++",
    "DEC_OP",   "--",
    "PTR_OP",   "->",
    "AND_OP",   "&&",
    "OR_OP",    "||",
    "LE_OP",    "<=",
    "GE_OP",    ">=",
    "EQ_OP",    "==",
    "NE_OP",    "!=",
    "DOT_STAR",       ".*",
    "PTR_STAR",       "->*",
    "IDENTIFIER",     "identifier",
    "INTEGER_VALUE",  "integer value",
    "FLOAT_VALUE",    "float value",
    "CHAR_VALUE",     "char value",
    "WCHAR_VALUE",    "wchar value",
    "STRING_VALUE",   "string value",
    "$end",           "end",
    "$undefined",     "undefined",
};


// This map including tokens parsed by YACC and used in function
// yyerror() @YACC.y, helps to print a readable error infomation.
// 
std::map<std::string, const char*> tokenMap;


// Initialiazation class. Only run once.
//
static class _InitParser{
public:
  _InitParser(){
    d_printf("starting init tokenMap...\n");
    for(size_t i=0; i<sizeof TokenRawMap/sizeof(void*); i+=2 )
      tokenMap[TokenRawMap[i]] = TokenRawMap[i+1];
    d_printf("init tokenMap done.\n");
  }
  
  ~_InitParser(){ }
}_init;


// Add spaces.
//
static void _addSpace(int n, int inter, std::string& ss)
{
  while(n--){
    if(n % inter == inter-1)
      ss.push_back('|');
    else
      ss.push_back(' ');
  }
}


// toString recursive implementation.
//
static void _toString(Token* node, int n, std::string& ss)
{
  static const int inter = 3;   // interval width
  static char buf[32];

  if( node && node->token == 0 )
    node = node->child;

  if(node == NULL)
    return;


  ss += "\e[0m";
  _addSpace(n, inter, ss);

  buf[0] = 0;
  //sprintf(buf, ": %p->%p", node->sym, node->sym ? node->sym->outer : NULL);
  ss += "|--\e[1;32m" + node->yytext + buf;

  // try to stringify type specifier
  if ( (node->filter & Token::Filter::EX_DECL_STM) !=0 ){
    Token *type = getTypeSpec(node);
    if ( type ) ss += ": " +  type->yytext;
  }
    
  // try to stirngify function
  if( (node->filter & Token::Filter::EX_FUNC_STM) !=0 ){
    Token *params = getFuncParams(node);
    if( params ){
      ss += "  ( ... )\n";
      _toString(params, n + 3 * inter, ss);
    }else{
      ss += " (void) \n";
    }
    _toString( getFuncBody(node), n + inter, ss );
  } else if ( node->token == WHILE  ) {
  // try to stringify while loop
    ss += "  ( ... )  \n";
    _toString( getWhileCond(node), n + 3*inter, ss );
    _toString( getWhileBody(node), n + inter, ss );
  } else if ( node->token == IF ) {
    Token *_falseBody = getIfFalseBody(node);
    ss += "  ( ... ) \n";
    _toString( getIfCond(node), n + 3*inter, ss );
    _toString( getIfTrueBody(node), n + inter, ss );
    if( _falseBody ){
      ss += "\e[0m";
      _addSpace(n, inter, ss);
      ss += "|--\e[1;32melse\n";
      _toString( _falseBody, n + inter, ss );
    }
  } else  if ( (node->filter & Token::Filter::EX_ARRAY_STM) !=0 ){
    // try to stingify array info 
    ss += "[ ... ]\n";
    _toString(getArraySize(node), n + 3*inter, ss);
  } else {
    ss. push_back('\n');
  }

  _toString(node->child, n+inter, ss);
  n -= inter;
  _toString(node->sibling, n+inter, ss);
}


// Convert parser tree to printable stirng.
//
std::string treeToString(Token* root)
{
  std::string ss;
  _toString(root, 0, ss);
  return ss;
}


///////////////////////////////////////////////////////////////
// 
// Implementations help to get tree attribute easier.....
//
////////////////////////////////////////////////////

//  IDENTIFIER  .r->  '[' .r->  size  .r-> ']'
//            
Token* getArraySize( Token *tok )
{
  if( tok && tok->token == IDENTIFIER
      && (tok->filter & Token::Filter::EX_ARRAY_STM) != 0
      && tok->r && tok->r->token == '['
      && tok->r->r && tok->r->r->token != ']' ){
    return tok->r->r;
  }
  return NULL;
}


//  type_specifier IDENTIFIER
//  IDENTIFIER .l-> type_specifier
//
Token* getTypeSpec(Token *tok)
{
  if( tok && tok->token == IDENTIFIER
      && (tok->filter & Token::Filter::EX_DECL_STM) != 0 ) {
    return tok->l;
  }
  return NULL;
}

// type_specifier IDENTIFIER '(' params ')' ....
//  IDENTIFIER .r-> '(' .r-> ')'
//  '(' .child-> params
//  params linked with .sibling->
//
Token* getFuncParams(Token *tok)
{
  if( tok && tok->token == IDENTIFIER
      && (tok->filter & Token::Filter::EX_FUNC_STM) != 0
      && tok->r ) {
    return tok->r->child;
  }
  return NULL;
}

// type_specifier IDENTIFIER '(' params ')' compound_stmt
//  IDENTIFIER .r-> '(' .r-> ')'
//  ')' .child-> compound_stmt
//  
//  compound_stmt is a basic block.
//  
Token* getFuncBody(Token *tok)
{
  if( tok && tok->token == IDENTIFIER
      && (tok->filter & Token::Filter::EX_FUNC_STM) != 0
      && tok->r && tok->r->r ) {
    return tok->r->r->child;
  }
  return NULL;
}


//  IF '('  expression ')' ....
//  IF .r-> '(' .r-> ')'
//  '(' .child-> expression
//
Token* getIfCond(Token *tok)
{
  if( tok && tok->token == IF && tok->r )
    return tok->r->child;
  return NULL;
}

//  IF '('  expression ')' statement
//  IF .r-> '(' .r-> ')'
//     ')'  .child-> statement
//
Token* getIfTrueBody(Token *tok)
{
  if( tok && tok->token == IF && tok->r && tok->r->r )
    return tok->r->r->child;
  return NULL;
}

//  IF '('  expression ')' statement ELSE statement
//  IF .r-> '(' .r-> ')' .r-> ELSE
//     ELSE  .child-> statement
//
Token* getIfFalseBody(Token *tok)
{
  Token * _else;
  if( tok && tok->token == IF ){
    _else = tok->lrNth(3);
    return _else != NULL ? _else->child : NULL;
  }
  return NULL;
}

//
// WHILE '(' expression ')' ...
//   WHILE .r-> '(' .r-> ')'
//  '(' .child-> expression
//
Token* getWhileCond(Token *tok)
{
  if( tok && tok->token == WHILE && tok->r ){
    return tok->r->child;
  }
  return NULL;
}


//
// WHILE '(' expression ')' statement
//   WHILE .r-> '(' .r-> ')'
//  ')' .child-> statement
//
Token* getWhileBody(Token *tok)
{
  if( tok && tok->token == WHILE && tok->r && tok->r->r ){
    return tok->r->r->child;
  }
  return NULL;
}

//
//  RETURN [ expression ]  ';'
//  RETURN .r-> expression
// 
Token* getRet(Token *tok)
{
  if( tok && tok->token == RETURN )
    return tok->child;
  return NULL;
}


//   
//  expression:
//      var '=' expression
//      | simple_expression
//   '=' .child-> var .sibling-> expression
//
Token* getExprLVal( Token *tok )
{
  if( tok && tok->filter == Token::Filter::OP_STM )
    return tok->child;
  return NULL;
}

Token* getExprRVal( Token *tok )
{
  if( tok && tok->filter == Token::Filter::OP_STM && tok->child )
    return tok->child->sibling;
  return tok;
}


//
// IDENTIFIER '(' args ')'
//   IDENTIFIER .r->  '('  .r->  ')'
//   '(' .child-> args
//  args linked with .sibing->
//  !!! and this condition is same as getFuncParams
Token* getCallArgs(Token *tok)
{
  return getFuncParams(tok);
}


};  // END of namespace psr;



