// SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
//
// SPDX-License-Identifier: MIT

#ifndef ABOUT_WINDOW_H
#define ABOUT_WINDOW_H

#include "base_control.h"
#include "base_window.h"
#include "root_control.h"

class about_window : public base_window
{
    public:
        about_window();
        ~about_window();
        bool should_close() override;

    private:
        root_control m_root_control;
        base_control m_text;
};

#endif
