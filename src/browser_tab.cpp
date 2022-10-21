// SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
//
// SPDX-License-Identifier: MIT

// Mac headers
#include <MacTypes.h>
#include <Resources.h>

// System headers

// Third-party headers

// My headers
#include "ResourceConstants.h"
#include "browser_tab.h"

browser_tab::browser_tab(WindowRecord& window)
    : m_web_view(nullptr)
{
#if USE_LITEHTML
    m_web_view = NewWebView(reinterpret_cast<WindowPtr>(&window));
    if (!m_web_view)
        throw std::bad_alloc();
    Handle html = GetResource('TEXT', r_TEXT_html);
    if (!html)
        throw std::bad_alloc();
    int16_t saved_state = HGetState(html);
    HLock(html);
    setHTML(*html);
    HSetState(html, saved_state);
#endif
}

browser_tab::~browser_tab()
{
#if USE_LITEHTML
    DisposeWebView(m_web_view);
#endif
}

void browser_tab::setHTML(char const *html)
{
#if USE_LITEHTML
    SetWebViewHTML(m_web_view, html);
#endif
}

void browser_tab::update(EventRecord const& event)
{
#if USE_LITEHTML
    DrawWebView(m_web_view);
#endif
}
