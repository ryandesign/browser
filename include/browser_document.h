// SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
//
// SPDX-License-Identifier: MIT

#ifndef BROWSER_DOCUMENT_H
#define BROWSER_DOCUMENT_H

#include <MacTypes.h>
#ifdef USE_LITEHTML
#include <litehtml.h>
#endif
#include <memory>

#include "browser_control.h"

class browser_document
{
    public:
        typedef std::shared_ptr<browser_document> ptr;

        browser_document();
        ~browser_document();
        void set_html(const char *const html, browser_control& control);
        void render_if_needed(int width);
        void draw(Rect const& rect);
        Point get_dimensions();

    private:
#ifdef USE_LITEHTML
        litehtml::document::ptr m_lite_document;
        int16_t m_rendered_width;
#endif
        Point m_scroll;
};

#endif
