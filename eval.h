#include "lval.h"

lval *lval_eval(lval *v);
lval *lval_eval_sexpr(lval *v);
lval *builtin(lval *a, char *func);
