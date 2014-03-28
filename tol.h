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

#ifndef TOL_H
#define TOL_H

#include <tcl.h>

#include <string>
#include <utility>
#include <vector>

#include "tolConfig.h"

class Tol {
    using StringList = std::vector<std::string>;

    public:
        Tol(int& argc, char ** argv);
        ~Tol();
        int Run();

    private:
        enum Flag {
            Continue = 0,
            IgnoreErrors,
            PrintResult,
        };

        void SetFlag(Flag f)
        {
            m_flags |= (1<<f);
        }

        void UnsetFlag(Flag f)
        {
            m_flags &= ~(1<<f);
        }

        void ResetFlags()
        {
            m_flags = 0;
        }

        bool IsFlagged(Flag f) const
        {
            return m_flags&1<<f;
        }

        void CreateInterp();
        void DeleteInterp();
        void ResetState();

        std::pair<int, std::string>
        Evaluate(const std::string& command);

        void PrintVersion() const;
        void PrintUsage() const;
        void PrintExamples() const;

    private:
        Tcl_Interp * m_interp;
        StringList m_args;
        unsigned m_flags;
};

#endif /* !TOL_H */
