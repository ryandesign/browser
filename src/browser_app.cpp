// SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
//
// SPDX-License-Identifier: MIT

#include "browser_app.h"

#include <Dialogs.h>
#include <LowMem.h>
#include <MacWindows.h>
#include <Resources.h>
#include <TextUtils.h>

#include "ResourceConstants.h"
#include "WebView.h"
#include "browser_window.h"
#include "helpers.h"
//#include "oserr_exception.h"

browser_app::browser_app() : base_app()
{
    if (!has_appearance())
        fatal_error_alert(e_no_appearance);

    if (load_menu_bar(r_MBAR) != noErr)
        fatal_error_alert(e_no_resource);

#ifdef USE_LITEHTML
    {
        Handle css = GetResource('TEXT', r_TEXT_css);
        if (css)
        {
            HLock(css);
            init_web_views(*css);
            ReleaseResource(css);
        }
        else
            fatal_error_alert(e_no_resource);
    }
#endif

    adjust_menu_bar();
    adjust_menu_items();
}

browser_app::~browser_app()
{
}

void browser_app::fatal_error_alert(int16_t error_number)
{
#ifdef __m68k__
    bool has_autopositioning = get_system_version() >= 0x0700;
    AlertTHndl alrt;
    Point offset;
    if (!has_autopositioning)
    {
        alrt = reinterpret_cast<AlertTHndl>(GetResource('ALRT', r_ALRT_fatal_error));
        if (alrt)
        {
            offset.h = (rect_width(qd.screenBits.bounds) - rect_width((**alrt).boundsRect) >> 1) - (**alrt).boundsRect.left;
            offset.v = LMGetMBarHeight() + 64 - (**alrt).boundsRect.top;
            OffsetRect(&(**alrt).boundsRect, offset.h, offset.v);
        }
    }
#endif

    SetCursor(&qd.arrow);
    Str255 error_message;
    GetIndString(error_message, r_STRx_error_messages, error_number);
    ParamText(error_message, "\p", "\p", "\p");
    int16_t item_hit = StopAlert(r_ALRT_fatal_error, nil);

#ifdef __m68k__
    if (!has_autopositioning && alrt)
        OffsetRect(&(**alrt).boundsRect, -offset.h, -offset.v);
#endif

    ExitToShell();
}

void browser_app::error_alert(int16_t error_number)
{
    int16_t item_hit;
    Str255 error_message;
    OSErr err;
    AlertStdAlertParamRec params;

    params.movable = false;
    params.helpButton = false;
    params.filterProc = nil;
    params.defaultText = reinterpret_cast<StringPtr>(kAlertDefaultOKText);
    params.cancelText = nil;
    params.otherText = nil;
    params.defaultButton = kAlertStdAlertOKButton;
    params.cancelButton = 0;
    params.position = kWindowDefaultPosition;
    GetIndString(error_message, r_STRx_error_messages, error_number);
    err = StandardAlert(kAlertStopAlert, error_message, nil, &params, &item_hit);
}

/*
void browser_app::oserr_alert(OSErr err)
{
    int16_t error_number;

    switch (err)
    {
        case memFullErr:
            error_number = e_no_memory;
            break;
        default:
            error_number = e_unknown_error;
    }

    error_alert(error_number);
}
*/

void browser_app::about()
{
}

void browser_app::on_apple_menu(int16_t menu_item)
{
    switch (menu_item)
    {
        case i_about:
            about();
            break;
    }
}

void browser_app::on_file_menu(int16_t menu_item)
{
    switch (menu_item)
    {
        case i_new_window:
            new browser_window();
            break;
        case i_close_window:
            if (WindowPtr window = FrontWindow())
                close_window(*reinterpret_cast<WindowPeek>(window));
            break;
        case i_quit:
            on_quit();
            break;
    }
}

void browser_app::on_edit_menu(int16_t menu_item)
{
    if (!SystemEdit(menu_item - 1))
    {
    }
}

void browser_app::on_quit()
{
    while (WindowPtr window = FrontWindow())
        close_window(*reinterpret_cast<WindowPeek>(window));
    quit();
}

void browser_app::try_consume_event()
{
    try
    {
        consume_event();
    }
/*
    catch (oserr_exception const& e)
    {
        oserr_alert(e.err());
    }
*/
    catch (std::bad_alloc const& e)
    {
        error_alert(e_no_memory);
    }
    catch (...)
    {
        error_alert(e_unknown_error);
    }
}

void browser_app::adjust_menu_bar()
{
    WindowPtr window = FrontWindow();
    MenuHandle menu = GetMenuHandle(k_edit_menu_id);
    set_menu_enabled(menu, is_desk_accessory_window(window));
}

void browser_app::adjust_menu_items()
{
    WindowPtr window = FrontWindow();
    MenuHandle menu = GetMenuHandle(k_file_menu_id);
    set_menu_item_enabled(menu, i_close_window, nullptr != window);
}

void browser_app::on_menu(int16_t menu_id, int16_t menu_item)
{
    switch (menu_id)
    {
        case k_apple_menu_id:
            on_apple_menu(menu_item);
            break;
        case k_file_menu_id:
            on_file_menu(menu_item);
            break;
        case k_edit_menu_id:
            on_edit_menu(menu_item);
            break;
    }
}
