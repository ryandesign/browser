// SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
//
// SPDX-License-Identifier: MIT

#include "base_window.h"

#include <Controls.h>
#include <Quickdraw.h>
#include <Resources.h>
#include <new>

#include "machine.h"
//#include "oserr_exception.h"

struct wind {
    Rect bounds;
    int16_t procid;
    // More fields we don't care about.
};
typedef wind *wind_ptr;
typedef wind **wind_handle;

base_window::base_window(int16_t resource_id)
{
    // I can't find a way to get the procid after the window has been created
    // so I load the resource manually and extract the procid from it before
    // creating the window.
    wind_handle wind = reinterpret_cast<wind_handle>(Get1Resource('WIND', resource_id));
    if (!wind)
        throw std::bad_alloc();
    m_procid = (**wind).procid;
    if (!GetNewCWindow(resource_id, &m_window, reinterpret_cast<WindowPtr>(k_move_to_front)))
        throw std::bad_alloc()/*oserr_exception(memFullErr)*/;
    m_window.windowKind = k_object_window_kind;
    m_window.refCon = reinterpret_cast<int32_t>(this);
}

base_window::~base_window()
{
    // Use CloseWindow, not DisposeWindow, because we allocated the memory for
    // the window record (within this object) so we don't want Mac OS to try to
    // dispose it for us.
    CloseWindow(reinterpret_cast<WindowPtr>(&m_window));
}

base_window *base_window::get_from_window(const WindowPtr window)
{
    base_window *window_obj = nullptr;
    if (window)
        window_obj = base_window::get_from_window(*reinterpret_cast<WindowPeek>(window));
    return window_obj;
}

base_window *base_window::get_from_window(WindowRecord const& window)
{
    base_window *window_obj = nullptr;
    if (k_object_window_kind == window.windowKind && window.refCon)
        window_obj = reinterpret_cast<base_window *>(window.refCon);
    return window_obj;
}

int16_t base_window::get_minimum_height()
{
    return k_default_minimum_height;
}

int16_t base_window::get_minimum_width()
{
    return k_default_minimum_width;
}

int16_t base_window::get_maximum_height()
{
    return k_default_maximum_height;
}

int16_t base_window::get_maximum_width()
{
    return k_default_maximum_width;
}

void base_window::did_activate(EventRecord const& event)
{
}

void base_window::did_deactivate(EventRecord const& event)
{
}

void base_window::adjust_menu_bar()
{
}

void base_window::adjust_menu_items()
{
}

void base_window::on_click(EventRecord const& event)
{
}

void base_window::idle(EventRecord const& event)
{
}

void base_window::on_key_down(EventRecord const& event)
{
}

void base_window::on_menu(int16_t menu, int16_t item)
{
}

void base_window::did_resize(int16_t dx, int16_t dy, int16_t part)
{
}

void base_window::update(EventRecord const& event)
{
    UpdateControls(reinterpret_cast<WindowPtr>(&m_window), m_window.port.visRgn);
    draw_grow_icon();
}

bool base_window::has_grow_icon()
{
    bool has_grow;
    uint32_t features;
    if (machine::has_appearance() && noErr == GetWindowFeatures(reinterpret_cast<WindowPtr>(&m_window), &features))
        has_grow = features & kWindowCanGrow;
    else
        switch (m_procid)
        {
            case documentProc:
            case zoomDocProc:
            case floatGrowProc:
            case floatZoomGrowProc:
            case floatSideGrowProc:
            case floatSideZoomGrowProc:
                has_grow = true;
                break;
            default:
                has_grow = false;
        }
    return has_grow;
}

void base_window::get_grow_icon_region(RgnHandle rgn)
{
    if (machine::has_appearance() && noErr == GetWindowRegion(reinterpret_cast<WindowPtr>(&m_window), kWindowGrowRgn, rgn))
    {
        Point offset;
        SetPt(&offset, 0, 0);
        LocalToGlobal(&offset);
        OffsetRgn(rgn, -offset.h, -offset.v);
    }
    else
    {
        Rect rect;
        rect.right = m_window.port.portRect.right + 1;
        rect.bottom = m_window.port.portRect.bottom + 1;
        rect.left = rect.right - 16;
        rect.top = rect.bottom - 16;
        RectRgn(rgn, &rect);
    }
}

void base_window::draw_grow_icon()
{
    // When using Appearance windows directly, calling DrawGrowIcon is not
    // necessary but is presumably not harmful. When using non-Appearance
    // windows in mapping mode, DrawGrowIcon must be called once but it's
    // presumably not harmful to call it every time.
    if (has_grow_icon())
        if (RgnHandle grow_icon_region = NewRgn())
        {
            // Mask out everything but the grow icon itself to avoid drawing the
            // possibly undesirable scroll bar delimiting lines.
            get_grow_icon_region(grow_icon_region);
            RgnHandle saved_clip_region = m_window.port.clipRgn;
            m_window.port.clipRgn = grow_icon_region;
            DrawGrowIcon(reinterpret_cast<WindowPtr>(&m_window));
            m_window.port.clipRgn = saved_clip_region;
            DisposeRgn(grow_icon_region);
        }
}

bool base_window::should_close()
{
    return true;
}

void base_window::close()
{
    delete this;
}

void base_window::select()
{
    SelectWindow(reinterpret_cast<WindowPtr>(&m_window));
}

void base_window::show()
{
    ShowWindow(reinterpret_cast<WindowPtr>(&m_window));
}

void base_window::hide()
{
    HideWindow(reinterpret_cast<WindowPtr>(&m_window));
}
