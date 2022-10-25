// SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
//
// SPDX-License-Identifier: MIT

#ifndef HELPERS_H
#define HELPERS_H

#include <MacWindows.h>
#include <string>

bool is_desk_accessory_window(WindowPtr window);
bool is_desk_accessory_window(WindowRecord const& window);
int16_t rect_height(Rect const& rect);
int16_t rect_width(Rect const& rect);
std::string& pappend(std::string& dst, ConstStr255Param src);
void debugprintf(char const *format, ...);

#endif
