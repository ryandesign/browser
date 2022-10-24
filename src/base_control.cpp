// SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
//
// SPDX-License-Identifier: MIT

#include "base_control.h"

#include <new>

#include "ResourceConstants.h"

base_control::base_control(int16_t resource_id, WindowRecord& window)
{
    m_control = GetNewControl(resource_id, reinterpret_cast<WindowPtr>(&window));
    if (nullptr == m_control)
        throw std::bad_alloc();
    int32_t flags = GetControlReference(m_control);
    m_move_horizontally = flags & k_move_horizontally;
    m_move_vertically = flags & k_move_vertically;
    m_resize_horizontally = flags & k_resize_horizontally;
    m_resize_vertically = flags & k_resize_vertically;
    init();
}

/*
base_control::base_control(ControlHandle control)
    : m_control(control)
{
    init();
}
*/

base_control::base_control()
    : m_control(nullptr),
      m_move_horizontally(0),
      m_move_vertically(0),
      m_resize_horizontally(0),
      m_resize_vertically(0)
{
}

base_control *base_control::get_from_control(const ControlHandle control)
{
    base_control *control_obj = nullptr;
    if (control && *control && GetControlReference(control))
        control_obj = base_control::get_from_control(**control);
    return control_obj;
}

base_control *base_control::get_from_control(ControlRecord const& control)
{
    base_control *control_obj = nullptr;
    if (control.contrlRfCon)
        control_obj = reinterpret_cast<base_control *>(control.contrlRfCon);
    return control_obj;
}

void base_control::init()
{
    SetControlReference(m_control, reinterpret_cast<int32_t>(this));
}

base_control::~base_control()
{
    DisposeControl(m_control);
}

bool base_control::is_visible()
{
    return (**m_control).contrlVis;
}

void base_control::show()
{
    ShowControl(m_control);
}

void base_control::hide()
{
    HideControl(m_control);
}

void base_control::window_did_resize(int16_t dx, int16_t dy)
{
    bool was_visible = is_visible();
    if (was_visible)
        hide();
    if (m_resize_horizontally)
        (**m_control).contrlRect.right += dx;
    if (m_resize_vertically)
        (**m_control).contrlRect.bottom += dy;
    if (!m_move_horizontally)
        dx = 0;
    if (!m_move_vertically)
        dy = 0;
    if (dx || dy)
        OffsetRect(&(**m_control).contrlRect, dx, dy);
    if (was_visible)
        show();
}
