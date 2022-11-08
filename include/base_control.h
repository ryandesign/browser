// SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
//
// SPDX-License-Identifier: MIT

#ifndef BASE_CONTROL_H
#define BASE_CONTROL_H

#include <Controls.h>
#include <MacWindows.h>

#include "base_window.h"

class base_control
{
    friend class root_control;

    public:
        base_control(int16_t resource_id, WindowRecord& window);
//        base_control(ControlHandle control);
        virtual ~base_control();
        static base_control *get_from_control(const ControlHandle control);
        static base_control *get_from_control(ControlRecord const& control);
        Rect const& get_rect() const;
        virtual base_window& get_window();
        bool is_visible() const;
        void show_and_inval();
        void show_and_draw();
        void hide_and_inval();
        void hide_and_draw();
        int16_t get_maximum() const;
        void set_maximum(int16_t maximum);
        int16_t get_minimum() const;
        void set_minimum(int16_t minimum);
        int16_t get_value() const;
        void set_value(int16_t value);
        OSErr set_data(ControlPartCode part, ResType tag, size_t size, void const* data);
        OSErr embed(base_control& control);
        void window_did_resize(int16_t dx, int16_t dy);
        void on_mouse_down(EventRecord const& event, int16_t part);
        void on_mouse_up(EventRecord const& event, int16_t part);
        virtual ControlActionUPP get_action_proc(int16_t part);

    protected:
        ControlHandle m_control;

    private:
        base_control();
        void init();

        bool m_move_horizontally;
        bool m_move_vertically;
        bool m_resize_horizontally;
        bool m_resize_vertically;
};

#endif
