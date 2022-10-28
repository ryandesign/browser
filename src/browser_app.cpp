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
#include "about_window.h"
#include "browser_window.h"
#include "helpers.h"
#include "machine.h"
//#include "oserr_exception.h"

browser_app::browser_app() : base_app()
{
    if (!machine::has_appearance())
        fatal_error_alert(e_no_appearance);

    if (load_menu_bar(r_MBAR) != noErr)
        fatal_error_alert(e_no_resource);

#ifdef USE_LITEHTML
    {
        Handle css = Get1Resource('TEXT', r_TEXT_css);
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
    bool has_autopositioning = machine::get_system_version() >= 0x0700;
    AlertTHndl alrt;
    Point offset;
    if (!has_autopositioning)
    {
        if (machine::has_128k_rom())
            alrt = reinterpret_cast<AlertTHndl>(Get1Resource('ALRT', r_ALRT_fatal_error));
        else
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
    static about_window *window_obj = nullptr;
    if (window_obj)
    {
        window_obj->show();
        window_obj->select();
    }
    else
        window_obj = new about_window();
    adjust_menu_bar();
}

void browser_app::on_file_menu(int16_t menu_item)
{
    switch (menu_item)
    {
        case i_new_window:
            new browser_window();
            adjust_menu_bar();
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

void browser_app::on_window_menu(int16_t menu_item)
{
    switch (menu_item)
    {
        case i_collapse:
            if (WindowPtr window = FrontWindow())
                CollapseWindow(window, !IsWindowCollapsed(window));
            break;
        case i_zoom:
            if (WindowPeek window = reinterpret_cast<WindowPeek>(FrontWindow()))
                zoom_window(*window, EqualRect(&(**window->contRgn).rgnBBox, &(**reinterpret_cast<WStateDataHandle>(window->dataHandle)).userState) ? inZoomOut : inZoomIn);
            break;
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
    set_menu_enabled(GetMenuHandle(k_edit_menu_id), is_desk_accessory_window(window));
    set_menu_enabled(GetMenuHandle(k_window_menu_id), nullptr != window);
}

void browser_app::adjust_menu_items()
{
    WindowPtr window = FrontWindow();
    uint32_t features;
    bool can_zoom, can_collapse, is_collapsed;
    if (window && (noErr == GetWindowFeatures(window, &features)))
    {
        can_zoom = features & kWindowCanZoom;
        can_collapse = features & kWindowCanCollapse;
        is_collapsed = IsWindowCollapsed(window);
    }
    else
    {
        can_zoom = false;
        can_collapse = false;
        is_collapsed = false;
    }
    MenuHandle menu = GetMenuHandle(k_file_menu_id);
    set_menu_item_enabled(menu, i_close_window, nullptr != window);
    menu = GetMenuHandle(k_window_menu_id);
    set_menu_item_enabled(menu, i_zoom, can_zoom);
    set_menu_item_enabled(menu, i_collapse, can_collapse);
    Str255 menu_item_text;
    GetIndString(menu_item_text, r_STRx_menu_items, is_collapsed ? k_expand : k_collapse);
    SetMenuItemText(menu, i_collapse, menu_item_text);
}

void browser_app::on_menu(int16_t menu_id, int16_t menu_item)
{
    switch (menu_id)
    {
        case k_file_menu_id:
            on_file_menu(menu_item);
            break;
        case k_edit_menu_id:
            on_edit_menu(menu_item);
            break;
        case k_window_menu_id:
            on_window_menu(menu_item);
            break;
    }
}
