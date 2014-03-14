/** 
 * Copyright (c) 2014, Pietro Cerutti <gahr@gahr.ch> All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <tcl.h>
#include <string.h>

#include "tolConfig.h"

#if (TCL_MAJOR_VERSION == 8) && (TCL_MINOR_VERSION < 6)
#define Tcl_GetErrorLine(interp) ((interp)->errorLine)
#endif

int main (int argc, char **argv)
{
    Tcl_Interp *interp;
    const char *res;
    int ret;
    char verbose = 0;

    if (argc < 2) {
        return 0;
    }

    Tcl_FindExecutable (argv[0]);
    interp = Tcl_CreateInterp ();

    for (int i=1; i<argc; i++) {

        /*
         * Handle options
         */

        /* -p : print the result */
        if (!strcmp (argv[i], "-p")) {
            verbose = 1;
            continue;
        }

        /* -r : recreate the interpreter */
        if (!strcmp (argv[i], "-r")) {
            Tcl_DeleteInterp (interp);
            interp = Tcl_CreateInterp ();
            continue;
        }

        /* -s var val : assign var to val */
        if (!strcmp (argv[i], "-s")) {
            if (i+2 >= argc) {
                fprintf (stderr, "Not enough arguments given.\n");
                Tcl_DeleteInterp (interp);
                return 1;
            }
            Tcl_SetVar (interp, argv[i+1], argv[i+2], 0);
            i+=2;
            continue;
        }

        /* -v : print version info (and keep going)*/
        if (!strcmp (argv[i], "-v")) {
            puts (
                  "tol version " TOL_VERS " (" TOL_TIME ")\n\n"
                  "Copyright (C) 2014 Pietro Cerutti <gahr@gahr.ch>\n\n"
                  "Redistribution and use in source and binary forms, with or without\n"
                  "modification, are permitted under the 2-clause BSD License.\n\n"
                  "Usage: tol ?arg ...?\n\n"
                  "        arg          Evaluate the Tcl script inside 'arg'.\n"
                  "        -p           Print the result of the script in the following argument.\n"
                  "        -r           Reset the interpreter.\n"
                  "        -s var val   Assign the value 'val' to the variable 'var'.\n"
                  "        -v           Display this message\n"
                 );
            continue;
        }


        /*
         * Not an option, eval it
         */

        ret = Tcl_Eval (interp, argv[i]);
        res = Tcl_GetStringResult (interp);
        if (ret != TCL_OK) {
            printf ("Arg %d, line %d: %s\n", i, Tcl_GetErrorLine (interp), res);
        } else {
            if (verbose) {
                puts (res);
            }
        }

        verbose = 0;
    }
    Tcl_DeleteInterp (interp);

    return ret == TCL_OK ? 0 : 1;
}
