// Mac headers
#include <Devices.h>
#include <Dialogs.h>
#include <DiskInit.h>
#include <Fonts.h>
#include <LowMem.h>
#include <Quickdraw.h>
#include <TextEdit.h>
#include <ToolUtils.h>
#include <Windows.h>

// System headers

// Third-party headers

// My headers
#include "Constants.h"
#include "ResourceConstants.h"
#include "WebView.h"

// Globals
Boolean gDone = false;
Boolean gInBackground = false;
Boolean gMenubarDirty = true;

// Defines
#define kMoveToFront (WindowPtr)-1L
#define kScrollBarWidth 15
#define kWindowInset 2
#define RectWidth(rect) ((rect).right - (rect).left)
#define RectHeight(rect) ((rect).bottom - (rect).top)

void drawWindow(WindowPtr window)
{
	GrafPtr savedPort;
    WebViewRef webView = (WebViewRef)GetWRefCon(window);

	GetPort(&savedPort);
	SetPort(window);
	// TODO: don't erase?
	EraseRgn(window->visRgn);
    DrawWebView(webView);
	SetPort(savedPort);
}

/*
void fixWindowAfterResize(WindowPtr windowPtr)
{
	GrafPtr savedPort;

	GetPort(&savedPort);
	SetPort(windowPtr);
	InvalRect(&windowPtr->portRect);
	SetPort(savedPort);
}
*/

void setViewRects(WindowPtr window)
{
    WebViewRef webView = (WebViewRef)GetWRefCon(window);
    Rect rect;

    SetRect(&rect, 0, 0, RectWidth(window->portRect) - kScrollBarWidth, RectHeight(window->portRect) - kScrollBarWidth);
    SetWebViewRect(webView, &rect);
}

void invalidateScrollBarArea(WindowPtr windowPtr)
{
	GrafPtr savedPort;
    Rect rect;

	GetPort(&savedPort);
	SetPort(windowPtr);

    rect = windowPtr->portRect;
    rect.left = rect.right - kScrollBarWidth;
    InvalRect(&rect);

    rect = windowPtr->portRect;
    rect.top = rect.bottom - kScrollBarWidth;
    InvalRect(&rect);

    SetPort(savedPort);
}

void doGrowWindow(WindowPtr windowPtr, EventRecord *eventPtr)
{
	Rect limitRect, savedViewRect;
	long growSize;
	RgnHandle localUpdateRgn;
	Boolean result;
//	DocumentHandle documentHandle;

	SetRect(&limitRect, kMinDocWidth, kMinDocHeight, kMaxDocWidth, kMaxDocHeight);
	growSize = GrowWindow(windowPtr, eventPtr->where, &limitRect);
	if (growSize != 0)
    {
/*
		documentHandle = (DocumentHandle)GetWRefCon(windowPtr);
		savedViewRect = (**documentHandle).viewRect;
		localUpdateRgn = NewRgn();
		getLocalUpdateRgn(windowPtr, localUpdateRgn);
*/
		invalidateScrollBarArea(windowPtr);
		SizeWindow(windowPtr, LoWord(growSize), HiWord(growSize), true);
		invalidateScrollBarArea(windowPtr);
		setViewRects(windowPtr);
//		fixWindowAfterResize(windowPtr);
/*
		result = SectRect(&savedViewRect, &(**documentHandle).viewRect, &savedViewRect);
		ValidRect(&savedViewRect);
		InvalRgn(localUpdateRgn);
		DisposeRgn(localUpdateRgn);
*/
	}
}

void doZoomWindow(WindowPtr windowPtr, short zoomInOrOut)
{
/*
	GrafPtr savePort;

	GetPort(&savePort);
	SetPort(windowPtr);
	EraseRect(&windowPtr->portRect);

	if (zoomInOrOut == inZoomOut)
    {
		getIdealWindowRect(windowPtr, &(**(WStateDataHandle)((WindowPeek)windowPtr)->dataHandle).stdState, 1, 1, false);
	}
	ZoomWindow(windowPtr, zoomInOrOut, windowPtr == FrontWindow());
	fixWindowAfterResize(windowPtr);

	SetPort(savePort);
*/
}

