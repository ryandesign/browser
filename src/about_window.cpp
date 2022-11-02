// SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
//
// SPDX-License-Identifier: MIT

#include "about_window.h"

#include <ControlDefinitions.h>
#include <MacTypes.h>
#include <Resources.h>
#include <new>
#include <string>

#include "ResourceConstants.h"
#include "helpers.h"

about_window::about_window()
    : base_window(r_WIND_about),
      m_root_control(m_window),
      m_text(r_CNTL_about_text, m_window)
{
    std::string text;
    {
        Str255 app_name;
        GetIndString(app_name, r_STRx_about, i_app_name);
        if (ResError())
            throw std::bad_alloc();
        pappend(text, app_name);
    }
    {
        Handle vers = Get1Resource('vers', r_vers_program);
        if (!vers)
            throw std::bad_alloc();
        text += " ";
        HLock(vers);
        pappend(text, (**reinterpret_cast<VersRecHndl>(vers)).shortVersion);
        ReleaseResource(vers);
    }
    m_text.set_data(kControlNoPart, kControlStaticTextTextTag, text.size(), text.data());
}

about_window::~about_window()
{
}

void about_window::close()
{
    hide();
}
