
#include<cassert>
#include<cstring>
#include"ast.h"
#include"new.h"
#include"parser.h"
#include"rtti.h"
#include"scanner.h"
#include"token.h"

using namespace pel;

//--------------------------------------------------
//    Parser and ParserImpl implementation
//--------------------------------------------------

namespace pel{

// private class
class ParserImpl{
private:
  Scanner *scanner;
  ASTree  *tree;

  std::size_t tokix; // index of token to get

  ParserImpl(const ParserImpl&);
  ParserImpl& operator=(const ParserImpl&);

public:
  ParserImpl(){
    this->scanner = 0;
    this->tree = 0;
    this->tokix = 0;
  }
  
  ParserImpl(ASTree *tree, Scanner *s){
    this->tree = tree;
    this->scanner = s;
    this->tokix = 0;
  }

  ~ParserImpl(){}

  template<class T>
  T* NewNode(){
    return tree->newNode<T>();
  }

  Token* NextToken(){
    assert(tree);
    assert(scanner);

    if( tokix < tree->tokens.Size() ){
      return tree->tokens[ tokix ++ ];
    }else{
      Token *tok = tree->newToken(); // pushed to tree->tokens.
      scanner->NextToken(tok);
      tokix++; // set one beyond the new token.
      return tok;
    }
  }

  // step back n steps
  void StepBack(std::size_t n){
    if( n > tokix ){
      tokix = 0;
    }else{
      tokix -= n;
    }
  }

  // To all AST nodes, if not matched return 0, or matched.
  ASTPathExpr* PathExpr(){
    Vector<const ASTPathElemExpr*> elems;
    Token *tok = 0;
    ASTPathElemExpr *elem = 0;

    do{
      elem = PathElemExpr();
      if( elem ){
        elems.Push( elem );
        tok = NextToken();
      }else{
        assert(0); // error
        break;
      }
    }while( tok->Rep() == TOK_dot ); // '.'

    StepBack(1);
    ASTPathExpr *path = NewNode<ASTPathExpr>();
    path->elems.Move(elems);
    return path;
  }

  ASTPathElemExpr* PathElemExpr(){
    void *t = 0;

    if( ASTConstExpr *p = ConstExpr() ){
      t = p; // 1. ConstExpr
    }
    else if( ASTFuncExpr *p = FuncExpr() ){
      t = p; // 2. FuncExpr
    }
    else if( ASTRecurPathExpr *p = RecurPathExpr() ){
      t = p; // 3. RecurPathExpr
    }
    else if( ASTModelElemExpr *p = ModelElemExpr() ){
      t = p; // 4. (final) ModelElemExpr
    }
    else{
      // error
    }

    ASTPathElemExpr *elem = 0;
    if( t ){
      elem = NewNode<ASTPathElemExpr>();
      elem->sel.constt = static_cast<ASTConstExpr*>(t);
    }else{
      return 0; // not matched
    }

    if( ASTIndexOp *p = IndexOp() ){
      elem->index = p;
    }

    return elem;
  }

  ASTModelElemExpr* ModelElemExpr(){
    Token *tok = NextToken();
    ASTModelElemExpr *model = 0;
    
    if ( tok->Rep() == TOK_ident ){ // ident
      model = NewNode<ASTModelElemExpr>();
      model->ident = tok;
    }else{
      StepBack(1); // not ident, go back
    }

    if( ASTypeCastExpr *p = TypeCastExpr() ){
      if( model == 0 ){
        model = NewNode<ASTModelElemExpr>();
      }
      model->type = p;
    }

    if( ASTNodeLabelExpr *p = NodeLabelExpr() ){
      if( model == 0 ){
        model = NewNode<ASTModelElemExpr>();
      }
      model->label = p;
    }

    if( ASTNodeCastExpr *p = NodeCastExpr() ){
      if( model == 0 ){
        model = NewNode<ASTModelElemExpr>();
      }
      model->node = p;
    }
     
    return model;
  }

  ASTypeCastExpr* TypeCastExpr(){
    ASTypeCastExpr *type = 0;

    Token *tok = NextToken();
    if( tok->Rep() == TOK_colon ) { // ':'
      tok = NextToken();
      if( tok->Rep() == TOK_ident ){ // matched
        type = NewNode<ASTypeCastExpr>();
        type->ident = tok;
      }else{
        assert(0); //error
      }
    }else{ // not ':', go back
      StepBack(1);
    }
    return type;
  }


