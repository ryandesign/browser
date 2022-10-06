// SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
//
// SPDX-License-Identifier: MIT

#include "oserr_exception.h"

oserr_exception::oserr_exception(OSErr err)
    : m_err(err)
{
}

oserr_exception::~oserr_exception()
{
}

OSErr oserr_exception::err() const
{
    return m_err;
}
