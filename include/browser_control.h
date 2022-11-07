// SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
//
// SPDX-License-Identifier: MIT

#ifndef BROWSER_CONTROL_H
#define BROWSER_CONTROL_H

#include <memory>

#ifdef USE_LITEHTML
#include "quickdraw_container.h"
#endif
#include "user_control.h"

class browser_document;

class browser_control :
#ifdef USE_LITEHTML
    public quickdraw_container,
#endif
    public user_control
{
    public:
        browser_control(int16_t resource_id, WindowRecord& window);
        ~browser_control();
        void set_document(std::shared_ptr<browser_document> document);

#ifdef USE_LITEHTML
        // quickdraw_container overrides:
        void get_client_rect(litehtml::position& client) const override;
        void set_caption(char const *caption) override;
#endif

        // user_control overrides:
        virtual void draw(int16_t part) const override;

    private:
        std::shared_ptr<browser_document> m_document;
};

#endif
