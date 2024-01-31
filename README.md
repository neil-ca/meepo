# My own Lisp on c

## Use it
```sh
apt install libedit-dev
make
./parsing
lisp> + 5 92
```

## What is a Programming Language
Is very similar to a real language. There is a structure behind it, and some rules
which dictate what is, and isn't, a valid thing to say. When we read an write natural
language, we are unconsiously learning these rules.

To write a programming language such as our Lisp we are going to need to understand
grammars. For reading in the user input we need to write a grammar which describes it.
Then we can use it along with our user input,to decide if the input is valid. We
can also use it to build a structured internal representation, which will make the
job of understanding it, and then evaluating it, performing the computations encoded
within, much easier.

## Parser Combinators
`mpc` is a Parser Combinator library that help us to build programs that understand
and process particular languages.

## Coding Grammars
> An Adjective is either "wow", "many", "so" or "such".
> A noun is either "lisp", "language", "c", "book" or "build".
> A phrase is an adjective followed by a noun.
> A Doge is zero or more phrases.
### Polish Notation
```sh
1 + 2 + 6 is + 1 2 6
6 + (2 * 9) is +6 (* 2 9)
```

## Evaluation
### Trees
An Abstract Syntax Tree represents the structure of the program based on the input
entered by the user in our case with a REPL.
### Recursion
Is a good use case to evaluate a tree with childs of another trees with recursion.
```c
int number_of_nodes(mpc_ast_t* t) {
  if (t->children_num == 0) { return 1; }
  if (t->children_num >= 1) {
    int total = 1;
    for (int i = 0; i < t->children_num; i++) {
      total = total + number_of_nodes(t->children[i]);
    }
    return total;
  }
  return 0;
}
```
