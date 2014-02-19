////////////////////////////////////////////////////////////////
///  @file parser-ext.h 
///  This file is *not* created by bison. This file aims to 
///  provide a media layer between parser and parser driver.
///  Functions implemented in @file parser.y.
/////////////////////////////////////////////////////////////////

#ifndef PARSER_EXT_H_
#define PARSER_EXT_H_

#include "psrdrv.h"

namespace psr{

/// List raw enumerators.
extern const char* TokenRawMap[];

/// Faster to find enumerator names.
extern std::map<std::string, const char*> tokenMap;

/// Root of parser tree.
extern Token* troot;

/// Convert a parser tree to string.
/// @param root Root of a parser tree.
/// @return Readable tree in string format.
std::string treeToString(Token* root);


/// Get array size of an identifier token, which has EX_ARRAY_STM filter.
/// The tree showed as below:
/// <pre>   IDENTIFIER  .r->  '[' .r->  size  .r-> ']' </pre>
/// @param tok Token to get array size.
/// @return Size token or tree if has array size attribute or NULL failed.
///         e.g. INTEGER_VALUE or an expression: var = expression.
Token* getArraySize( Token *tok );


/// Get type specifier of an identifier token.
/// The tree showed as below:
/// <pre>  IDENTIFIER .l->  type_specifier </pre>
/// @param tok Token to get type from.
/// @return Type (a token or list) if has type specifier(s) or NULL failed.
///         .e.g. INT, VOID
Token* getTypeSpec(Token *tok);


/// Get function parameters of an identifier token, 
/// which has EX_FUNC_STM filter.
/// The tree showed as below: 
/// <pre>
///   type_specifier IDENTIFIER '(' params ')' ....
///   IDENTIFIER .r-> '(' .r-> ')'
///   '(' .child-> params
///   params are linked with .sibling ptr.  
/// </pre>
/// @tok Token to get params from.
/// @return Params linked with .sibing-> ptr or NULL if failed.
Token* getFuncParams(Token *tok);


/// Get function body of an identifier token, 
/// which has EX_FUNC_STM filter.
/// The tree showed as below:
/// <pre>
///  type_specifier IDENTIFIER '(' params ')' compound_stmt
///  IDENTIFIER .r-> '(' .r-> ')'
///  ')' .child-> compound_stmt
///  and compound_stmt is a basic block. 
///  </pre>
///  @param tok Token to get function body from.
///  @return A basic block ( all type list linked ) or NULL if failed.
///         .e.g.  
Token* getFuncBody(Token *tok);


/// Get condition token in IF statement of an identifier token. 
/// The tree showed as below:
/// <pre>
///  IF '('  expression ')' ....
///  IF .r-> '(' .r-> ')'
///  '(' .child-> expression
/// </pre>
/// @param  tok Token to get condition from.
/// @return An expression statement, see @func getExprLVal @func getExprRVal.
///         e.g. if ( (a = b[1] = 1) != val )...
Token* getIfCond(Token *tok);


/// Get the *true* body block in IF statement of an identifier token.
/// The tree showed as below: 
/// <pre>
///  IF '('  expression ')' statement
///  IF .r-> '(' .r-> ')'
///     ')'  .child-> statement
/// </pre>
///  @param  tok Token to get body in *true* condition 
///  @return A statement or a basic block, for more info see @file parser.y.
///           or NULL if failed.
Token* getIfTrueBody(Token *tok);


/// Get the *false* body block in IF statement of an identifier token.
/// The tree showed as below: 
/// <pre>
///  IF '('  expression ')' statement ELSE statement
///  IF .r-> '(' .r-> ')' .r-> ELSE
///     ELSE  .child-> statement
/// </pre>
/// @param  tok Token to get body in *false* condition.
/// @return A statement or a basic block, for more info see @file parser.y.
///           or NULL if failed.
Token* getIfFalseBody(Token *tok);


/// Get the condition in WHILE statement.
/// The tree showed as below: 
/// <pre>
/// WHILE '(' expression ')' ...
///   WHILE .r-> '(' .r-> ')'
///  '(' .child-> expression
/// </pre>
/// @param tok Token to get loop condition  from.
/// @return An expression statement, see @func getExprLVal @func getExprRVal.
///         e.g. while ( (a = b[1] = 1) != val )...
Token* getWhileCond(Token *tok);


/// Get the body in WHILE statement.
/// The tree showed as below: 
/// <pre>
/// WHILE '(' expression ')' statement
///   WHILE .r-> '(' .r-> ')'
///  ')' .child-> statement
/// </pre>
/// @param tok Token to get loop body from.
/// @return A statement or a basic block, for more info see @file parser.y.
///           or NULL if failed.
Token* getWhileBody(Token *tok);


/// Get the expression in RETURN statement.
/// The tree showed as below: 
/// <pre>
///  RETURN [ expression ]  ';'
///  RETURN .r-> expression
/// </pre>
/// @param tok Token to get return statement from.
/// @return NULL if has no expression or an expression.
///         see @func getExprLVal @func getExprRVal.
Token* getRet(Token *tok);


/// Get the left expression value of the token, and the token must have
/// OP_STM filter, or will get NULL.
/// The tree showed as below: 
/// <pre>
///  expression:
///      var '=' expression
///     | simple_expression
///   '=' .child-> var .sibling-> expression
/// </pre>
/// @param tok Token to get left value of exression.
/// @return  Left value or NULL if failed.
Token* getExprLVal( Token *tok );


/// Get the right expression value of the token, and the token must have
/// OP_STM fileter, or will get NULL.
/// The tree showed as below: 
/// <pre>
///  expression:
///      var '=' expression
///     | simple_expression
///   '=' .child-> var .sibling-> expression
/// </pre>
/// @param tok Token to get right value of exression.
/// @return  Right value or NULL if failed.
Token* getExprRVal( Token *tok );


/// Get args of a function caller token, which has EX_FUNC_STM filter.
/// The tree showed as below: 
/// <pre>
/// IDENTIFIER '(' args ')'
///   IDENTIFIER .r->  '('  .r->  ')'
///   '(' .child-> args
///  args linked with .sibing->
/// </pre>
/// This condition is same as @func getFuncParams
/// @tok Token to get params from.
/// @return Params linked with .sibing-> ptr or NULL if failed.
/// 
Token* getCallArgs(Token *tok);


}; // end of namespace psr


#endif
