// SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
//
// SPDX-License-Identifier: MIT

#ifndef WEBVIEW_HPP
#define WEBVIEW_HPP

#include <MacWindows.h>

#include "quickdraw_container.h"

class WebView : public quickdraw_container
{
    public:
        WebView(litehtml::context *liteContext, WindowRecord& window);
        ~WebView();

        void get_client_rect(litehtml::position& client) const;
        void set_caption(char const *caption);

        void setRect(Rect const *rect);
        void setHTML(char const *html);
        int16_t render();
        void draw();

    private:
        litehtml::context *m_liteContext;
        litehtml::document::ptr m_liteDoc;
        WindowRecord& m_window;
        Rect m_rect;
        Point m_scroll;
        int16_t m_renderedWidth;
};

#endif
