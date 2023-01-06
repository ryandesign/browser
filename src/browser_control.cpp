// SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
//
// SPDX-License-Identifier: MIT

#include "browser_control.h"

#include "browser_document.h"
#include "browser_window.h"
#include "helpers.h"

browser_control::browser_control(int16_t resource_id, WindowRecord& window)
    : user_control(resource_id, window),
      m_document(nullptr)
{
}

browser_control::~browser_control()
{
}

void browser_control::set_document(std::shared_ptr<browser_document> document)
{
    m_document = document;
}

#ifdef USE_LITEHTML

// quickdraw_container overrides:

void browser_control::get_client_rect(litehtml::position& client) const
{
    Rect rect = get_rect();
    client.x = rect.left;
    client.y = rect.top;
    client.width = rect_width(rect);
    client.height = rect_height(rect);
}

void browser_control::set_caption(char const *caption)
{
    get_window().set_title(caption);
}

#endif

// user_control overrides:

void browser_control::draw(int16_t part) const
{
    if (0 == part && m_document)
    {
        Rect rect = get_rect();
        Point origin;
        SetPt(&origin, rect.left, rect.top);
        SectRect(&(**(**m_control).contrlOwner->visRgn).rgnBBox, &rect, &rect);
        EraseRect(&rect);
        OffsetRect(&rect, -origin.h, -origin.v);
        SetOrigin(-origin.h, -origin.v);
        m_document->draw(rect);
        SetOrigin(0, 0);
    }
}
