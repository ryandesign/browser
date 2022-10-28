// SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
//
// SPDX-License-Identifier: MIT

#include "base_app.h"

#include <Appearance.h>
#include <Controls.h>
#include <Devices.h>
#include <Dialogs.h>
#include <DiskInit.h>
#include <Fonts.h>
#include <MacMemory.h>
#include <Menus.h>
#include <Quickdraw.h>
#include <Resources.h>
#include <ToolUtils.h>

#include "base_window.h"
#include "helpers.h"
#include "machine.h"

base_app::base_app(int32_t extra_stack_bytes, int16_t extra_master_pointers)
    : m_done(false), m_is_in_foreground(true), m_menu_unhighlight_ticks(0)
{
    expand_stack(extra_stack_bytes);
    MaxApplZone();
    more_masters(extra_master_pointers);
    InitGraf(&qd.thePort);
    InitFonts();
    InitWindows();
    InitMenus();
    TEInit();
    InitDialogs(nullptr);
    FlushEvents(everyEvent, 0);
    InitCursor();
    GetDateTime(reinterpret_cast<uint32_t *>(&qd.randSeed));

    if (machine::has_appearance())
        RegisterAppearanceClient();
}

base_app::~base_app()
{
}

void base_app::run()
{
    while (!m_done)
        try_consume_event();
}

typedef struct mbar {
    int16_t length;
    int16_t resource_id[];
} mbar;
typedef mbar *mbar_ptr;
typedef mbar **mbar_handle;

OSErr base_app::load_menu_bar(int16_t mbar_id)
{
    {
        mbar_handle mbar = reinterpret_cast<mbar_handle>(Get1Resource('MBAR', mbar_id));
        if (!mbar)
            return resNotFound;
        m_apple_menu_id = 0;
        m_num_apple_menu_items = 0;
        for (int i = 0; i < (**mbar).length; ++i)
        {
            MenuHandle menu = reinterpret_cast<MenuHandle>(Get1Resource('MENU', (**mbar).resource_id[i]));
            if (!menu)
                return resNotFound;
            if (EqualString((**menu).menuData, "\p\x14", true, true))
            {
                m_apple_menu_id = (**menu).menuID;
                break;
            }
        }
    }

    if (!m_apple_menu_id)
        return resNotFound;

    {
        Handle menu_bar = GetNewMBar(mbar_id);
        if (!menu_bar)
            return resNotFound;
        SetMenuBar(menu_bar);
        DisposeHandle(reinterpret_cast<Handle>(menu_bar));
        MenuHandle menu = GetMenuHandle(m_apple_menu_id);
        m_num_apple_menu_items = CountMItems(menu);
        AppendResMenu(menu, 'DRVR');
        InvalMenuBar();
    }

    return noErr;
}

void base_app::consume_event()
{
    EventRecord event;

    if (WaitNextEvent(everyEvent, &event, get_sleep(), get_cursor_region()))
        on_event(event);
    else
        on_idle_event(event);

    if (m_menu_unhighlight_ticks && TickCount() >= m_menu_unhighlight_ticks)
    {
        HiliteMenu(0);
        m_menu_unhighlight_ticks = 0;
    }
}

void base_app::on_event(EventRecord const& event)
{
    WindowPeek window;

    switch (event.what)
    {
        case mouseDown:
            on_mouse_down_event(event);
            break;
        case keyDown:
        case autoKey:
            on_key_down_event(event);
            break;
        case activateEvt:
            on_activate_event(event);
            break;
        case updateEvt:
            on_update_event(event);
            break;
        case diskEvt:
            on_disk_event(event);
            break;
        case osEvt:
            on_os_event(event);
            break;
        case kHighLevelEvent:
            on_high_level_event(event);
            break;
    }
}

void base_app::on_idle_event(EventRecord const& event)
{
    if (base_window *window_obj = base_window::get_from_window(FrontWindow()))
        window_obj->idle(event);
}

void base_app::on_mouse_down_event(EventRecord const& event)
{
    short part;
    WindowPtr window;

    part = FindWindow(event.where, &window);
    switch (part)
    {
        case inMenuBar:
            adjust_menu_items();
            on_menu_event(MenuSelect(event.where));
            break;
        case inSysWindow:
            SystemClick(&event, window);
            break;
        case inContent:
            if (FrontWindow() == window)
                content_click(*reinterpret_cast<WindowPeek>(window), event);
            else
                SelectWindow(window);
            break;
        case inDrag:
            DragWindow(window, event.where, &(**GetGrayRgn()).rgnBBox);
            break;
        case inGrow:
            grow_window(*reinterpret_cast<WindowPeek>(window), event);
            break;
        case inGoAway:
            if (TrackGoAway(window, event.where))
                close_window(*reinterpret_cast<WindowPeek>(window));
            break;
        case inZoomIn:
        case inZoomOut:
            if (TrackBox(window, event.where, part))
                zoom_window(*reinterpret_cast<WindowPeek>(window), part);
            break;
    }
}

