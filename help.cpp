/**
 * This file is part of Tcl One-Liner (tol).
 * See tol.h for the COPYRIGHT notice.
 */

#include <iostream>

#include "tol.h"

void
Tol::PrintVersion () const
{
    std::cout <<
      "tol version " TOL_VERS " (" TOL_TIME ")\n\n"
      "Copyright (C) 2014 Pietro Cerutti <gahr@gahr.ch>\n\n"
      "Redistribution and use in source and binary forms, with or without\n"
      "modification, are permitted under the 2-clause BSD License.\n\n"
      "Usage: tol ?arg ...?\n\n"
      "  arg         Evaluate the Tcl command inside 'arg'.\n"
      "  -c          Evaluate the following argument if it is a complete\n"
      "              Tcl command, or concatenate subsequent arguments up\n"
      "              to the first one not preceeded by -c until they form\n"
      "              a complete Tcl command.\n"
      "  -e          Display some examples of usage\n"
      "  -i          Ignore any errors from the following argument.\n"
      "  -p          Print the result of the command in the following argument.\n"
      "  -r          Reset the interpreter.\n"
      "  -s var val  Assign the value 'val' to the variable 'var'.\n"
      "  -v          Display this message\n\n";
}

void
Tol::PrintExamples () const
{
    std::cout << 
      "Examples:\n\n"
      "./tol 'puts Hello'\n"
      "Hello\n\n"
      "./tol -p 'expr {1+2}'\n"
      "3\n\n"
      "./tol 'set a 2' 'puts $a'\n"
      "2\n\n"
      "./tol 'set a 2' -r 'puts $a'\n"
      "can't read \"a\": no such variable\n"
      "    while executing\n"
      "\"puts $a\"\n\n"
      "./tol 'puts before' 'set a 2' -r -i 'puts $a' 'puts after'\n"
      "before\n"
      "after\n\n"
      "./tol -s home $HOME 'puts \"we are living in $home\"'\n"
      "we are living in /Users/gahr\n\n"
      "./tol -c 'set vars [dict create' \\\n"
      "    -c editor -c $EDITOR \\\n"
      "    -c pager -c $PAGER ']' \\\n"
      "    'foreach {k v} $vars {puts \"$k => $v\"}'\n"
      "editor => vim\n"
      "pager => less\n"
      "\n";
}
