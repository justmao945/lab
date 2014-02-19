#include <string.h>
#include "utils.h"

Token* cp(Token* dest, const Token* src)
{
  return memcpy(dest, src, sizeof(Token));
}

// add_xx
static Token* add_ii(Token* dest, const Token* a, const Token* b)
{ 
  dest->ival = a->ival + b->ival; 
  dest->type = INT;
  return dest;
}

static Token* add_if(Token* dest, const Token* a, const Token* b)
{ 
  dest->fval = a->ival + b->fval;
  dest->type = FLOAT;
  return dest;
}

static Token* add_fi(Token* dest, const Token* a, const Token* b)
{ 
  dest->fval = a->fval + b->ival; 
  dest->type = FLOAT;
  return dest;
}

static Token* add_ff(Token* dest, const Token* a, const Token* b)
{ 
  dest->fval = a->fval + b->fval; 
  dest->type = FLOAT;
  return dest;
}


// sub_xx
static Token* sub_ii(Token* dest, const Token* a, const Token* b)
{ 
  dest->ival = a->ival - b->ival; 
  dest->type = INT;
  return dest;
}

static Token* sub_if(Token* dest, const Token* a, const Token* b)
{ 
  dest->fval = a->ival - b->fval;
  dest->type = FLOAT;
  return dest;
}

static Token* sub_fi(Token* dest, const Token* a, const Token* b)
{ 
  dest->fval = a->fval - b->ival; 
  dest->type = FLOAT;
  return dest;
}

static Token* sub_ff(Token* dest, const Token* a, const Token* b)
{ 
  dest->fval = a->fval - b->fval; 
  dest->type = FLOAT;
  return dest;
}

// mul_xx
static Token* mul_ii(Token* dest, const Token* a, const Token* b)
{ 
  dest->ival = a->ival * b->ival; 
  dest->type = INT;
  return dest;
}

static Token* mul_if(Token* dest, const Token* a, const Token* b)
{ 
  dest->fval = a->ival * b->fval;
  dest->type = FLOAT;
  return dest;
}

static Token* mul_fi(Token* dest, const Token* a, const Token* b)
{ 
  dest->fval = a->fval * b->ival; 
  dest->type = FLOAT;
  return dest;
}

static Token* mul_ff(Token* dest, const Token* a, const Token* b)
{ 
  dest->fval = a->fval * b->fval; 
  dest->type = FLOAT;
  return dest;
}

static Token* neg_i(Token* dest, const Token* x, const Token* _)
{
  dest->ival = - x->ival;
  dest->type = x->type;
  return dest;
}

static Token* neg_f(Token* dest, const Token* x, const Token* _)
{
  dest->fval = - x->fval;
  dest->type = x->type;
  return dest;
}

typedef Token* (*Func)(Token*, const Token*, const Token*);

static const Func add_func[2][2] = {
  {add_ii, add_if},
  {add_fi, add_ff},
};


static const Func sub_func[2][2] = {
  {sub_ii, sub_if},
  {sub_fi, sub_ff},
};

static const Func mul_func[2][2] = {
  {mul_ii, mul_if},
  {mul_fi, mul_ff},
};

static const Func neg_func[2] = {
  neg_i, neg_f,
};

Token* add(Token* dest, const Token* a, const Token* b)
{
  return add_func[a->type][b->type](dest, a, b);
}

Token* sub(Token* dest, const Token* a, const Token* b)
{
  return sub_func[a->type][b->type](dest, a, b);
}

Token* mul(Token* dest, const Token* a, const Token* b)
{
  return mul_func[a->type][b->type](dest, a, b);
}

Token* neg(Token* dest, const Token* x)
{
  return neg_func[x->type](dest, x, NULL);
}

void zero(Token* x)
{
  memset(x, 0, sizeof(Token));
}

void print(const Token* tok)
{
  switch (tok->type){
    case INT:
      printf("%lli", tok->ival);
      break;
    case FLOAT:
      printf("%g", tok->fval);
      break;
    default:
      break;
  }
}
