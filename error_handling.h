#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

#include "mpc.h"

typedef struct {
  int type;
  long num;
  int err;
} lval;

lval eval(mpc_ast_t *t);
void lval_println(lval v);

#endif