void closeDocumentWindow(WindowPtr windowPtr)
{
	WebViewRef webViewRef;

	webViewRef = (WebViewRef)GetWRefCon(windowPtr);
	DisposeWebView(webViewRef);
	DisposeWindow(windowPtr);
}

short getWindowType(WindowPtr windowPtr)
{
	short windowKind, windowType;

	if (windowPtr == NULL)
    {
		windowType = kNoWindow;
	}
	else
    {
		windowKind = ((WindowPeek)windowPtr)->windowKind;
		if (windowKind == userKind)
        {
			windowType = kDocumentWindow;
		}
		else if (windowKind < 0)
        {
			windowType = kDAWindow;
		}
		else
        {
			windowType = GetWRefCon(windowPtr);
		}
	}

	return windowType;
}

void doContentClick(WindowPtr windowPtr, EventRecord *eventPtr)
{
}

void handleKeyDown(WindowPtr windowPtr, EventRecord *eventPtr)
{
	short windowType;

	windowType = getWindowType(windowPtr);
	switch (windowType)
	{
		case kDocumentWindow:
			break;
	}
}

void setItemEnabled(MenuHandle menuHandle, short menuItem, Boolean enabled)
{
	if (menuHandle != NULL)
	{
		if (menuItem == 0 && enabled != (**menuHandle).enableFlags & 1L)
		{
			gMenubarDirty = true;
		}

		if (enabled)
		{
			EnableItem(menuHandle, menuItem);
		}
		else
		{
			DisableItem(menuHandle, menuItem);
		}
	}
}

void adjustMenuItems(void)
{
	WindowPtr frontWindowPtr;
	short frontWindowType;
	MenuHandle menuHandle;

	frontWindowPtr = FrontWindow();
	frontWindowType = getWindowType(frontWindowPtr);

	menuHandle = GetMenuHandle(mFile);
	setItemEnabled(menuHandle, iClose, frontWindowType != kNoWindow);
}

void adjustMenus(void)
{
	WindowPtr frontWindowPtr;
	short frontWindowType;
	MenuHandle menuHandle;

	frontWindowPtr = FrontWindow();
	frontWindowType = getWindowType(frontWindowPtr);

	menuHandle = GetMenuHandle(mEdit);
	setItemEnabled(menuHandle, 0, frontWindowType == kDAWindow);
}

void doZoomCmd(WindowPtr windowPtr)
{
	short windowType;

	windowType = getWindowType(windowPtr);
	switch (windowType)
	{
		case kDocumentWindow:
			doZoomWindow(windowPtr, EqualRect(&(**((WindowPeek)windowPtr)->contRgn).rgnBBox, &(**(WStateDataHandle)((WindowPeek)windowPtr)->dataHandle).userState) ? inZoomOut : inZoomIn);
			break;
	}
}

void doCloseCmd(WindowPtr windowPtr)
{
	short windowType;

	windowType = getWindowType(windowPtr);
	switch (windowType)
	{
		case kDocumentWindow:
			closeDocumentWindow(windowPtr);
			break;
		case kDAWindow:
			CloseDeskAcc(((WindowPeek)windowPtr)->windowKind);
			break;
	}

	adjustMenus();
}

void doQuitCmd(void)
{
	WindowPtr frontWindowPtr;

	while (frontWindowPtr = FrontWindow())
	{
		doCloseCmd(frontWindowPtr);
	}

	gDone = true;
}

