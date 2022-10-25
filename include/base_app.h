// SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
//
// SPDX-License-Identifier: MIT

#ifndef BASE_APP_H
#define BASE_APP_H

#include <Events.h>
#include <MacTypes.h>
#include <MacWindows.h>
#include <Patches.h>

// TODO: Reevaluate visibility in all classes. Probably make most things private.
class base_app
{
    public:
        base_app(int32_t extra_stack_bytes = 0, int16_t extra_master_pointers = 0);
        virtual ~base_app();
        void run();
        bool has_128k_rom();
        bool has_gestalt();
        int16_t get_system_version();
        bool has_appearance();
        bool trap_available(uint16_t trap);
        TrapType get_trap_type(uint16_t trap);
        int16_t get_num_toolbox_traps();
        OSErr load_menu_bar(int16_t mbar_id);
        void consume_event();
        void on_event(EventRecord const& event);
        void on_idle_event(EventRecord const& event);
        void on_mouse_down_event(EventRecord const& event);
        void content_click(WindowRecord const& window, EventRecord const& event);
        void grow_window(WindowRecord& window, EventRecord const& event);
        void close_window(WindowRecord& window);
        void zoom_window(WindowRecord& window, int16_t in_or_out);
        void on_key_down_event(EventRecord const& event);
        void on_activate_event(EventRecord const& event);
        void activate_window(WindowRecord& window, bool activate, EventRecord const& event);
        void on_update_event(EventRecord const& event);
        void on_disk_event(EventRecord const& event);
        void on_os_event(EventRecord const& event);
        void on_high_level_event(EventRecord const& event);
        void on_suspend_resume_event(EventRecord const& event);
        void on_menu_event(int32_t menu_result);
        void set_menu_enabled(MenuHandle menu, bool enabled);
        void set_menu_item_enabled(MenuHandle menu, int16_t item, bool enabled);
        void quit();

    protected:
        virtual void try_consume_event();
        virtual uint32_t get_sleep();
        virtual RgnHandle get_cursor_region();
        virtual void adjust_menu_bar();
        virtual void adjust_menu_items();
        virtual void on_menu(int16_t menu_id, int16_t menu_item);

    private:
        static constexpr uint32_t k_visual_delay = 8;
        static constexpr uint32_t k_sleep_time = 0x7FFFFFFF;
        static constexpr int16_t i_about = 1;

        void expand_stack(int32_t extra_bytes);
        void more_masters(int16_t num_masters);
        virtual void will_suspend();
        virtual void will_resume();
        virtual void about();

        uint32_t m_menu_unhighlight_ticks;
        int16_t m_apple_menu_id;
        int16_t m_num_apple_menu_items;
        bool m_done;
        bool m_is_in_foreground;
};

#endif