void base_app::content_click(WindowRecord const& window, EventRecord const& event)
{
    if (base_window *window_obj = base_window::get_from_window(window))
        window_obj->on_click(event);
}

void base_app::grow_window(WindowRecord& window, EventRecord const& event)
{
    int16_t min_width, min_height, max_width, max_height;
    base_window *window_obj = base_window::get_from_window(window);
    if (window_obj)
    {
        min_width = window_obj->get_minimum_width();
        min_height = window_obj->get_minimum_height();
        max_width = window_obj->get_maximum_width();
        max_height = window_obj->get_maximum_height();
    }
    else
    {
        min_width = base_window::k_default_minimum_width;
        min_height = base_window::k_default_minimum_height;
        max_width = base_window::k_default_maximum_width;
        max_height = base_window::k_default_maximum_height;
    }
    Rect limit_rect;
    SetRect(&limit_rect, min_width, min_height, max_width, max_height);
    if (int32_t new_size = GrowWindow(reinterpret_cast<WindowPtr>(&window), event.where, &limit_rect))
    {
        int16_t new_width = LoWord(new_size);
        int16_t new_height = HiWord(new_size);
        int16_t dx = new_width - rect_width(window.port.portRect);
        int16_t dy = new_height - rect_height(window.port.portRect);
        SizeWindow(reinterpret_cast<WindowPtr>(&window), new_width, new_height, true);
        if (window_obj)
            window_obj->did_resize(dx, dy);
    }
}

void base_app::close_window(WindowRecord& window)
{
    if (is_desk_accessory_window(window))
        CloseDeskAcc(window.windowKind);
    else if (base_window *window_obj = base_window::get_from_window(window))
    {
        if (window_obj->should_close())
            window_obj->close();
    }
    else
        DisposeWindow(reinterpret_cast<WindowPtr>(&window));
    adjust_menu_bar();
}

void base_app::zoom_window(WindowRecord& window, int16_t in_or_out)
{
    GrafPtr saved_port;
    GetPort(&saved_port);
    SetPort(reinterpret_cast<WindowPtr>(&window));
    EraseRect(&window.port.portRect);
    int16_t dx = rect_width(window.port.portRect);
    int16_t dy = rect_height(window.port.portRect);
    ZoomWindow(reinterpret_cast<WindowPtr>(&window), in_or_out, true);
    if (base_window *window_obj = base_window::get_from_window(window))
    {
        dx = rect_width(window.port.portRect) - dx;
        dy = rect_height(window.port.portRect) - dy;
        window_obj->did_zoom(dx, dy, in_or_out);
    }
    SetPort(saved_port);
}

void base_app::on_key_down_event(EventRecord const& event)
{
    if (event.modifiers & cmdKey)
    {
        if (keyDown == event.what)
        {
            adjust_menu_items();
            int32_t menu_result;
            if (machine::has_appearance())
                menu_result = MenuEvent(&event);
            else
                menu_result = MenuKey(event.message & charCodeMask);
            on_menu_event(menu_result);
        }
    }
    else if (base_window *window_obj = base_window::get_from_window(FrontWindow()))
        window_obj->on_key_down(event);
}

void base_app::on_activate_event(EventRecord const& event)
{
    if (WindowPeek window = reinterpret_cast<WindowPeek>(event.message))
        activate_window(*window, event.modifiers & activeFlag, event);
}

void base_app::activate_window(WindowRecord& window, bool activate, EventRecord const& event)
{
    if (base_window *window_obj = base_window::get_from_window(window))
    {
        ControlHandle control;
        OSErr err = GetRootControl(reinterpret_cast<WindowPtr>(&window), &control);
        if (activate)
        {
            if (err == noErr)
                ActivateControl(control);
            window_obj->did_activate(event);
        }
        else
        {
            if (err == noErr)
                DeactivateControl(control);
            window_obj->did_deactivate(event);
        }
    }
}

