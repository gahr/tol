/**
 * This file is part of Tcl One-Liner (tol).
 * See tol.h for the COPYRIGHT notice.
 */

#include <iostream>

#include "tol.h"

void
Tol::PrintVersion () const
{
    std::cout << "tol version " TOL_VERS " (" TOL_TIME ")" << std::endl;
}

void
Tol::PrintUsage () const
{
    PrintVersion ();
    std::cout << R"(
Copyright (C) 2014 Pietro Cerutti <gahr@gahr.ch>

Redistribution and use in source and binary forms, with or without
modification, are permitted under the 2-clause BSD License.

Usage: tol ?arg ...?

  arg         Evaluate the Tcl command inside 'arg'.
  -c          Evaluate the following argument if it is a complete
              Tcl command, or concatenate subsequent arguments up
              to the first one not preceeded by -c until they form
              a complete Tcl command.
  -e          Display some examples of usage
  -i          Ignore any errors from the following argument.
  -p          Print the result of the command in the following argument.
  -r          Reset the interpreter.
  -s var val  Assign the value 'val' to the variable 'var'.
  -v          Display this message.

)";
}

void
Tol::PrintExamples () const
{
    PrintVersion ();
    std::cout << R"(
Examples:

tol 'puts Hello'
>> Hello

tol -p 'expr {1+2}'
>> 3

tol 'set a 2' 'puts $a'
>> 2

tol 'set a 2' -r 'puts $a'
>> can't read "a": no such variable
>>    while executing
>> "puts $a"

tol 'puts before' 'set a 2' -r -i 'puts $a' 'puts after'
>> before
>> after

tol -s home $HOME 'puts "we are living in $home"'
>> we are living in /Users/gahr

tol -c 'set vars [dict create' \
    -c editor -c $EDITOR \
    -c pager -c $PAGER ']' \
    'foreach {k v} $vars {\
        puts "$k => $v"\
     }'
>> editor => vim
>> pager => less
)";
}
