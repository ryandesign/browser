// SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
//
// SPDX-License-Identifier: MIT

#include "WebView.hpp"

// Mac headers
#include <MacMemory.h>
#include <MacWindows.h>
#include <Quickdraw.h>
//#include <TextUtils.h>

// System headers
/*#include <stddef.h>*/
#include <string.h>

// Third-party headers
#include <litehtml.h>

// My headers
#include "WebView.h"
#include "helpers.h"
#include "quickdraw_container.h"

WebView::WebView(litehtml::context *liteContext, WindowRecord& window)
    : m_liteContext(liteContext), m_window(window), m_renderedWidth(0)
{
    SetRect(&m_rect, 0, 0, rect_width(window.port.portRect), rect_height(window.port.portRect));
    SetPt(&m_scroll, 0, 0);
}

WebView::~WebView()
{
}

void WebView::get_client_rect(litehtml::position& client) const
{
    client.x = m_rect.left;
    client.y = m_rect.right;
    client.width = m_rect.right - client.x;
    client.height = m_rect.bottom - client.y;
}

void WebView::set_caption(char const *caption)
{
    setwtitle(reinterpret_cast<WindowPtr>(&m_window), caption);
}

void WebView::setRect(Rect const *rect)
{
    m_rect = *rect;
    m_renderedWidth = 0;
}

void WebView::setHTML(char const *html)
{
    m_liteDoc = litehtml::document::createFromUTF8(html, this, m_liteContext);
    m_renderedWidth = 0;
}

int16_t WebView::render() {
    int16_t bestWidth = 0;

    if (m_liteDoc)
    {
        int16_t maxWidth = rect_width(m_rect);
        if (m_renderedWidth != maxWidth)
        {
            bestWidth = m_liteDoc->render(maxWidth);
            if (bestWidth < maxWidth)
            {
                m_liteDoc->render(bestWidth);
            }
            //m_renderedWidth = maxWidth;
        }
    }

    return bestWidth;
}

void WebView::draw()
{
    if (m_liteDoc)
    {
        // TODO: I shouldn't have to render every time I draw, should I?
        int16_t width = render();
        litehtml::position clip(m_scroll.h, m_scroll.v, width, rect_height(m_rect));
/*
        Str255 d;
        NumToString(clip.x, d);
        DebugStr(d);
        NumToString(clip.y, d);
        DebugStr(d);
        NumToString(clip.width, d);
        DebugStr(d);
        NumToString(clip.height, d);
        DebugStr(d);
*/
        m_liteDoc->draw(0, -clip.x, -clip.y, &clip);
    }
}

static bool s_initialized = false;
static litehtml::context s_context;

void init_web_views(char const * const css)
{
    if (!s_initialized)
    {
        s_context.load_master_stylesheet(css);
        s_initialized = true;
    }
}

WebViewRef NewWebView(WindowPtr window)
{
    WebView *webView;
//    Ptr htmlP;
//    Size htmlPSize;

    if (!s_initialized)
        return nullptr;

    // TODO: wrap in try?
    webView = new WebView(&s_context, *reinterpret_cast<WindowPeek>(window));
    /*;

    if (webViewPtr) {
        if (html) {
            (**webViewPtr).liteDocPtr = new litehtml::document::createFromUTF8(html, quickdraw_container, context);
            (**webViewPtr).htmlLen = strlen(html);
            htmlPSize = (**webViewPtr).htmlLen + 1;
            htmlP = NewPtr(htmlPSize);
            if (htmlP) {
                BlockMoveData(html, htmlP, htmlPSize);
                (**webViewPtr).htmlP = htmlP;
            } else {
                DisposeHandle((Handle)webViewPtr);
                webViewPtr = nullptr;
            }
        }
    }
    */
    return reinterpret_cast<WebViewRef>(webView);
}

void SetWebViewRect(WebViewRef webViewRef, Rect const *rect)
{
    WebView *webView = reinterpret_cast<WebView *>(webViewRef);

    webView->setRect(rect);
}

void SetWebViewHTML(WebViewRef webViewRef, char const *html)
{
    WebView *webView = reinterpret_cast<WebView *>(webViewRef);

    webView->setHTML(html);
}

void DrawWebView(WebViewRef webViewRef)
{
    WebView *webView = reinterpret_cast<WebView *>(webViewRef);

    webView->draw();
/*
    if ((**webViewPtr).htmlP) {
        MoveTo(3, 13);
        DrawText((**webViewPtr).htmlP, 0, (**webViewPtr).htmlLen);
    }
*/
}

void DisposeWebView(WebViewRef webViewRef)
{
    WebView *webView = reinterpret_cast<WebView *>(webViewRef);

    delete webView;
}