void base_app::on_update_event(EventRecord const& event)
{
    if (WindowPtr window = reinterpret_cast<WindowPtr>(event.message))
    {
        BeginUpdate(window);
        GrafPtr saved_port;
        GetPort(&saved_port);
        SetPort(window);
        if (base_window *window_obj = base_window::get_from_window(window))
            window_obj->update(event);
        UpdateControls(window, window->visRgn);
        if (!machine::has_appearance())
        {
            // TODO: set clip to grow box region
            DrawGrowIcon(window);
            // TODO: restore clip
        }
        SetPort(saved_port);
        EndUpdate(window);
    }
}

void base_app::on_disk_event(EventRecord const& event)
{
    if (HiWord(event.message))
    {
        // System 7 and later ignores this point and centers the dialog.
        Point top_left;
        SetPt(&top_left, 98, 98);
        DILoad();
        OSErr err = DIBadMount(top_left, event.message);
        DIUnload();
    }
}

void base_app::on_os_event(EventRecord const& event)
{
    switch ((event.message >> 24) & 0xFF)
    {
        case mouseMovedMessage:
            on_idle_event(event);
            break;
        case suspendResumeMessage:
            on_suspend_resume_event(event);
            break;
    }
}

void base_app::on_high_level_event(EventRecord const& event)
{
}

void base_app::on_suspend_resume_event(EventRecord const& event)
{
    m_is_in_foreground = event.message & resumeFlag;
    if (m_is_in_foreground)
        will_suspend();
    else
        will_resume();
    if (WindowPeek window = reinterpret_cast<WindowPeek>(FrontWindow()))
        activate_window(*window, m_is_in_foreground, event);
}

void base_app::on_menu_event(int32_t menu_result)
{
    int16_t menu_id = HiWord(menu_result);
    m_menu_unhighlight_ticks = TickCount();
    if (menu_id)
    {
        bool handled = false;
        m_menu_unhighlight_ticks += k_visual_delay;
        int16_t menu_item = LoWord(menu_result);
        if (menu_id == m_apple_menu_id)
        {
            if (menu_item > m_num_apple_menu_items)
            {
                if (MenuHandle menu = GetMenuHandle(menu_id))
                {
                    Str255 item_name;
                    GetMenuItemText(menu, menu_item, item_name);
                    int16_t ref_num = OpenDeskAcc(item_name);
                    adjust_menu_bar();
                }
                handled = true;
            }
            else if (menu_item == i_about)
            {
                about();
                handled = true;
            }
        }
        if (!handled)
            on_menu(menu_id, menu_item);
    }
}

void base_app::set_menu_enabled(MenuHandle menu, bool enabled)
{
    set_menu_item_enabled(menu, 0, enabled);
}

void base_app::set_menu_item_enabled(MenuHandle menu, int16_t item, bool enabled)
{
    if (menu && *menu)
    {
        if (0 == item && enabled != (**menu).enableFlags & 1L)
            InvalMenuBar();
        if (enabled)
            EnableItem(menu, item);
        else
            DisableItem(menu, item);
    }
}

void base_app::quit()
{
    m_done = true;
}

void base_app::try_consume_event()
{
    try
    {
        consume_event();
    }
    catch (...)
    {
        Debugger();
    }
}

uint32_t base_app::get_sleep()
{
    if (m_menu_unhighlight_ticks > 0)
        return m_menu_unhighlight_ticks - TickCount();
    return k_sleep_time;
}

RgnHandle base_app::get_cursor_region()
{
    return nullptr;
}

void base_app::adjust_menu_bar()
{
    if (base_window *window_obj = base_window::get_from_window(FrontWindow()))
        window_obj->adjust_menu_bar();
}

void base_app::adjust_menu_items()
{
    if (base_window *window_obj = base_window::get_from_window(FrontWindow()))
        window_obj->adjust_menu_items();
}

void base_app::on_menu(int16_t menu_id, int16_t menu_item)
{
}

void base_app::expand_stack(int32_t extra_stack_bytes)
{
    if (extra_stack_bytes)
        SetApplLimit(GetApplLimit() - extra_stack_bytes);
}

void base_app::more_masters(int16_t extra_master_pointers)
{
    if (extra_master_pointers > 0)
    {
        THz zone = GetZone();
        int16_t saved_num_masters = zone->moreMast;
        zone->moreMast = extra_master_pointers;
        MoreMasters();
        zone->moreMast = saved_num_masters;
    }
}

void base_app::will_suspend()
{
}

void base_app::will_resume()
{
}

void base_app::about()
{
}
