/**
 * This file is part of Tcl One-Liner (tol).
 * See tol.h for the COPYRIGHT notice.
 */

#include "tol.h"

int
main(int argc, char** argv)
{
    Tol t{ argc, argv };
    return t.Run();
}
