// SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
//
// SPDX-License-Identifier: MIT

// Mac headers
//#include <NumberFormatting.h>
#include <Script.h>
#include <TextUtils.h>

// System headers

// Third-party headers

// My headers
#include "quickdraw_container.h"
#include "quickdraw_font.h"

quickdraw_container::quickdraw_container()
{
}

quickdraw_container::~quickdraw_container()
{
}

litehtml::uint_ptr quickdraw_container::create_font(const char *face_name, int size, int weight, litehtml::font_style italic_style, unsigned int decoration, litehtml::font_metrics *fm)
{
    StyleParameter style = normal;
    if (weight >= 700)
        style |= bold;
    if (litehtml::font_style_italic == italic_style)
        style |= italic;
    if (decoration & litehtml::font_decoration_underline)
        style |= underline;
    quickdraw_font *font = new quickdraw_font(face_name, size, style);
    FontInfo& metrics = font->metrics();
    fm->height = metrics.ascent + metrics.descent + metrics.leading;
    fm->ascent = metrics.ascent;
    fm->descent = metrics.descent;
    // TODO: find real x-height
    fm->x_height = fm->height / 2;
    return reinterpret_cast<litehtml::uint_ptr>(font);
}

void quickdraw_container::delete_font(litehtml::uint_ptr hFont)
{
    quickdraw_font *font = reinterpret_cast<quickdraw_font *>(hFont);
    delete font;
}

int quickdraw_container::text_width(const char *text, litehtml::uint_ptr hFont)
{
    quickdraw_font *font = reinterpret_cast<quickdraw_font *>(hFont);
    return font->width(text);
}

void quickdraw_container::draw_text(litehtml::uint_ptr hdc, const char *text, litehtml::uint_ptr hFont, litehtml::web_color color, const litehtml::position& pos)
{
    quickdraw_font *font = reinterpret_cast<quickdraw_font *>(hFont);
    font->draw(text, pos);
}

int quickdraw_container::pt_to_px(int pt) const
{
    return pt;
}

int quickdraw_container::get_default_font_size() const
{
    return pt_to_px(12);
}

const char *quickdraw_container::get_default_font_name() const
{
    return "Helvetica";
}

void quickdraw_container::draw_list_marker(litehtml::uint_ptr hdc, const litehtml::list_marker& marker)
{
}

void quickdraw_container::load_image(const char *src, const char *base_url, bool redraw_on_ready)
{
}

void quickdraw_container::get_image_size(const char *src, const char *base_url, litehtml::size& sz)
{
}

void quickdraw_container::draw_background(litehtml::uint_ptr hdc, const litehtml::background_paint& bg)
{
}

void quickdraw_container::draw_borders(litehtml::uint_ptr hdc, const litehtml::borders& borders, const litehtml::position& draw_pos, bool root)
{
}

void quickdraw_container::set_base_url(const char *base_url)
{
}

void quickdraw_container::link(const std::shared_ptr<litehtml::document>& doc, const litehtml::element::ptr& el)
{
}

void quickdraw_container::on_anchor_click(const char *url, const litehtml::element::ptr& el)
{
}

void quickdraw_container::set_cursor(const char *cursor)
{
}

void quickdraw_container::transform_text(litehtml::string& text, litehtml::text_transform tt)
{
    if (!text.empty())
    {
        switch (tt)
        {
            case litehtml::text_transform_capitalize:
                UppercaseText(&text[0], 1, smCurrentScript);
                break;
            case litehtml::text_transform_uppercase:
                UppercaseText(&text[0], text.length(), smCurrentScript);
                break;
            case litehtml::text_transform_lowercase:
                LowercaseText(&text[0], text.length(), smCurrentScript);
                break;
        }
    }
}

void quickdraw_container::import_css(litehtml::string& text, const litehtml::string& url, litehtml::string& base_url)
{
}

void quickdraw_container::set_clip(const litehtml::position& pos, const litehtml::border_radiuses& radiuses, bool valid_x, bool valid_y)
{
}

void quickdraw_container::del_clip()
{
}

std::shared_ptr<litehtml::element> quickdraw_container::create_element(const char *tag_name, const litehtml::string_map &attributes, const std::shared_ptr<litehtml::document> &doc)
{
    return nullptr;
}

void quickdraw_container::get_media_features(litehtml::media_features& media) const
{
}

void quickdraw_container::get_language(litehtml::string& language, litehtml::string& culture) const
{
}
