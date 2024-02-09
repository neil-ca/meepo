// Lisp Value: Make errors a possible result of evaluating an expression.
#include "eval.h"

// Evaluate
// Variable Evaluation
// Our environment returns a copy of the value
// we need to remember to delete the input symbol lval.
lval *lval_eval(lenv *e, lval *v) {
  if (v->type == LVAL_SYM) {
    lval *x = lenv_get(e, v);
    lval_del(v);
    return x;
  }
  if (v->type == LVAL_SEXPR) {
    return lval_eval_sexpr(e, v);
  }
  // All other lval types remain the same
  return v;
}

lval *builtin_op(lenv *e, lval *a, char *op) {
  // Ensure all arguments are numbers
  for (int i = 0; i < a->count; i++) {
    LASSERT(a, a->cell[i]->type == LVAL_NUM,
            "Cannot operate on non-number. Got: %s, Expected %s",
            ltype_name(a->cell[i]->type), ltype_name(LVAL_NUM));
  }

  // Pop the first element
  lval *x = lval_pop(a, 0);

  // If no arguments and sub then perform unary negation
  if ((strcmp(op, "-") == 0) && a->count == 0) {
    x->num = -x->num;
  }

  // While there are still elements remaining
  while (a->count > 0) {
    // Pop the next element
    lval *y = lval_pop(a, 0);
    if (strcmp(op, "+") == 0) {
      x->num += y->num;
    }
    if (strcmp(op, "-") == 0) {
      x->num -= y->num;
    }
    if (strcmp(op, "*") == 0) {
      x->num *= y->num;
    }
    if (strcmp(op, "/") == 0) {
      if (y->num == 0) {
        lval_del(x);
        lval_del(y);
        x = lval_err("Division By Zero!");
        break;
      }
      x->num /= y->num;
    }
    if (strcmp(op, "^") == 0) {
      x->num = pow(x->num, y->num);
    }
    if (strcmp(op, "%") == 0) {
      x->num %= y->num;
    }

    lval_del(y);
  }
  lval_del(a);
  return x;
}

/* Now that our evaluation relies on the new function type we need to make
 * sure we can register all of our builtin functions with the environment
 * before we start the interactive prompt.
 * */
lval *builtin_add(lenv *e, lval *a) { return builtin_op(e, a, "+"); }

lval *builtin_sub(lenv *e, lval *a) { return builtin_op(e, a, "-"); }

lval *builtin_mul(lenv *e, lval *a) { return builtin_op(e, a, "*"); }

lval *builtin_div(lenv *e, lval *a) { return builtin_op(e, a, "/"); }

lval *builtin_mod(lenv *e, lval *a) { return builtin_op(e, a, "%"); }

lval *builtin_exp(lenv *e, lval *a) { return builtin_op(e, a, "^"); }

lval *lval_eval_sexpr(lenv *e, lval *v) {
  // Evaluate children
  for (int i = 0; i < v->count; i++) {
    v->cell[i] = lval_eval(e, v->cell[i]);
  }

  // Error checking
  for (int i = 0; i < v->count; i++) {
    if (v->cell[i]->type == LVAL_ERR) {
      return lval_take(v, i);
    }
  }

  // Empty expression
  if (v->count == 0) {
    return v;
  }

  // Single expression
  if (v->count == 1) {
    return lval_take(v, 0);
  }

  // Ensure first element is a function after evaluation
  lval *f = lval_pop(v, 0);
  if (f->type != LVAL_FUN) {
    lval_del(f);
    lval_del(v);
    return lval_err("First element is not a function!");
  }

  // If so call function to get result
  lval *result = f->fun(e, v);
  lval_del(f);
  return result;
}

lval *builtin_head(lenv *e, lval *a) {
  LASSERT(a, a->count == 1,
          "Function 'head' passed to many arguments. Got %i, Expected %i.",
          a->count, 1);
  LASSERT(a, a->cell[0]->type == LVAL_QEXPR,
          "Function 'head' passed incorrect type for argument 0. Got %s, "
          "Expected %s.",
          ltype_name(a->cell[0]->type), ltype_name(LVAL_QEXPR));
  LASSERT(a, a->cell[0]->count != 0, "Function 'head' passed {}!");

  // Otherwise take first argument
  lval *v = lval_take(a, 0);

  // Delete all elements that are not head and return
  while (v->count > 1) {
    lval_del(lval_pop(v, 1));
  }
  return v;
}

lval *builtin_tail(lenv *e, lval *a) {
  LASSERT(a, a->count == 1, "Function 'tail' passed too many arguments!");

  LASSERT(a, a->cell[0]->type == LVAL_QEXPR,
          "Function 'tail' passed incorrect type!");

  LASSERT(a, a->cell[0]->count != 0, "Function 'tail' passed {}!");

  lval *v = lval_take(a, 0);

  lval_del(lval_pop(v, 0));
  return v;
}

lval *builtin_list(lenv *e, lval *a) {
  a->type = LVAL_QEXPR;
  return a;
}

lval *builtin_eval(lenv *e, lval *a) {
  LASSERT(a, a->count == 1, "Function 'eval' passed to many arguments!");
  LASSERT(a, a->cell[0]->type == LVAL_QEXPR,
          "Function 'eval' passed incorrect type!");

  lval *x = lval_take(a, 0);
  x->type = LVAL_SEXPR;
  return lval_eval(e, x);
}

lval *lval_join(lval *x, lval *y) {
  // For each cell in 'y' add it to 'x'
  while (y->count) {
    x = lval_add(x, lval_pop(y, 0));
  }
  lval_del(y);
  return x;
}

lval *builtin_join(lenv *e, lval *a) {
  for (int i = 0; i < a->count; i++) {
    LASSERT(a, a->cell[i]->type == LVAL_QEXPR,
            "Function 'join' passed incorrect type.");
  }

  lval *x = lval_pop(a, 0);

  while (a->count) {
    x = lval_join(x, lval_pop(a, 0));
  }

  lval_del(a);
  return x;
}

lval *builtin(lenv *e, lval *a, char *func) {
  if (strcmp("list", func) == 0) {
    return builtin_list(e, a);
  }
  if (strcmp("head", func) == 0) {
    return builtin_head(e, a);
  }
  if (strcmp("tail", func) == 0) {
    return builtin_tail(e, a);
  }
  if (strcmp("join", func) == 0) {
    return builtin_join(e, a);
  }
  if (strcmp("eval", func) == 0) {
    return builtin_eval(e, a);
  }
  if (strstr("+-/*^%", func)) {
    return builtin_op(e, a, func);
  }
  lval_del(a);
  return lval_err("Unknown Function!");
}

// Define a function
lval *builtin_def(lenv *e, lval *a) {
  LASSERT(a, a->cell[0]->type == LVAL_QEXPR,
          "Function 'def' passed incorrect type!");

  // First argument is symbol list
  lval *syms = a->cell[0];

  // Ensure all elements of first list are symbols
  for (int i = 0; i < syms->count; i++) {
    LASSERT(a, syms->cell[i]->type == LVAL_SYM,
            "Function 'def' cannot define non-symbol");
  }

  // Check correct number of symbols and values
  LASSERT(a, syms->count == a->count - 1,
          "Function 'def' cannot define incorrect number of values to symbols");

  // Assign copies of values to symbols
  for (int i = 0; i < syms->count; i++) {
    lenv_put(e, syms->cell[i], a->cell[i + 1]);
  }

  lval_del(a);
  return lval_sexpr();
}
