#include "eval.h"

// Quoted expressions {}, istead of Macros

// A pointer to a new empty Qexpr lval
lval *lval_qexpr(void) {
  lval *v = malloc(sizeof(lval));
  v->type = LVAL_QEXPR;
  v->count = 0;
  v->cell = NULL;
  return v;
}
