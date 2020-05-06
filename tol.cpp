/**
 * This file is part of Tcl One-Liner (tol).
 * See tol.h for the COPYRIGHT notice.
 */

#include <algorithm>
#include <cstring>
#include <iostream>
#include <ostream>

#include "tol.h"

Tol::Tol (int& argc, char ** argv)
    : m_interp { nullptr },
      m_flags  { 0 }
{
    Tcl_FindExecutable(argv[0]);
    CreateInterp();
    std::copy(argv+1, argv+argc, std::back_inserter(m_args));
}

Tol::~Tol ()
{
    DeleteInterp();
}

void
Tol::CreateInterp()
{
    m_interp = Tcl_CreateInterp();
    Tcl_Init(m_interp);
}

void
Tol::DeleteInterp()
{
    if (m_interp != nullptr) {
        Tcl_DeleteInterp(m_interp);
        m_interp = nullptr;
    }
}

void
Tol::ResetState()
{
    ResetFlags();
    DeleteInterp();
    CreateInterp();
}

int Tol::Evaluate(const std::string& command)
{
    Tcl_ResetResult(m_interp);

    int res { Tcl_Eval(m_interp, command.c_str()) };
    std::string out { Tcl_GetStringFromObj(Tcl_GetObjResult(m_interp), NULL) };

    if (res != TCL_OK) {
       if (!IsFlagged(Flag::IgnoreErrors)) {
        std::cerr << Tcl_GetVar(m_interp, "::errorInfo", 0) << std::endl;
        return res;
       }
       res = TCL_OK;
    }

    /*
     * Avoid printing blank lines by skipping over empty results
     */
    if (IsFlagged(Flag::PrintResult) && !out.empty()) {
        std::cout << out << std::endl;
    }

    return res;
}

int
Tol::Run()
{
    if (m_args.size() == 0) {
        return 0;
    }

    std::string cmd;

    /*
     * Unfortunately I can't iterate over arguments with a more
     * convenient range-for because I have to manually increment
     * the iterator to skip over options.
     */
    for (auto i = m_args.cbegin(); i != m_args.cend(); ++i) {
        const auto& s = *i;

        if (s.empty()) {
            continue;
        }

        if (s.length() > 1 && s[0] == '-') {
            bool handled = true;

            /*
             * Handle options
             */
            switch (s[1]) {

                case 'a':
                    SetFlag(Flag::AccumulateCommands);
                    break;

                case 'c':
                    SetFlag(Flag::Continue);
                    break;

                case 'e':
                    PrintExamples();
                    break;

                case 'i':
                    SetFlag(Flag::IgnoreErrors);
                    break;

                case 'p':
                    SetFlag(Flag::PrintResult);
                    break;

                case 'r':
                    ResetState();
                    break;

                case 's':
                    if (i + 2 >= m_args.end()) {
                        std::cerr << "Not enough arguments given." << std::endl;
                        return 1;
                    }
                    Tcl_SetVar(m_interp, (i+1)->c_str(), (i+2)->c_str(), 0);
                    i += 2;
                    break;

                case 'h':
                case 'v':
                    PrintUsage();
                    break;

                default:
                    handled = false;
                    break;
                }

            if (handled)
                continue;
        }

        /*
         * Options are done. Assume the current argument is part of a
         * command to be evaluated.
         */
        cmd.append(s);


        /*
         * Skip evaluation of the command if Continue flag was used and
         * the command is not yet complete.
         */
        if (IsFlagged(Flag::Continue) && !Tcl_CommandComplete(cmd.c_str())) {
            cmd.append(" ");
            ResetFlags();
            continue;
        }


        /*
         * Skip evaluation of the command if AccumulateCommands flag was used.
         */
        if (IsFlagged(Flag::AccumulateCommands)) {
            cmd.append(" ");
            continue;
        }


        /*
         * Finally evaluate the command. Only report errors if the
         * IgnoreErrors flag wasn't used.
         */
        auto res = Evaluate(cmd);
        if (res != TCL_OK)
        {
            return 1;
        }

        ResetFlags();
        cmd.clear();
    }

    /*
     * Evaluate any remaining commands.
     */
    if (!cmd.empty()) {
        return Evaluate(cmd);
    }

    return 0;
}
