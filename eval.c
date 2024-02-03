// Lisp Value: Make errors a possible result of evaluating an expression.
#include "eval.h"

// Evaluate
lval *lval_eval(lval *v) {
  if (v->type == LVAL_SEXPR) {
    return lval_eval_sexpr(v);
  }
  // All other lval types remain the same
  return v;
}