  ASTNodeLabelExpr* NodeLabelExpr (){
    ASTNodeLabelExpr *label = 0;
    Token *tok = NextToken();

    if( tok->Rep() == TOK_lcur ) { // '{'
      Token *ident = NextToken();
      if( ident->Rep() == TOK_ident ){ // ident
        tok = NextToken();
        if( tok->Rep() == TOK_rcur ){ // '}'  matched
          label = NewNode<ASTNodeLabelExpr>();
          label->ident = ident;
        }else{
          assert(0); // error
        }
      }else{
        assert(0); // error
      }
    }else{ // not '{', go back
      StepBack(1);
    }

    return label;
  }

  ASTNodeCastExpr* NodeCastExpr(){
    Vector<const ASTPathDecl*> paths;
    ASTCondExpr *cond = 0;
    ASTNodeCastExpr *node = 0;
    Token *tok = NextToken();

    if( tok->Rep() == TOK_lbra ) { // '['
      tok = NextToken();
      if( tok->Rep() == TOK_lcur ){ // '{'
        ASTPathDecl *decl = 0;
        do{
          decl = PathDecl(); // PathDecl
          if( decl ){
            paths.Push( decl );
            tok = NextToken();
          }else{
            assert(0); // error
            break;
          }
        }while( tok->Rep() == TOK_comma ); // ','

        if( tok->Rep() != TOK_rcur ){ // '}'
          assert(0); // error
        }
      }else{
        StepBack(1); // not '{', go back
      }

      if( (cond = CondExpr()) ){ // matched CondExpr
        // 
      }else{ // NodeCastExpr not matched. !!!
        // FIXME: tokens consumed by PathDecl ???
        StepBack(1); // not cond, put back '[' and return
        return 0;
      }
      
      tok = NextToken();
      if( tok->Rep() == TOK_rbra ){ // ']'  matched NodeCastExpr
        node = NewNode<ASTNodeCastExpr>();
        node->paths.Move(paths);
        node->cond = cond;
      }else{
        assert(0); // error
      }
    }else{ // go back
      StepBack(1);
    }

    return node;
  }

  ASTPathDecl* PathDecl(){
    ASTPathDecl *decl = 0;
    Token *tok = NextToken(), *ident = 0;

    if( tok->Rep() == TOK_ident ){ // ident
      ident = tok;

      tok = NextToken();
      if( tok->Rep() == TOK_asi ){ // '='
        ASTPathExpr *path = 0;
        path = PathExpr();  // path
        if( path ){
          decl = NewNode<ASTPathDecl>();
          decl->ident = ident;
          decl->path = path;
        }else{
          assert(0); // error
        }
      }else{
        assert(0); // error
      }
    }else{
      // not 'ident', go back
      StepBack(1);
    }

    return decl;
  }

  ASTCondExpr* CondExpr(){
    if( ASTOrExpr *or_ = OrExpr() ){
      ASTCondExpr *cond = NewNode<ASTCondExpr>();
      cond->or_ = or_;
      return cond;
    }else{
      return 0;
    }
  }

  ASTOrExpr* OrExpr(){
    ASTAndExpr *and_ = 0;
    Token *tok = 0;
    Vector<const ASTAndExpr*> ands;

    do{
      and_ = AndExpr();
      if ( and_ ){
        ands.Push( and_ );
        tok = NextToken();
      }else{
        assert(0); // error
        break;
      }
    }while( tok->Rep() == TOK_or ); // || match

    StepBack(1); // not ||, go back

    if( ! ands.Empty() ){
      ASTOrExpr *or_ = NewNode<ASTOrExpr>();
      or_->ands.Move(ands);
      return or_;
    }else{
      return 0;
    }
  }

  ASTAndExpr* AndExpr(){
    ASTCondTerm *term = 0;
    Token *tok = 0;
    Vector<const ASTCondTerm*> terms;

    do{
      term = CondTerm();
      if ( term ){
        terms.Push( term );
        tok = NextToken();
      }else{
        assert(0); // error
        break;
      }
    }while( tok->Rep() == TOK_and ); // && match

    StepBack(1); // not &&, go back

    if( ! terms.Empty() ){
      ASTAndExpr *and_ = NewNode<ASTAndExpr>();
      and_->terms.Move(terms);
      return and_;
    }else{
      return 0;
    }
  }

