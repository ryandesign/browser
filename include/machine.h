// SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
//
// SPDX-License-Identifier: MIT

#ifndef MACHINE_H
#define MACHINE_H

#include <Patches.h>
#include <cstdint>

namespace machine
{
    bool has_128k_rom();
    bool has_gestalt();
    int16_t get_system_version();
    bool has_appearance();
    bool trap_available(uint16_t trap);
    TrapType get_trap_type(uint16_t trap);
    int16_t get_num_toolbox_traps();
}

#endif
