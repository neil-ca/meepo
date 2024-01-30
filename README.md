# My own Lisp on c

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

