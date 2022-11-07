// SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
//
// SPDX-License-Identifier: MIT

#ifndef USER_CONTROL_H
#define USER_CONTROL_H

#include "base_control.h"

class user_control : public base_control
{
    public:
        user_control(int16_t resource_id, WindowRecord& window);
        ~user_control();

    private:
        static pascal void draw_proc(ControlHandle control, int16_t part);
        virtual void draw(int16_t part) const;
        static pascal void activate_proc(ControlHandle control, Boolean activating);
        virtual void activate(bool activating);
};

#endif