  ASTCondTerm* CondTerm(){
    Token *tok = NextToken();
    ASTCondExpr* cond = 0;
    ASTBaseTerm* base = 0;

    if( tok->Rep() == TOK_lpar ){ // '('
      cond = CondExpr(); // CondExpr
      if( cond ){
        //
        tok = NextToken();
        if( tok->Rep() == TOK_rpar ) { // ')'
          ASTCondTerm *term = NewNode<ASTCondTerm>();
          term->sel.cond = cond;
          return term;
        }else{
          assert(0); // error
        }
      }else{
        assert(0); // error
      }

    }else{
      StepBack(1); // not '(', go back
      if( (base = BaseTerm()) ){  // BaseTerm
        ASTCondTerm *term = NewNode<ASTCondTerm>();
        term->sel.base = base;
        return term;
      }
    }

    return 0;
  }

  ASTBaseTerm* BaseTerm(){
    ASTPathExpr *lhs = 0, *rhs = 0;
    ASTRelOp *op = 0;

    if( (lhs = PathExpr()) ){ // left PathExpr
      if( (op = RelOp()) ){ // RelOp
        if( (rhs = PathExpr()) ){ // right PathExpr
          ASTBaseTerm *base = NewNode<ASTBaseTerm>();
          base->lpath = lhs;
          base->op = op;
          base->rpath = rhs;
          return base;
        }else{
          assert(0); // error
        }
      }else{
        assert(0); // error
      }
    }else{
      assert(0); // error
    }
    return 0;
  }

  ASTRelOp* RelOp(){
    Token *tok = NextToken();
    ASTRelOp *op = 0;

    switch( tok->Rep() ){
    case TOK_lt:
    case TOK_gt:
    case TOK_le:
    case TOK_ge:
    case TOK_eq:
    case TOK_ne:
    case TOK_like:
      op = NewNode<ASTRelOp>();
      op->op = tok;
      break;
    default:
      assert(0); // error?
      StepBack(1); // or go back??
      break;
    }
    assert(op);
    return op;
  }

  ASTFuncExpr* FuncExpr(){
    Token *tok =  NextToken(), *ident;

    if( tok->Rep() == TOK_ident ){ // ident
      ident = tok;
      tok = NextToken();
      if( tok->Rep() == TOK_lpar ){  // '('
        Vector<const ASTPathExpr*> params;
        ASTPathExpr *p = 0;
        do{
          p = PathExpr();
          if( p ){
            params.Push(p);
          }else{
            // no param
          }
          tok = NextToken();
        }while( tok->Rep() == TOK_comma ); // ','
        
        if( tok->Rep() == TOK_rpar ){ // ')' matched
          ASTFuncExpr *f = NewNode<ASTFuncExpr>();
          f->paths.Move(params);
          return f;
        }else{
          assert(0); // error
        }
      }else{
        StepBack(2); // go back
      }
    }else{
      StepBack(1); // go back
    }

    return 0;
  }

  ASTRecurPathExpr* RecurPathExpr(){
    Token *tok = NextToken();
    if( tok->Rep() == TOK_lt ){ // '<'
      ASTPathExpr *path = PathExpr(); // path
      if( path ){
        tok = NextToken();
        if( tok->Rep() == TOK_gt ) { // '>'
          tok = NextToken();
          if( tok->Rep() == TOK_plus ){ // first '+'
            tok = NextToken();
            ASTRecurPathExpr *rec = NewNode<ASTRecurPathExpr>();
            rec->path = path;
            if( tok->Rep() == TOK_plus ){ // second ''+
              rec->plusplus = true;
            }else{
              StepBack(1); // go back
            }
            return rec;
          }else{
            assert(0); // error
          }
        }else{
          assert(0); // error
        }
      }else{
        assert(0); // error 
      }

    }else{
      StepBack(1); // go back
    }
    return 0;
  }

  ASTConstExpr* ConstExpr(){
    void *t = 0;

    // Meta is what ???
    //if( ASTMetaVarExpr *p = MetaVarExpr() ){
    //  t = p; // 1. MetaVarExpr
    //}else
    
    if( ASTVarExpr *p = VarExpr() ){
      t = p; // 2. VarExpr
    }else if( ASTComplexLit *p = ComplexLit() ){
      t = p; // 3. ComplexLit
    }else{
      return 0; // not matched
    }

    if( t ){
      ASTConstExpr *constt = NewNode<ASTConstExpr>();
      constt->sel.var = static_cast<ASTVarExpr*>(t);
      return constt;
    }else{
      return 0; // not matched
    }
  }

  ASTComplexLit* ComplexLit(){
    void *t = 0;

    if( ASTValLit *p = ValLit() ){
      t = p;
    }else if( ASTListLit *p = ListLit() ){
      t = p;
    //}else if( ASTInterLit *p = InterLit() ){
      //t = p;
    }else{
      return 0;
    }

    if( t ){
      ASTComplexLit *lit = NewNode<ASTComplexLit>();
      lit->sel.val = static_cast<ASTValLit*>(t);
      return lit;
    }else{
      return 0;
    }
  }

