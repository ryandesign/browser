#ifndef WEBVIEW_H
#define WEBVIEW_H

// Mac headers
#include <Types.h>

// System headers

// Third-party headers

// My headers

#ifdef __cplusplus
#include <litehtml.h>
typedef litehtml::document::ptr LiteDocPtr;
extern "C" {
#else
typedef Ptr LiteDocPtr;
#endif

typedef struct WebViewRecord {
    Ptr htmlP;
    Size htmlLen;
    LiteDocPtr liteDocPtr;
} WebViewRecord, *WebViewPtr, **WebViewHandle;

WebViewHandle NewWebView(char *html);
void DrawWebView(WebViewHandle webViewH);
void DisposeWebView(WebViewHandle webViewH);

#ifdef __cplusplus
}
#endif

#endif
