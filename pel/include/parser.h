/*
 * File: parser.h
 * Provides syntax tree for analysis.
 * Paser uses the top-down recursive-descent parsing algorithm,
 * same as many other compilers.
 */

/*
 * EBNF of path expression language:
 *
 * PathExpr := PathElemExpr {  "."  PathElemExpr } ;
 *
 * PathElemExpr :=(ModelElemExpr |
 *                 FuncExpr |
 *                 RecurPathExpr |
 *                 ConstExpr) [ IndexOp ] ;
 *
 * ModelElemExpr := [Ident] [TypeCastExpr] [NodeLabelExpr] [NodeCastExpr] ;
 *
 * TypeCastExpr  := ":" Ident ;
 *
 * NodeLabelExpr := "{" Ident "}" ;
 *
 * NodeCastExpr  := "[" [ "{" PathDecl { "," PathDecl } "}" ] CondExpr "]" ;
 * 
 * PathDecl := Ident "=" PathExpr ;
 *
 * CondExpr := OrExpr ;
 * 
 * OrExpr   := AndExpr { "||" AndExpr } ;
 * 
 * AndExpr  := CondTerm { "&&" CondTerm } ;
 *
 * CondTerm := BaseTerm | "(" CondExpr ")" ;
 *
 * BaseTerm := PathExpr RelOp PathExpr ;
 *
 * RelOp    := "<" | ">" | "<=" | ">=" | "==" | "!=" | "=~" ;
 * 
 * FuncExpr := Ident "(" PathExpr { "," PathExpr } ")" ;
 * 
 * RecurPathExpr := "<" PathExpr ">" "+" ["+"] ;
 * 
 * ConstExpr := ComplexLit |
 *              VarExpr |
 *              MetaVarExpr;
 *
 * ComplexLit := ValLit | ListLit | InterLit | TolerLit ;
 * 
 * ValLit  := Integer | Float | String | Null ;
 *
 * ListLit := "[" ValLit { "," ValLit } "]" ;
 *
 * InterLit := "[" ValLit "." "." ValLit "]" ;
 *
 * TolerLit := "[" ValLit ":" ValLit "]" ; // ?
 *
 * VarExpr := "$" "{" Ident "}" ;   // set Var outside of pel
 *
 * MetaVarExpr := "$" "$" "{" Ident "}" ; // ?
 *
 * IndexOp := "[" Interger "]" ;
 *
 */


#ifndef PEL_PARSER_H
#define PEL_PARSER_H


#include"vector.h"

namespace pel{

class Scanner; // scanner.h
class Token;   // scanner.h
class Parser;
class ParserImpl; // parser.cpp
class ASTree; // ast.h


//-----------------------
//   Paser
//-----------------------
class Parser{
public:
  Parser();
  ~Parser();

  // Register a callback called when scanner returns a token represents EOF.
  // @cb Callback, called if not 0.
  //     self is this parser and data is same as @data.
  //     If cb returns true then parser will ignore the EOF flag and
  //     continue to call NextToken of scanner to resume parsing,
  //     or false to stop after EOF.
  // @data Data passed to cb.
  Parser& AtEof( bool(*cb)(Parser& self, void *data), void* data=0);

  Scanner* From()const;

  // Parse from a scanner, please keep scanner alive during parsing.
  // @return self.
  Parser& From(Scanner *s);

  // Main function used to create an abstract syntax tree.
  // Notice that can only parse *once* on a scanner.
  // If @tree is 0, don't forget to free returned AST,
  // as the parser doesn't manager constructed tree.
  // @tree If not 0, the new tree will be placed into this address.
  // @return Constructed AST.
  const ASTree* Parse(ASTree *tree=0);

private:
  bool(*ateof)(Parser&, void*); // callback at eof
  void *ateofd; // data of ateof

  Scanner *scanner;  // scanner

  friend class ParserImpl;

private:
  Parser(const Parser&); // copy is disabled
  Parser& operator=(const Parser&);
};

} // ~pel

#endif // ~ PEL_PARSER_H
