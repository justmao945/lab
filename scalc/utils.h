#ifndef CALC_H
#define CALC_H

#include <stdio.h>

enum {
  INT = 0,
  FLOAT = 1,
};


typedef struct {
  union{
    long long int ival;
    double fval;
  };
  int type;
} Token;


Token* cp(Token* dest, const Token* src);
Token* add(Token* dest, const Token* a, const Token* b);
Token* sub(Token* dest, const Token* a, const Token* b);
Token* mul(Token* dest, const Token* a, const Token* b);
Token* neg(Token* dest, const Token* x);

void zero(Token* tok);
void print(const Token* tok);

#endif
