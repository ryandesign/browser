// SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
//
// SPDX-License-Identifier: MIT

#include "base_window.h"

#include <new>

//#include "oserr_exception.h"

base_window::base_window(int16_t resource_id)
{
    if (!GetNewCWindow(resource_id, &m_window, reinterpret_cast<WindowPtr>(k_move_to_front)))
        throw std::bad_alloc()/*oserr_exception(memFullErr)*/;
    m_window.windowKind = k_object_window_kind;
    m_window.refCon = reinterpret_cast<int32_t>(this);
}

base_window::~base_window()
{
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

void base_window::did_resize(int16_t dx, int16_t dy)
{
}

void base_window::did_zoom(int16_t dx, int16_t dy, int16_t in_or_out)
{
}

void base_window::update(EventRecord const& event)
{
}

bool base_window::can_close()
{
    return true;
}

void base_window::show()
{
    ShowWindow(reinterpret_cast<WindowPtr>(&m_window));
}

void base_window::hide()
{
    HideWindow(reinterpret_cast<WindowPtr>(&m_window));
}