void doNewCmd(Boolean showWindow)
{
    Rect strucRect, contRect, windRect;
    WindowPtr windowP;
    WebViewRef webViewRef;
    char *html = "<html><head><title>Hello World</title></head><body><p>The quick brown fox jumps over the lazy dog.</p></body></html>";
    windowP = GetNewWindow(rWindow, NULL, kMoveToFront);
    if (windowP)
    {
        webViewRef = NewWebView(windowP);
        if (webViewRef)
        {
            SetWebViewHTML(webViewRef, html);
            SetWRefCon(windowP, (long)webViewRef);
            // Make the window visible but offscreen so that its structure and
            // content regions get initialized.
            MoveWindow(windowP, -32000, -32000, false);
            ShowWindow(windowP);
            strucRect = (**((WindowPeek)windowP)->strucRgn).rgnBBox;
            contRect = (**((WindowPeek)windowP)->contRgn).rgnBBox;
            windRect = qd.screenBits.bounds;
            windRect.left += kWindowInset + (contRect.left - strucRect.left);
            windRect.top += kWindowInset + (contRect.top - strucRect.top)
                + LMGetMBarHeight();
            windRect.right -= kWindowInset - (contRect.right - strucRect.right);
            windRect.bottom -= kWindowInset - (contRect.bottom - strucRect.bottom);
            if (!showWindow)
            {
                HideWindow(windowP);
            }
            SizeWindow(windowP, RectWidth(windRect), RectHeight(windRect), true);
            MoveWindow(windowP, windRect.left, windRect.top, true);
            setViewRects(windowP);
            adjustMenus();
        }
        else
        {
            DisposeWindow(windowP);
        }
    }
}

void doAboutCmd()
{
}

void handleEditCommand(short menuItem)
{
	if (!SystemEdit(menuItem - 1))
	{
	}
}

void handleFileCommand(short menuItem)
{
	switch (menuItem)
	{
		case iNew:
			doNewCmd(true);
			break;
		case iClose:
			doCloseCmd(FrontWindow());
			break;
		case iQuit:
			doQuitCmd();
			break;
	}
}

void handleAppleCommand(short menuItem)
{
	MenuHandle menuHandle;
	short daRefNum;
	Str255 itemName;

	switch (menuItem)
	{
		case iAbout:
			doAboutCmd();
			break;
		default:
			menuHandle = GetMenuHandle(mApple);
			if (menuHandle != NULL)
			{
				GetMenuItemText(menuHandle, menuItem, itemName);
				daRefNum = OpenDeskAcc(itemName);
				adjustMenus();
			}
	}
}

void doMenuCommand(long menuResult)
{
	short menuID, menuItem;
	unsigned long ticks;

	ticks = TickCount();
	menuID = HiWord(menuResult);
	menuItem = LoWord(menuResult);
	switch (menuID)
	{
		case mApple:
			handleAppleCommand(menuItem);
			break;
		case mFile:
			handleFileCommand(menuItem);
			break;
		case mEdit:
			handleEditCommand(menuItem);
			break;
	}
	while (TickCount() - ticks < 8) {}
	HiliteMenu(0);
}

void setUpMenus(void)
{
	Handle menuBarHandle;
	MenuHandle menuHandle;

	menuBarHandle = GetNewMBar(rMenuBar);
	if (menuBarHandle == NULL)
	{
		return;
	}

	SetMenuBar(menuBarHandle);
	DisposeHandle(menuBarHandle);

	menuHandle = GetMenuHandle(mApple);
	if (menuHandle != NULL)
	{
		AppendResMenu(menuHandle, 'DRVR');
	}

	adjustMenus();
	adjustMenuItems();
}

void doActivate(WindowPtr windowPtr, Boolean activate, EventRecord *eventPtr)
{
}

void doSuspendResumeEvent(EventRecord *eventPtr)
{
	gInBackground = !(eventPtr->message & resumeFlag);
	doActivate(FrontWindow(), !gInBackground, eventPtr);
}

void doIdle(EventRecord *eventPtr)
{
	WindowPtr frontWindowPtr;
	short frontWindowType;

	frontWindowPtr = FrontWindow();
	frontWindowType = getWindowType(frontWindowPtr);
	switch (frontWindowType)
	{
		case kDocumentWindow:
			break;
	}
}

void doHighLevelEvent(EventRecord *eventPtr)
{
}

void doOSEvent(EventRecord *eventPtr)
{
	switch ((eventPtr->message >> 24) & 0xFF)
	{
		case mouseMovedMessage:
			doIdle(eventPtr);
			break;
		case suspendResumeMessage:
			doSuspendResumeEvent(eventPtr);
			break;
	}
}

void doDiskEvent(EventRecord *eventPtr)
{
	Point point;
	OSErr err;

	if (HiWord(eventPtr->message) != noErr)
	{
		DILoad();
		SetPt(&point, 100, 100);
		err = DIBadMount(point, eventPtr->message);
		DIUnload();
	}
}

