#ifndef EVAL_H
#define EVAL_H
#endif
#include "mpc.h"

typedef struct lval {
  int type;
  long num;
  char *err;
  char *sym;
  int count;
  struct lval **cell;
} lval;

lval *lval_eval(lval *v);
lval *lval_eval_sexpr(lval *v);
lval *lval_read(mpc_ast_t *t);
void lval_expr_print(lval *v, char open, char close);
void lval_print(lval *v);
void lval_println(lval *v);
void lval_del(lval *v);
