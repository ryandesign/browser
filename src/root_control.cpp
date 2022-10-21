// SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
//
// SPDX-License-Identifier: MIT

#include "root_control.h"

#include <new>

root_control::root_control(WindowRecord& window) : base_control()
{
    OSErr err = CreateRootControl(reinterpret_cast<WindowPtr>(&window), &m_control);
    if (noErr != err)
        throw std::bad_alloc();
    init();
}

root_control::~root_control()
{
}
