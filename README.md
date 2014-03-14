Tcl One Liner is a simple tool that allows to execute simple Tcl
one-liners on the console.

    $ ./tol 'puts Hello'
    Hello

The -p switches makes tol print the result of the following expression.

    $ ./tol -p 'expr {1+2}'
    3

State is maintained from the evaluation of an argument to the next one...

    $ ./tol 'set a 2' 'puts $a'
    2

... unless the -r switch is used. This effectively resets the interpreter.

    $ ./tol 'set a 2' -r 'puts $a'
    can't read "a": no such variable
        while executing
    "puts $a"


Errors in an arguments can be ignored with the -i switch.

    $ ./tol 'puts before' 'set a 2' -r -i 'puts $a' 'puts after'
    before
    after


The -s var val can be used to set $var to the value val inside the interpreter.

    $ ./tol -s home $HOME 'puts "we are living in $home"'
    we are living in /Users/pietrocerutti



