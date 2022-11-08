// SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
//
// SPDX-License-Identifier: MIT

#include "browser_window.h"

#include <Resources.h>
#include "ResourceConstants.h"
#include "helpers.h"

browser_window::browser_window()
    : base_window(r_WIND_browser),
      m_selected_document(std::make_shared<browser_document>()),
      m_root_control(m_window),
      m_header(r_CNTL_header, m_window),
      m_back_button(r_CNTL_back_button, m_window),
      m_forward_button(r_CNTL_forward_button, m_window),
      m_home_button(r_CNTL_home_button, m_window),
      m_reload_button(r_CNTL_reload_button, m_window),
      m_address_bar(r_CNTL_address_bar, m_window),
      m_browser(r_CNTL_browser, m_window),
      m_horizontal_scroll_bar(r_CNTL_horizontal_scroll_bar, m_window),
      m_vertical_scroll_bar(r_CNTL_vertical_scroll_bar, m_window)
{
    m_header.embed(m_back_button);
    m_header.embed(m_forward_button);
    m_header.embed(m_home_button);
    m_header.embed(m_reload_button);
    m_header.embed(m_address_bar);
#ifdef USE_LITEHTML
    Handle html = Get1Resource('TEXT', r_TEXT_html);
    if (!html)
        throw std::bad_alloc();
    int16_t saved_state = HGetState(html);
    HLock(html);
    m_selected_document->set_html(*html, m_browser);
    HSetState(html, saved_state);
    m_browser.set_document(m_selected_document);
    render_if_needed_and_update_scrollbars();
#endif
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
    base_window::did_resize(dx, dy, part);
    render_if_needed_and_update_scrollbars();
}

void browser_window::render_if_needed_and_update_scrollbars()
{
    Rect rect = m_browser.get_rect();
    int16_t width = rect_width(rect);
    m_selected_document->render_if_needed(width);
    Point dimensions = m_selected_document->get_dimensions();
    dimensions.h -= width;
    if (dimensions.h < 0) dimensions.h = 0;
    dimensions.v -= rect_height(rect);
    if (dimensions.v < 0) dimensions.v = 0;
    m_vertical_scroll_bar.set_maximum(dimensions.v);
    m_horizontal_scroll_bar.set_maximum(dimensions.h);
}
