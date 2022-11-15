##################################################
               MATHEMATICAL PARSER
##################################################

This program is a parser that is able to calculate
the value of a string of mathematical expressions
provided in the form of standard textual input.

In case of an error in the input the program
can report the type of the error and its exact
location within the input.

The use of parentheses are supported, and correct
order of execution is ensured by converting the
input from infix to postfix notation during
execution.

The following operators are available:
+    addition
-    subtraction
*    multiplication
/    division
%    modulo
^    power

The following functions are available:
sin    sine
cos    cosine
tan    tangent
ln     natural logarithm
log    decimal logarithm
abs    absolute value
fac    factorial

For trigonometric functions prepend 'a' for
arcus and append 'd' for degree.

Use 'pi' for an accurate value of
the constant.

---------
  BUILD
---------

This program was written entirely in C11 using
only the standard library, so no external
resources are required to compile it.

A system with CMake and a C compiler is required.

- To build normally:

make

- To run unit tests:

make test

- To delete all compiled results:

make clean

---------
  USAGE
---------

The most straightforward way to use the program
is to pass it the character string containing
the expression. For example:

parser "(2 + 3) * 4"

The -p flag can be used to instead print the
expression in postfix notation. This mode
does not calculate a result.

When started without arguments, a basic
line by line interpreter mode is available.
