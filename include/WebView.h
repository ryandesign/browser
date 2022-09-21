#ifndef WEBVIEW_H
#define WEBVIEW_H

// Mac headers
//#include <Types.h>

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

void InitWebViews(FourCharCode css_rsrc_type, short css_rsrc_id);
WebViewRef NewWebView(WindowPtr window);
void SetWebViewRect(WebViewRef webViewRef, const Rect *rect);
void SetWebViewHTML(WebViewRef webViewRef, const char *html);
void DrawWebView(WebViewRef webViewRef);
void DisposeWebView(WebViewRef webViewRef);

#ifdef __cplusplus
}
#endif

#endif
