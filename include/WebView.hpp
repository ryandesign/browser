// SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
//
// SPDX-License-Identifier: MIT

#ifndef WEBVIEW_HPP
#define WEBVIEW_HPP

#include "quickdraw_container.h"

class WebView : public quickdraw_container
{
    public:
        WebView(litehtml::context *liteContext, WindowPtr window);
        ~WebView();

        void get_client_rect(litehtml::position& client) const;
        void set_caption(const char *caption);

        void setRect(const Rect *rect);
        void setHTML(const char *html);
        short render();
        void draw();

    private:
        litehtml::context *m_liteContext;
        litehtml::document::ptr m_liteDoc;
        WindowPtr m_window;
        Rect m_rect;
        Point m_scroll;
        short m_renderedWidth;
};

#endif
