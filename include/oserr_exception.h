// SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
//
// SPDX-License-Identifier: MIT

#ifndef OSERR_EXCEPTION_H
#define OSERR_EXCEPTION_H

#include <MacTypes.h>
#include <exception>

class oserr_exception : public std::exception
{
    public:
        oserr_exception(OSErr err);
        ~oserr_exception();
        OSErr err() const;

    protected:
        OSErr m_err;
};

#endif
