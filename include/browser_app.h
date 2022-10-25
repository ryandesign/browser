// SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
//
// SPDX-License-Identifier: MIT

#ifndef BROWSER_APP_H
#define BROWSER_APP_H

#include "base_app.h"

class browser_app : public base_app
{
    public:
        browser_app();
        ~browser_app() override;

        void fatal_error_alert(int16_t error_number);
        void error_alert(int16_t error_number);
//        void oserr_alert(OSErr err);
        void on_file_menu(int16_t menu_item);
        void on_edit_menu(int16_t menu_item);
        void on_window_menu(int16_t menu_item);
        void on_quit();

    protected:
        void try_consume_event() override;
        void adjust_menu_bar() override;
        void adjust_menu_items() override;
        void on_menu(int16_t menu_id, int16_t menu_item) override;

    private:
        void about() override;
};

#endif
