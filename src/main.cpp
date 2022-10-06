// SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
//
// SPDX-License-Identifier: MIT

// Mac headers
#include <Appearance.h>
#include <Devices.h>
#include <Dialogs.h>
#include <DiskInit.h>
#include <Fonts.h>
#include <Gestalt.h>
#include <LowMem.h>
#include <MacTypes.h>
#include <MacWindows.h>
#include <Quickdraw.h>
#include <Resources.h>
#include <TextEdit.h>
#include <ToolUtils.h>
#include <Traps.h>

// System headers

// Third-party headers

// My headers
#include "Constants.h"
#include "oserr_exception.h"
#include "ResourceConstants.h"
#include "WebView.h"

// Globals
Boolean gDone = false;
Boolean gInBackground = false;
Boolean gMenubarDirty = true;

// Defines
#define kScrollBarWidth 15
#define kWindowInset 2
#define RectWidth(rect) ((rect).right - (rect).left)
#define RectHeight(rect) ((rect).bottom - (rect).top)

static void error_alert(unsigned short error_number)
{
    short item_hit;
    Str255 error_message;
    OSErr err;
    AlertStdAlertParamRec params;

    params.movable = false;
    params.helpButton = false;
    params.filterProc = nil;
    params.defaultText = (StringPtr)kAlertDefaultOKText;
    params.cancelText = nil;
    params.otherText = nil;
    params.defaultButton = kAlertStdAlertOKButton;
    params.cancelButton = 0;
    params.position = kWindowDefaultPosition;
    GetIndString(error_message, r_STRx_error_messages, error_number);
    err = StandardAlert(kAlertStopAlert, error_message, nil, &params, &item_hit);
    // TODO: Remove this debugging code:
    if (err)
    {
        NumToString(err, error_message);
        DebugStr(error_message);
    }
}

static void oserr_alert(OSErr err)
{
    unsigned short error_number;

    switch(err)
    {
        case memFullErr:
            error_number = e_no_memory;
            break;
        default:
            error_number = e_unknown_error;
    }

    error_alert(error_number);
}

void delay_until(unsigned long ticks)
{
    EventRecord event;

    while (TickCount() < ticks)
        EventAvail(kNoEvents, &event);
}

