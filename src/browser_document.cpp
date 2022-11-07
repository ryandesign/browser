// SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
//
// SPDX-License-Identifier: MIT

#include "browser_document.h"

#include <Quickdraw.h>

#include "helpers.h"

#ifdef USE_LITEHTML
litehtml::context browser_document::s_context;
#endif

browser_document::browser_document()
    : m_scroll({0, 0})
{
}

browser_document::~browser_document()
{
}

void browser_document::set_default_css(const char *const css)
{
#ifdef USE_LITEHTML
    s_context.load_master_stylesheet(css);
#endif
}

void browser_document::set_html(const char *const html, browser_control& control)
{
#ifdef USE_LITEHTML
    m_lite_document = litehtml::document::createFromUTF8(html, &control, &s_context);
    m_rendered_width = 0;
#endif
    SetPt(&m_scroll, 0, 0);
}

void browser_document::render_if_needed(int width)
{
#ifdef USE_LITEHTML
    if (m_lite_document && width != m_rendered_width)
    {
        m_lite_document->render(width);
        m_rendered_width = width;
    }
#endif
}

void browser_document::draw(Rect const& rect)
{
#ifdef USE_LITEHTML
    if (m_lite_document)
    {
        litehtml::position clip(rect.left + m_scroll.h, rect.top + m_scroll.v, rect_width(rect), rect_height(rect));
        m_lite_document->draw(0, -m_scroll.h, -m_scroll.v, &clip);
        //debugprintf("draw doc origin x=%d y=%d clip x=%d y=%d w=%d h=%d", -m_scroll.h, -m_scroll.v, clip.x, clip.y, clip.width, clip.height);
    }
#endif
}

Point browser_document::get_dimensions()
{
    Point dimensions;
#ifdef USE_LITEHTML
    if (m_lite_document)
        SetPt(&dimensions, m_lite_document->width(), m_lite_document->height());
    else
#endif
        SetPt(&dimensions, 0, 0);
    return dimensions;
}
