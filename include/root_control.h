// SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
//
// SPDX-License-Identifier: MIT

#ifndef ROOT_CONTROL_H
#define ROOT_CONTROL_H

#include "base_control.h"

class root_control final : public base_control
{
    public:
        root_control(WindowRecord& window);
        ~root_control();
};

#endif
