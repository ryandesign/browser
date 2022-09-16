// Mac headers
#include <Memory.h>
#include <Quickdraw.h>

// System headers
/*#include <stddef.h>*/
#include <string.h>

// Third-party headers
#include <litehtml.h>

// My headers
#include "WebView.h"
#include "quickdraw_container.h"

static litehtml::context gWebViewContext;
static bool gWebViewContextInitialized = false;

static const char gMasterCss[] = {
#include "master.css.inc"
};

WebViewHandle NewWebView(char *html)
{
    WebViewHandle webViewH = (WebViewHandle)NewHandleClear(sizeof(WebViewRecord));
    Ptr htmlP;
    Size htmlPSize;

    if (!gWebViewContextInitialized) {
        gWebViewContext.load_master_stylesheet(gMasterCss);
        gWebViewContextInitialized = true;
    }

    if (webViewH) {
        if (html) {
            (**webViewH).liteDocPtr = new litehtml::document(quickdraw_container, gWebViewContext);
            (**webViewH).htmlLen = strlen(html);
            htmlPSize = (**webViewH).htmlLen + 1;
            htmlP = NewPtr(htmlPSize);
            if (htmlP) {
                /*strlcpy(htmlP, html, htmlPSize);*/
                BlockMoveData(html, htmlP, htmlPSize);
                (**webViewH).htmlP = htmlP;
            } else {
                DisposeHandle((Handle)webViewH);
                webViewH = nil;
            }
        }
    }
    return webViewH;
}

void DrawWebView(WebViewHandle webViewH)
{
    SInt8 savedState = HGetState((Handle)webViewH);

    HLock((Handle)webViewH);
    if ((**webViewH).htmlP) {
        MoveTo(3, 13);
        DrawText((**webViewH).htmlP, 0, (**webViewH).htmlLen);
    }
    HSetState((Handle)webViewH, savedState);
}

void DisposeWebView(WebViewHandle webViewH)
{
    if ((**webViewH).htmlP) {
        DisposePtr((**webViewH).htmlP);
    }
    DisposeHandle((Handle)webViewH);
}
