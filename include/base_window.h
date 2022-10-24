// SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
//
// SPDX-License-Identifier: MIT

#ifndef WINDOW_H
#define WINDOW_H

#include <MacWindows.h>

class base_window
{
    public:
        static constexpr int16_t k_default_minimum_height = 80;
        static constexpr int16_t k_default_minimum_width = 80;
        static constexpr int16_t k_default_maximum_height = 32766;
        static constexpr int16_t k_default_maximum_width = 32766;

        base_window(int16_t resource_id);
        virtual ~base_window();
        static base_window *get_from_window(const WindowPtr window);
        static base_window *get_from_window(WindowRecord const& window);
        virtual int16_t get_minimum_height();
        virtual int16_t get_minimum_width();
        virtual int16_t get_maximum_height();
        virtual int16_t get_maximum_width();
        virtual void did_activate(EventRecord const& event);
        virtual void did_deactivate(EventRecord const& event);
        // TODO: Do windows really need to be able to adjust and handle menus?
        virtual void adjust_menu_bar();
        virtual void adjust_menu_items();
        virtual void on_click(EventRecord const& event);
        virtual void idle(EventRecord const& event);
        virtual void on_key_down(EventRecord const& event);
        virtual void on_menu(int16_t menu, int16_t item);
        virtual void did_resize(int16_t dx, int16_t dy);
        virtual void did_zoom(int16_t dx, int16_t dy, int16_t in_or_out);
        virtual void update(EventRecord const& event);
        virtual bool should_close();
        void select();
        void show();
        void hide();

    protected:
        WindowRecord m_window;

    private:
        static constexpr intptr_t k_move_to_front = -1L;
        static constexpr int16_t k_object_window_kind = 0x4242;
};

#endif
