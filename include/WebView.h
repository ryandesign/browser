// SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
//
// SPDX-License-Identifier: MIT

#ifndef WEBVIEW_H
#define WEBVIEW_H

// Mac headers
#include <MacWindows.h>

// System headers

// Third-party headers

// My headers

typedef Ptr WebViewRef;

/*
typedef struct WebViewRecord {
    Ptr htmlP;
    Size htmlLen;
    LiteDocPtr liteDocPtr;
} WebViewRecord, *WebViewPtr, **WebViewHandle;
*/

#ifdef __cplusplus
extern "C" {
#endif

void init_web_views(char const * const css);
WebViewRef NewWebView(WindowPtr window);
void SetWebViewRect(WebViewRef webViewRef, Rect const *rect);
void SetWebViewHTML(WebViewRef webViewRef, char const *html);
void DrawWebView(WebViewRef webViewRef);
void DisposeWebView(WebViewRef webViewRef);

#ifdef __cplusplus
}
#endif

#endif
