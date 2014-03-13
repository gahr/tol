Tcl One Liner is a simple tool that allows to execute simple Tcl
one-liners on the console:

$ ./tol 'puts Hello' Hello

$ ./tol -v 'expr {1+2}' 3

The -v switch makes tol print result of the following expression. State
is maintained from the evaluation of an argument to the next one

$ ./tol 'set a 2' 'puts $a'
2

unless the -r switch is used. This effectively resets the interpreter.

$ ./tol 'set a 2' -r 'puts $a'
Arg 3, line 1: can't read "a": no such variable.
