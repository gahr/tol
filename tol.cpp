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

#include <algorithm>
#include <cstring>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "tolConfig.h"

#if (TCL_MAJOR_VERSION == 8) && (TCL_MINOR_VERSION < 6)
#define Tcl_GetErrorLine(interp) ((interp)->errorLine)
#endif

class Tol {

    using StringList = std::vector<std::string>;

    public:
        Tol (int& argc, char** argv);
        ~Tol ();
        int Run ();

    private:

        enum Flag {
            IgnoreErrors = 0,
            PrintResult,
        };
        void CreateInterp ();
        void DeleteInterp ();
        void ResetInterp  ();

        std::pair<int, std::string> Evaluate (const std::string &script);

        void ResetFlags   ();
        void SetFlag      (Flag f);
        void UnsetFlag    (Flag f);
        bool IsFlagged    (Flag f) const;

        void PrintVersion () const;

    private:
        Tcl_Interp* m_interp;
        StringList  m_args;
        unsigned    m_flags;
};

Tol::Tol (int& argc, char** argv)
    : m_interp {nullptr},
      m_flags  {0}
{
    Tcl_FindExecutable (argv[0]);

    std::copy (argv+1, argv+argc, std::back_inserter (m_args));
}

Tol::~Tol ()
{
    DeleteInterp ();
}

int
Tol::Run ()
{
    if (m_args.size() < 1) {
        return 0;
    }

    CreateInterp ();

    int ret;
    const char *res;

    for (auto i = m_args.cbegin(); i != m_args.cend(); ++i) {
        auto &s = *i;

        /*
         * Handle options
         */
        if (s == "-i") {
            SetFlag (Flag::IgnoreErrors);
            continue;
        }

        if (s == "-p") {
            SetFlag (Flag::PrintResult);
            continue;
        }

        if (s == "-r") {
            ResetInterp ();
            continue;
        }

        if (s == "-s") {
            if (i+2 >= m_args.end()) {
                std::cerr << "Not enough arguments given." << std::endl;
                return 1;
            }
            Tcl_SetVar (m_interp, (i+1)->c_str(), (i+2)->c_str(), 0);
            i += 2;
            continue;
        }

        if (s == "-v") {
            PrintVersion ();
            continue;
        }

        /*
         * Done with options, do whatever we need to do.
         */
        auto res = Evaluate (s);
        if (std::get<0> (res) != TCL_OK && !IsFlagged (Flag::IgnoreErrors)) {
            std::cerr << Tcl_GetVar (m_interp, "::errorInfo", 0) << std::endl;
            return 1;
        }

        if (IsFlagged (Flag::PrintResult) && !std::get<1> (res).empty ()) {
            std::cout << std::get<1> (res) << std::endl;
        }

        ResetFlags ();

        //std::cout << "Flags are now: " << m_flags << std::endl;
    }

    return 0;
}

void
Tol::CreateInterp ()
{
    m_interp = Tcl_CreateInterp ();
    Tcl_Init (m_interp);
}

void
Tol::DeleteInterp ()
{
    if (m_interp != nullptr) {
        Tcl_DeleteInterp (m_interp);
        m_interp = nullptr;
    }
}

void
Tol::ResetInterp ()
{
    DeleteInterp ();
    CreateInterp ();
}

void
Tol::ResetFlags ()
{
    m_flags = 0;
}

void
Tol::SetFlag (Flag f)
{
    m_flags |= 1<<f;
}

void
Tol::UnsetFlag (Flag f)
{
    m_flags &= ~(1<<f);
}

bool
Tol::IsFlagged (Flag f) const
{
    return m_flags & 1<<f;
}

void
Tol::PrintVersion () const
{
    std::cout <<
      "tol version " TOL_VERS " (" TOL_TIME ")\n\n"
      "Copyright (C) 2014 Pietro Cerutti <gahr@gahr.ch>\n\n"
      "Redistribution and use in source and binary forms, with or without\n"
      "modification, are permitted under the 2-clause BSD License.\n\n"
      "Usage: tol ?arg ...?\n\n"
      "  arg         Evaluate the Tcl script inside 'arg'.\n"
      "  -i          Ignore any errors from the following argument.\n"
      "  -p          Print the result of the script in the following argument.\n"
      "  -r          Reset the interpreter.\n"
      "  -s var val  Assign the value 'val' to the variable 'var'.\n"
      "  -v          Display this message\n";
}

std::pair<int, std::string>
Tol::Evaluate (const std::string &script)
{

    Tcl_ResetResult (m_interp);
    return std::make_pair (
            Tcl_Eval (m_interp, script.c_str()), 
            Tcl_GetStringFromObj (Tcl_GetObjResult (m_interp), NULL)
            );
}

int
main (int argc, char** argv)
{
    Tol t (argc, argv);
    return t.Run ();
}