void doUpdate(WindowPtr windowPtr)
{
	short windowType;

	windowType = getWindowType(windowPtr);
	switch (windowType)
	{
		case kDocumentWindow:
			BeginUpdate(windowPtr);
			DrawGrowIcon(windowPtr);
			drawWindow(windowPtr);
			EndUpdate(windowPtr);
			break;
	}
}

void doKeyDown(EventRecord *eventPtr)
{
	char key;

	if (eventPtr->modifiers & cmdKey)
	{
		if (eventPtr->what == keyDown)
		{
			adjustMenuItems();
        	key = (char)(eventPtr->message & charCodeMask);
			doMenuCommand(MenuKey(key));
		}	
	}
	else
	{
		handleKeyDown(FrontWindow(), eventPtr);
	}
}

void doMouseDown(EventRecord *eventPtr)
{
	short part;
	WindowPtr windowPtr;

	part = FindWindow(eventPtr->where, &windowPtr);
	switch (part)
	{
		case inMenuBar:
			adjustMenuItems();
			doMenuCommand(MenuSelect(eventPtr->where));
			break;
		case inSysWindow:
			SystemClick(eventPtr, windowPtr);
			break;
		case inContent:
			if (windowPtr == FrontWindow())
			{
				doContentClick(windowPtr, eventPtr);
			}
			else
			{
				SelectWindow(windowPtr);
			}
			break;
		case inDrag:
			DragWindow(windowPtr, eventPtr->where, &(**GetGrayRgn()).rgnBBox);
			break;
		case inGrow:
			doGrowWindow(windowPtr, eventPtr);
			break;
		case inGoAway:
			if (TrackGoAway(windowPtr, eventPtr->where))
			{
				doCloseCmd(windowPtr);
			}
			break;
		case inZoomIn:
		case inZoomOut:
			if (TrackBox(windowPtr, eventPtr->where, part))
			{
				doZoomWindow(windowPtr, part);
			}
			break;
	}
}

void doEvent(EventRecord *eventPtr)
{
	switch (eventPtr->what)
	{
		case mouseDown:
			doMouseDown(eventPtr);
			break;
		case keyDown:
		case autoKey:
			doKeyDown(eventPtr);
			break;
		case activateEvt:
			doActivate((WindowPtr)eventPtr->message, eventPtr->modifiers & activeFlag, eventPtr);
			break;
		case updateEvt:
			doUpdate((WindowPtr)eventPtr->message);
			break;
		case diskEvt:
			doDiskEvent(eventPtr);
			break;
		case osEvt:
			doOSEvent(eventPtr);
			break;
		case kHighLevelEvent:
			doHighLevelEvent(eventPtr);
			break;
	}
}

void doEventLoop(void)
{
	Boolean gotEvent;
	EventRecord event;
	RgnHandle cursorRgn = nil;

	while (!gDone)
	{
		if (gMenubarDirty)
		{
			DrawMenuBar();
			gMenubarDirty = false;
		}
		gotEvent = WaitNextEvent(everyEvent, &event, kSleepTime, cursorRgn);
		if (gotEvent)
		{
			doEvent(&event);
		}
		else
		{
			doIdle(&event);
		}
	}
}

void thisManyMoreMasters(short numMasters)
{
    THz zone;
    short savedNumMasters;

    zone = GetZone();
    savedNumMasters = zone->moreMast;
    zone->moreMast = numMasters;
    MoreMasters();
    zone->moreMast = savedNumMasters;
}

void initToolbox(void)
{
    MaxApplZone();
    // TODO: figure out how many masters we need
    //thisManyMoreMasters(64);
    InitGraf(&qd.thePort);
    InitFonts();
    InitWindows();
    InitMenus();
    TEInit();
    InitDialogs(nil);
    FlushEvents(everyEvent, 0);
    InitCursor();
    GetDateTime(&qd.randSeed);
}

void main(void)
{
    initToolbox();
    setUpMenus();
    doNewCmd(true);
    doEventLoop();
    ExitToShell();
}
