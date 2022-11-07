// SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
//
// SPDX-License-Identifier: MIT

#include <ControlDefinitions.h>
#include <OSUtils.h>

#include "user_control.h"

user_control::user_control(int16_t resource_id, WindowRecord& window)
    : base_control(resource_id, window)
{
    OSErr err;

    {
        ControlUserPaneDrawUPP draw_upp = NewControlUserPaneDrawUPP(draw_proc);
        err = SetControlData(m_control, 0, kControlUserPaneDrawProcTag, sizeof(draw_upp), reinterpret_cast<Ptr>(&draw_upp));
        DisposeRoutineDescriptor(reinterpret_cast<UniversalProcPtr>(draw_upp));
    }
    {
        ControlUserPaneActivateUPP activate_upp = NewControlUserPaneActivateUPP(activate_proc);
        err = SetControlData(m_control, 0, kControlUserPaneActivateProcTag, sizeof(activate_upp), reinterpret_cast<Ptr>(&activate_upp));
        DisposeRoutineDescriptor(reinterpret_cast<UniversalProcPtr>(activate_upp));
    }
}

user_control::~user_control()
{
}

void user_control::draw_proc(ControlHandle control, int16_t part)
{
    if (user_control *control_obj = static_cast<user_control *>(get_from_control(control)))
        control_obj->draw(part);
}

void user_control::draw(int16_t part) const
{
}

void user_control::activate_proc(ControlHandle control, Boolean activating)
{
    if (user_control *control_obj = static_cast<user_control *>(get_from_control(control)))
        control_obj->activate(activating);
}

void user_control::activate(bool activating)
{
}
