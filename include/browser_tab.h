// SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
//
// SPDX-License-Identifier: MIT

#ifndef BROWSER_TAB_H
#define BROWSER_TAB_H

// Mac headers

// System headers
#include <memory>

// Third-party headers

// My headers
#if USE_LITEHTML
#include "WebView.h"
#endif

class browser_tab
{
    public:
        browser_tab(WindowRecord& window);
        ~browser_tab();
        void update(EventRecord const& event);
        void setHTML(char const *html);

    private:
        std::unique_ptr<browser_tab> m_next_tab;
#if USE_LITEHTML
        WebViewRef m_web_view;
#endif
};

#endif
