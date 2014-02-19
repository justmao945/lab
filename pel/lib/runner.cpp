
#include "ast.h"
#include "parser.h"
#include "runner.h"
#include "scanner.h"

struct ELCtx{

};

struct ELStmt{

};


ELCtx* pel_CreateCtx( int type )
{
  switch(type){
  case EL_CTX_MEM:
    break;
    
  case EL_CTX_SQLITE:
    break;
    
  case EL_CTX_MYSQL:
    break;
    
  default:
    break;
  }

  return 0;
}

void pel_DestroyCtx( ELCtx* ctx )
{

}

ELStmt* pel_Prepare( ELCtx* ctx, const char* stmt )
{
  return 0;
}

ELStmt* pel_PrepareFile( ELCtx* ctx, const char* filename )
{
  return 0;
}

void pel_Finalize( ELStmt* stmt )
{

}

int pel_Step( ELStmt* stmt )
{
  return 0;
}
