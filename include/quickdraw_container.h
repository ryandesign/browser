// SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
//
// SPDX-License-Identifier: MIT

#ifndef QUICKDRAW_CONTAINER_H
#define QUICKDRAW_CONTAINER_H

// Mac headers

// System headers

// Third-party headers
#include <litehtml.h>

// My headers

class quickdraw_container : public litehtml::document_container
{
    public:
        quickdraw_container();
        ~quickdraw_container();

        litehtml::uint_ptr create_font(const char *face_name, int size, int weight, litehtml::font_style italic_style, unsigned int decoration, litehtml::font_metrics *fm);
        void delete_font(litehtml::uint_ptr hFont);
        int text_width(const char *text, litehtml::uint_ptr hFont);
        void draw_text(litehtml::uint_ptr hdc, const char *text, litehtml::uint_ptr hFont, litehtml::web_color color, const litehtml::position& pos);
        int pt_to_px(int pt) const;
        int get_default_font_size() const;
        const char *get_default_font_name() const;
        void draw_list_marker(litehtml::uint_ptr hdc, const litehtml::list_marker& marker);
        void load_image(const char *src, const char *base_url, bool redraw_on_ready);
        void get_image_size(const char *src, const char *base_url, litehtml::size& sz);
        void draw_background(litehtml::uint_ptr hdc, const litehtml::background_paint& bg);
        void draw_borders(litehtml::uint_ptr hdc, const litehtml::borders& borders, const litehtml::position& draw_pos, bool root);
        void set_base_url(const char *base_url);
        void link(const std::shared_ptr<litehtml::document>& doc, const litehtml::element::ptr& el);
        void on_anchor_click(const char *url, const litehtml::element::ptr& el);
        void set_cursor(const char *cursor);
        void transform_text(litehtml::string& text, litehtml::text_transform tt);
        void import_css(litehtml::string& text, const litehtml::string& url, litehtml::string& base_url);
        void set_clip(const litehtml::position& pos, const litehtml::border_radiuses& radiuses, bool valid_x, bool valid_y);
        void del_clip();
        std::shared_ptr<litehtml::element> create_element(const char *tag_name, const litehtml::string_map &attributes, const std::shared_ptr<litehtml::document> &doc);
        void get_media_features(litehtml::media_features& media) const;
        void get_language(litehtml::string& language, litehtml::string& culture) const;
};

#endif