void drawWindow(WindowPtr window)
{
	GrafPtr savedPort;
#if USE_LITEHTML
    WebViewRef webView = (WebViewRef)GetWRefCon(window);
#endif

	GetPort(&savedPort);
	SetPort(window);
	// TODO: don't erase?
	EraseRgn(window->visRgn);
#if USE_LITEHTML
    DrawWebView(webView);
#endif
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
#ifdef USE_LITEHTML
    WebViewRef webView = (WebViewRef)GetWRefCon(window);
    Rect rect;

    SetRect(&rect, 0, 0, RectWidth(window->portRect) - kScrollBarWidth, RectHeight(window->portRect) - kScrollBarWidth);
    SetWebViewRect(webView, &rect);
#endif
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
    GrafPtr saved_port;
	Rect limitRect, savedViewRect;
	long growSize;
	RgnHandle localUpdateRgn;
	Boolean result;
//	DocumentHandle documentHandle;

	SetRect(&limitRect, kMinDocWidth, kMinDocHeight, kMaxDocWidth, kMaxDocHeight);
	growSize = GrowWindow(windowPtr, eventPtr->where, &limitRect);
	if (growSize != 0)
    {
        GetPort(&saved_port);
        SetPort(windowPtr);
/*
		documentHandle = (DocumentHandle)GetWRefCon(windowPtr);
		savedViewRect = (**documentHandle).viewRect;
		localUpdateRgn = NewRgn();
		getLocalUpdateRgn(windowPtr, localUpdateRgn);
*/
//       invalidateScrollBarArea(windowPtr);
        SizeWindow(windowPtr, LoWord(growSize), HiWord(growSize), false);
//       invalidateScrollBarArea(windowPtr);
		setViewRects(windowPtr);
       InvalRect(&windowPtr->portRect);
//		fixWindowAfterResize(windowPtr);
/*
		result = SectRect(&savedViewRect, &(**documentHandle).viewRect, &savedViewRect);
		ValidRect(&savedViewRect);
		InvalRgn(localUpdateRgn);
		DisposeRgn(localUpdateRgn);
*/
        SetPort(saved_port);
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
#ifdef USE_LITEHTML
	WebViewRef webViewRef;

	webViewRef = (WebViewRef)GetWRefCon(windowPtr);
	DisposeWebView(webViewRef);
#endif
	DisposeWindow(windowPtr);
}

short getWindowType(WindowPtr windowPtr)
{
	short windowKind, windowType;

	if (windowPtr)
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
	else
    {
		windowType = kNoWindow;
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

void setItemEnabled(MenuHandle menu, short menuItem, Boolean enabled)
{
	if (menu)
	{
		if (menuItem == 0 && enabled != (**menu).enableFlags & 1L)
		{
			gMenubarDirty = true;
		}

		if (enabled)
		{
			EnableItem(menu, menuItem);
		}
		else
		{
			DisableItem(menu, menuItem);
		}
	}
}

void adjustMenuItems()
{
	WindowPtr frontWindowPtr;
	short frontWindowType;
	MenuHandle menu;

	frontWindowPtr = FrontWindow();
	frontWindowType = getWindowType(frontWindowPtr);

    menu = GetMenuHandle(r_MENU_file);
	setItemEnabled(menu, iClose, frontWindowType != kNoWindow);
}

void adjustMenus()
{
	WindowPtr frontWindowPtr;
	short frontWindowType;
	MenuHandle menu;

	frontWindowPtr = FrontWindow();
	frontWindowType = getWindowType(frontWindowPtr);

    menu = GetMenuHandle(r_MENU_edit);
	setItemEnabled(menu, 0, frontWindowType == kDAWindow);
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

void doQuitCmd()
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
    Boolean ok = true;
    char const * const html = "<html><head><title>Hello World</title></head><body><p>The quick brown fox jumps over the lazy dog.</p></body></html>";

    windowP = GetNewCWindow(rWindow, NULL, kMoveToFront);
    if (!windowP)
        ok = false;

#ifdef USE_LITEHTML
    if (ok)
    {
        webViewRef = NewWebView(windowP);
        if (webViewRef)
        {
            SetWebViewHTML(webViewRef, html);
            SetWRefCon(windowP, (long)webViewRef);
        }
        else
            ok = false;
    }
#endif

    if (ok) {
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
            HideWindow(windowP);
        SizeWindow(windowP, RectWidth(windRect), RectHeight(windRect), true);
        MoveWindow(windowP, windRect.left, windRect.top, true);
        setViewRects(windowP);
        adjustMenus();
    } else {
        if (windowP)
            DisposeWindow(windowP);
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
	MenuHandle menu;
	short daRefNum;
	Str255 itemName;

	switch (menuItem)
	{
		case iAbout:
			doAboutCmd();
			break;
		default:
            menu = GetMenuHandle(r_MENU_apple);
			if (menu)
			{
				GetMenuItemText(menu, menuItem, itemName);
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
        case r_MENU_apple:
			handleAppleCommand(menuItem);
			break;
        case r_MENU_file:
			handleFileCommand(menuItem);
			break;
        case r_MENU_edit:
			handleEditCommand(menuItem);
			break;
	}
    delay_until(ticks + kVisualDelay);
	HiliteMenu(0);
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
			drawWindow(windowPtr);
			EndUpdate(windowPtr);
			break;
	}
}

void doKeyDown(EventRecord *eventPtr)
{
	if (eventPtr->modifiers & cmdKey)
	{
		if (eventPtr->what == keyDown)
		{
			adjustMenuItems();
            doMenuCommand(MenuEvent(eventPtr));
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

void doEventLoop()
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
        try {
            if (gotEvent)
                doEvent(&event);
            else
                doIdle(&event);
        }
        catch (oserr_exception const& e)
        {
            oserr_alert(e.err());
        }
        catch (std::bad_alloc const& e)
        {
            error_alert(e_no_memory);
        }
        catch (...)
        {
            error_alert(e_unknown_error);
        }
	}
}

void this_many_more_masters(short num_masters)
{
    THz zone;
    short saved_num_masters;

    zone = GetZone();
    saved_num_masters = zone->moreMast;
    zone->moreMast = num_masters;
    MoreMasters();
    zone->moreMast = saved_num_masters;
}

static void expand_stack(long extra_bytes)
{
    SetApplLimit(GetApplLimit() - extra_bytes);
}

static void fatal_error_alert(unsigned short error_number, Boolean has_autopositioning)
{
    short item_hit;
    Str255 error_message;

#ifdef __m68k__
    AlertTHndl alrt;
    Point offset;
    if (!has_autopositioning)
    {
        alrt = (AlertTHndl)GetResource('ALRT', r_ALRT_fatal_error);
        if (alrt)
        {
            offset.h = (RectWidth(qd.screenBits.bounds) - RectWidth((**alrt).boundsRect) >> 1) - (**alrt).boundsRect.left;
            offset.v = LMGetMBarHeight() + 64 - (**alrt).boundsRect.top;
            OffsetRect(&(**alrt).boundsRect, offset.h, offset.v);
        }
    }
#endif

    SetCursor(&qd.arrow);
    GetIndString(error_message, r_STRx_error_messages, error_number);
    ParamText(error_message, "\p", "\p", "\p");
    item_hit = StopAlert(r_ALRT_fatal_error, nil);

#ifdef __m68k__
    if (!has_autopositioning && alrt)
        OffsetRect(&(**alrt).boundsRect, -offset.h, -offset.v);
#endif

    ExitToShell();
}

static int num_toolbox_traps()
{
	if (NGetTrapAddress(_InitGraf, ToolTrap) == NGetTrapAddress(0xAA6E, ToolTrap))
		return 0x200;
	return 0x400;
}

static TrapType get_trap_type(short trap)
{
	if (trap & 0x0800)
	    return ToolTrap;
	return OSTrap;
}

static Boolean trap_available(short trap)
{
	TrapType trap_type;

	trap_type = get_trap_type(trap);
	if (ToolTrap == trap_type) {
		trap &= 0x03FF;
		if (trap >= num_toolbox_traps())
			trap = _Unimplemented;
	}
	return NGetTrapAddress(trap, trap_type) != NGetTrapAddress(_Unimplemented, ToolTrap);
}

static Boolean has_128k_rom()
{
    return LMGetROM85() >= 0;
}

static void init_app()
{
	Handle menuBar;
	MenuHandle menu;
	OSErr err;
	long result;
    Boolean has_autopositioning;

#ifndef __m68k__
    has_autopositioning = true;
#else
    if (has_128k_rom() && trap_available(_Gestalt))
        err = Gestalt(gestaltSystemVersion, &result);
    else
        err = unimpErr;

    if (err)
        has_autopositioning = false;
    else
        has_autopositioning = LoWord(result) >= 0x0700;

    if (!err)
#endif
    	err = Gestalt(gestaltAppearanceAttr, &result);
	if (err)
        fatal_error_alert(e_no_appearance, has_autopositioning);
	RegisterAppearanceClient();

    menuBar = GetNewMBar(r_MBAR);
	if (!menuBar)
        fatal_error_alert(e_no_resource, has_autopositioning);
	SetMenuBar(menuBar);
	DisposeHandle(menuBar);

    menu = GetMenuHandle(r_MENU_apple);
	if (!menu)
        fatal_error_alert(e_no_resource, has_autopositioning);
	AppendResMenu(menu, 'DRVR');

    GetDateTime((unsigned long *)&qd.randSeed);
#ifdef USE_LITEHTML
    Handle css = GetResource('TEXT', r_TEXT_css);
    if (css)
    {
        HLock(css);
        init_web_views(*css);
        ReleaseResource(css);
    }
    else
        fatal_error_alert(e_no_resource, has_autopositioning);
#endif

	adjustMenus();
	adjustMenuItems();
}

static void init_toolbox()
{
    // TODO: Expand stack here if needed.
    //expand_stack(32 * 1024);
    // Some of the below functions allocate non-relocatable memory so
    // we're not supposed to unload the segment containing this function
    // since that would fragment memory.
    MaxApplZone();
    // TODO: figure out how many masters we need
    //this_many_more_masters(64);
    InitGraf(&qd.thePort);
    InitFonts();
    InitWindows();
    InitMenus();
    TEInit();
    InitDialogs(nil);
    FlushEvents(everyEvent, 0);
    InitCursor();
}

int main()
{
    init_toolbox();
    init_app();
    doNewCmd(true);
    doEventLoop();
    ExitToShell();
}
