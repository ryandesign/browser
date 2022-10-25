// SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
//
// SPDX-License-Identifier: MIT

#ifndef BROWSER_WINDOW_H
#define BROWSER_WINDOW_H

#include <Controls.h>

#include "base_control.h"
#include "base_window.h"
#include "browser_tab.h"
#include "root_control.h"

class browser_window : public base_window
{
    public:
        browser_window();
        ~browser_window();
        int16_t get_minimum_height() override;
        int16_t get_minimum_width() override;
        void did_resize(int16_t dx, int16_t dy) override;
        void did_zoom(int16_t dx, int16_t dy, int16_t in_or_out) override;
        void update(EventRecord const& event) override;

    private:
        browser_tab m_first_tab;
        root_control m_root_control;
        base_control m_header;
        base_control m_address_bar;
        base_control m_web_view;
        base_control m_back_button;
        base_control m_forward_button;
        base_control m_home_button;
        base_control m_reload_button;
        base_control m_horizontal_scroll_bar;
        base_control m_vertical_scroll_bar;
};

#endif