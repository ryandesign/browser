// SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
//
// SPDX-License-Identifier: MIT

// Mac headers
#include <Fonts.h>
#include <Quickdraw.h>

// System headers

// Third-party headers

// My headers
#include "quickdraw_font.h"

quickdraw_font::quickdraw_font(const char *name, int16_t size, StyleParameter style)
    : m_size(size), m_style(style)
{
    getfnum(name, &m_id);
    construct();
}

quickdraw_font::quickdraw_font(int16_t id, int16_t size, StyleParameter style)
    : m_id(id), m_size(size), m_style(style)
{
    construct();
}

quickdraw_font::~quickdraw_font()
{
    GrafPtr saved_port;
    GetPort(&saved_port);
    ClosePort(&m_port);
}

void quickdraw_font::draw(const char *text, const litehtml::position& pos)
{
    set_port_font();
    MoveTo(pos.x, pos.y + m_metrics.ascent);
//    MoveTo(pos.x, pos.y + pos.height - m_metrics.descent);
    DrawText(text, 0, strlen(text));
/*
    Rect r;
    SetRect(&r, pos.left(), pos.top(), pos.right(), pos.bottom());
    FrameRect(&r);
*/
}

int16_t quickdraw_font::width(const char *text)
{
    GrafPtr saved_port;
    GetPort(&saved_port);
    SetPort(&m_port);
    int16_t width = TextWidth(text, 0, strlen(text));
    SetPort(saved_port);
    return width;
}

FontInfo& quickdraw_font::metrics()
{
    return m_metrics;
}

void quickdraw_font::construct()
{
    GrafPtr saved_port;
    GetPort(&saved_port);
    OpenPort(&m_port);
    set_port_font();
    GetFontInfo(&m_metrics);
    SetPort(saved_port);
}

void quickdraw_font::set_port_font()
{
    TextFont(m_id);
    TextSize(m_size);
    TextFace(m_style);
}
