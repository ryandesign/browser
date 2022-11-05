// SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
//
// SPDX-License-Identifier: MIT

#include "browser_window.h"

#include "ResourceConstants.h"
#include "base_control.h"
#include "helpers.h"
#include "root_control.h"

browser_window::browser_window()
    : base_window(r_WIND_browser),
      m_first_tab(m_window),
      m_root_control(m_window),
      m_header(r_CNTL_header, m_window),
      m_back_button(r_CNTL_back_button, m_window),
      m_forward_button(r_CNTL_forward_button, m_window),
      m_home_button(r_CNTL_home_button, m_window),
      m_reload_button(r_CNTL_reload_button, m_window),
      m_address_bar(r_CNTL_address_bar, m_window),
      m_web_view(r_CNTL_web_view, m_window),
      m_horizontal_scroll_bar(r_CNTL_horizontal_scroll_bar, m_window),
      m_vertical_scroll_bar(r_CNTL_vertical_scroll_bar, m_window)
{
}

browser_window::~browser_window()
{
}

int16_t browser_window::get_minimum_height()
{
    return 128;
}

int16_t browser_window::get_minimum_width()
{
    return 128;
}

void browser_window::did_resize(int16_t dx, int16_t dy, int16_t part)
{
    ControlHandle control = reinterpret_cast<ControlHandle>(m_window.controlList);
    while (control)
    {
        base_control *control_obj = base_control::get_from_control(control);
        if (control_obj)
            control_obj->window_did_resize(dx, dy);
        control = (**control).nextControl;
    }
}

void browser_window::update(EventRecord const& event)
{
    base_window::update(event);
    m_first_tab.update(event);
}