  ASTValLit* ValLit(){
    Token *tok = NextToken();

    switch( tok->Rep() ){
    case TOK_int8:
    case TOK_int10: case TOK_int:
    case TOK_int16:
      break;

    case TOK_float:
      break;

    case TOK_string:
      break;

    case TOK_null:
      break;

    default: // go back
      StepBack(1);
      return 0; // not matched
    }

    ASTValLit *lit = NewNode<ASTValLit>();
    lit->val = tok;
    return lit; // matched
  }

  ASTListLit* ListLit(){
    Token *tok = NextToken();
    
    if( tok->Rep() == TOK_lbra ){ // '['
      ASTValLit *val = 0;
      Vector<const ASTValLit*> vals;
      do{
        if( (val = ValLit()) ){
          vals.Push( val );
          tok = NextToken(); // next is ','
        }else{
          assert(0); // error
          break;
        }
      }while(tok->Rep() == TOK_comma); // ','

      if( tok->Rep() == TOK_rbra ){ // ']'
        ASTListLit *lit = NewNode<ASTListLit>();
        lit->vals.Move(vals);
        return lit;
      }else{
        assert(0); // error
      }

    }else{
      // not matched
      StepBack(1);
      return 0;
    }

    return 0;
  }

  ASTInterLit* InterLit(){
    // not implemented
    return 0;
  }

  ASTolerLit* TolerLit(){
    // not implemented
    return 0;
  }

  ASTVarExpr* VarExpr(){
    Token *tok = NextToken(), *ident = 0;

    if( tok->Rep() == TOK_dollar ){ // '$' matched
      tok = NextToken();
      if( tok->Rep() == TOK_lcur ){ // '{'
        tok = NextToken();
        if( tok->Rep() == TOK_ident ){ // 'ident'
          ident = tok; //save ident
          tok = NextToken();
          if( tok->Rep() == TOK_rcur ){ // '}'
            ASTVarExpr *var = NewNode<ASTVarExpr>();
            var->ident = ident;
            return var;
          }else{
            assert(0); // error
          }
        }else{
          assert(0); //error
        }
      }else{
        assert(0); // error
      }

    }else{
      StepBack(1);
    }
    return 0;
  }

  ASTMetaVarExpr* MetaVarExpr(){
    assert(0); // not implemented
    return 0;
  }

  ASTIndexOp* IndexOp(){
    Token *tok = NextToken(), *intg = 0;

    if( tok->Rep() == TOK_lbra ){ // '[' matched
      tok = NextToken();
      bool isi = false;
      if( tok->Rep() == TOK_int8 ){   // integer
        isi = true;

      }else if( tok->Rep() == TOK_int10 ){
        isi = true;

      }else if( tok->Rep() == TOK_int16 ){
        isi = true;

      }else{
        assert(0); // error
      }

      if( isi ){
        intg = tok;
        tok = NextToken();
        if( tok->Rep() == TOK_rbra ){ // ']'
          ASTIndexOp *op = NewNode<ASTIndexOp>();
          op->intv = intg;
          return op; // all matched
        }else{
          assert(0); // error
        }
      }
    }else{
      // 
      StepBack(1);
    }

    return 0;
  }
};

} // ~pel

//-------------------------
//    Parser impl
//-------------------------
Parser::Parser()
{
  this->ateof = 0;
  this->ateofd = 0;
  this->scanner = 0;
}

Parser::~Parser()
{
}

Parser& Parser::AtEof(bool(*cb)(Parser&, void*), void* data)
{
  assert(cb);
  this->ateof = cb;
  this->ateofd = data;
  return *this;
}

Scanner* Parser::From()const {
  return scanner;
}

Parser& Parser::From(Scanner *s)
{
  assert(s);
  this->scanner = s;
  return *this;
}


const ASTree* Parser::Parse(ASTree* tree)
{
  assert(this->scanner);
  if(tree == 0){
    tree = new ASTree;
  }

  ParserImpl impl(tree, this->scanner);

  // start recursive parse
  tree->root = impl.PathExpr();

  return tree;
}


//-------------------------
//    Test
//-------------------------
#include"test.h"

namespace pel{

void TestParser()
{
  TestBegin("Parser");

  ASTree t;
  Scanner s;
  Parser p;

  p.From( &s.From("a.b.c{dd}.z[{g=ff.z} a > 1 ]") ).Parse(&t);
  t.Print();
  TestEnd();
}

} // ~pel
