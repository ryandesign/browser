// SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
//
// SPDX-License-Identifier: MIT

#ifndef BASE_CONTROL_H
#define BASE_CONTROL_H

#include <Controls.h>
#include <MacWindows.h>

class base_control
{
    friend class root_control;

    public:
        base_control(int16_t resource_id, WindowRecord& window);
//        base_control(ControlHandle control);
        virtual ~base_control();
        static base_control *get_from_control(const ControlHandle control);
        static base_control *get_from_control(ControlRecord const& control);
        bool is_visible();
        void show();
        void hide();
        OSErr set_data(ControlPartCode part, ResType tag, size_t size, void const* data);
        void window_did_resize(int16_t dx, int16_t dy);

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
