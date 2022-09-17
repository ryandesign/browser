// Mac headers
#include <Fonts.h>
#include <Quickdraw.h>

// System headers

// Third-party headers

// My headers
#include "quickdraw_font.h"

quickdraw_font::quickdraw_font(const char *name, short size, StyleParameter style)
    : m_size(size), m_style(style)
{
    getfnum(name, &m_id);
    construct();
}

quickdraw_font::quickdraw_font(short id, short size, StyleParameter style)
    : m_id(id), m_size(size), m_style(style)
{
    construct();
}

quickdraw_font::~quickdraw_font()
{
}

void quickdraw_font::draw(const litehtml::tchar_t *text, const litehtml::position& pos)
{
    setUpPort();
    MoveTo(pos.x, pos.y + m_metrics.ascent);
//    MoveTo(pos.x, pos.y + pos.height - m_metrics.descent);
    DrawText(text, 0, strlen(text));
/*
    Rect r;
    SetRect(&r, pos.left(), pos.top(), pos.right(), pos.bottom());
    FrameRect(&r);
*/
}

short quickdraw_font::width(const litehtml::tchar_t *text)
{
    setUpPort();
    return TextWidth(text, 0, strlen(text));
}

FontInfo& quickdraw_font::metrics()
{
    return m_metrics;
}

void quickdraw_font::construct()
{
    setUpPort();
    GetFontInfo(&m_metrics);
}

void quickdraw_font::setUpPort()
{
    TextFont(m_id);
    TextSize(m_size);
    TextFace(m_style);
}
