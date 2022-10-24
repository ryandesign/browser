// SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
//
// SPDX-License-Identifier: MIT

#include "about_window.h"

#include "ResourceConstants.h"

about_window::about_window()
    : base_window(r_WIND_about)
{
    show();
}

about_window::~about_window()
{
}

bool about_window::should_close()
{
    hide();
    return false;
}
