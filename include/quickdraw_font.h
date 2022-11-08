// SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
//
// SPDX-License-Identifier: MIT

#ifndef QUICKDRAW_FONT_H
#define QUICKDRAW_FONT_H

// Mac headers
#include <Fonts.h>
#include <MacTypes.h>

// System headers

// Third-party headers
#include <litehtml.h>

// My headers

class quickdraw_font
{
    public:
        quickdraw_font(const char *name, int16_t size, StyleParameter style = normal);
        quickdraw_font(int16_t id, int16_t size, StyleParameter style = normal);
        ~quickdraw_font();
        void draw(const char *text, const litehtml::position& pos);
        int16_t width(const char *text);
        FontInfo& metrics();

    private:
        int16_t m_id;
        int16_t m_size;
        StyleParameter m_style;
        FontInfo m_metrics;
        GrafPort m_port;

        void construct();
        void set_port_font();
};

#endif
