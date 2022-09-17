// Mac headers
#include <Memory.h>
#include <Quickdraw.h>
#include <Windows.h>
//#include <TextUtils.h>

// System headers
/*#include <stddef.h>*/
#include <string.h>

// Third-party headers
#include <litehtml.h>

// My headers
#include "WebView.h"
#include "WebView.hpp"
#include "quickdraw_container.h"

// Defines
#define RectWidth(rect) ((rect).right - (rect).left)
#define RectHeight(rect) ((rect).bottom - (rect).top)

WebView::WebView(litehtml::context *liteContext, WindowPtr window)
    : m_liteContext(liteContext), m_window(window), m_renderedWidth(0)
{
    SetRect(&m_rect, 0, 0, RectWidth(window->portRect), RectHeight(window->portRect));
    SetPt(&m_scroll, 0, 0);
}

WebView::~WebView()
{
    // TODO: Do I not have to delete this?
    //delete m_liteDoc;
}

void WebView::get_client_rect(litehtml::position& client) const
{
    client.x = m_rect.left;
    client.y = m_rect.right;
    client.width = m_rect.right - client.x;
    client.height = m_rect.bottom - client.y;
}

void WebView::set_caption(const char *caption)
{
    setwtitle(m_window, caption);
}

void WebView::setRect(const Rect *rect)
{
    m_rect = *rect;
    m_renderedWidth = 0;
}

void WebView::setHTML(const char *html)
{
    // TODO: Do I not have to delete this?
    //delete m_liteDoc;
    m_liteDoc = litehtml::document::createFromUTF8(html, this, m_liteContext);
    m_renderedWidth = 0;
}

short WebView::render() {
    short bestWidth = 0;

    if (m_liteDoc)
    {
        short maxWidth = RectWidth(m_rect);
        if (m_renderedWidth != maxWidth)
        {
            bestWidth = m_liteDoc->render(maxWidth);
            if (bestWidth < maxWidth)
            {
                m_liteDoc->render(bestWidth);
            }
            m_renderedWidth = maxWidth;
        }
    }

    return bestWidth;
}

void WebView::draw()
{
    if (m_liteDoc)
    {
        short width = render();
        litehtml::position clip(m_scroll.h, m_scroll.v, width, RectHeight(m_rect));
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

static const char g_masterCss[] =
{
#include "master.css.inc"
};

WebViewRef NewWebView(WindowPtr window)
{
    static litehtml::context s_liteContext;
    static bool s_liteContextInitialized = false;
    WebView *webView;
//    Ptr htmlP;
//    Size htmlPSize;

    if (!s_liteContextInitialized)
    {
        s_liteContext.load_master_stylesheet(g_masterCss);
        s_liteContextInitialized = true;
    }

    // TODO: wrap in try?
    webView = new WebView(&s_liteContext, window);
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
                webViewPtr = nil;
            }
        }
    }
    */
    return (WebViewRef)webView;
}

void SetWebViewRect(WebViewRef webViewRef, const Rect *rect)
{
    WebView *webView = (WebView *)webViewRef;

    webView->setRect(rect);
}

void SetWebViewHTML(WebViewRef webViewRef, const char *html)
{
    WebView *webView = (WebView *)webViewRef;

    webView->setHTML(html);
}

void DrawWebView(WebViewRef webViewRef)
{
    WebView *webView = (WebView *)webViewRef;

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
    WebView *webView = (WebView *)webViewRef;

    delete webView;
}
