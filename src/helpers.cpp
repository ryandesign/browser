// SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
//
// SPDX-License-Identifier: MIT

#include "helpers.h"

#include <cstdio>
#include <string>

bool is_desk_accessory_window(WindowPtr window)
{
    return window && is_desk_accessory_window(*reinterpret_cast<WindowPeek>(window));
}

bool is_desk_accessory_window(WindowRecord const& window)
{
    return window.windowKind < 0;
}

int16_t rect_height(Rect const& rect) {
    return rect.bottom - rect.top;
}

int16_t rect_width(Rect const& rect) {
    return rect.right - rect.left;
}

std::string& pappend(std::string& dst, ConstStr255Param src)
{
    return dst.append(reinterpret_cast<const char *>(&src[1]), src[0]);
}

void debugprintf(char const *format, ...)
{
    va_list args;
    va_start(args, format);
    Str255 str;
    int result = vsnprintf(reinterpret_cast<char *>(&str[1]), sizeof(str), format, args);
    va_end(args);
    if (result >= 0)
    {
        str[0] = result >= sizeof(str) ? sizeof(str) - 1 : result;
        DebugStr(str);
    }
}
