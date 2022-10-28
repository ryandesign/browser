// SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
//
// SPDX-License-Identifier: MIT

#include "machine.h"

#include <Gestalt.h>
#include <LowMem.h>
#include <ToolUtils.h>
#include <Traps.h>

bool machine::has_128k_rom()
{
#ifdef __m68k__
    return LMGetROM85() >= 0;
#else
    return true;
#endif
}

bool machine::has_gestalt()
{
#ifdef __m68k__
    static int16_t s_has_gestalt = -1;
    if (-1 == s_has_gestalt)
        s_has_gestalt = has_128k_rom() && trap_available(_Gestalt);
    return s_has_gestalt;
#else
    return true;
#endif
}

int16_t machine::get_system_version()
{
    static int16_t s_system_version = -1;
    if (-1 == s_system_version)
    {
        int32_t result;
        if (has_gestalt() && Gestalt(gestaltSystemVersion, &result) != noErr)
            s_system_version = LoWord(result);
        else
            s_system_version = 0;
    }
    return s_system_version;
}

bool machine::has_appearance()
{
    static int16_t s_has_appearance = -1;
    if (-1 == s_has_appearance)
    {
        int32_t result;
        if (has_gestalt() && Gestalt(gestaltAppearanceAttr, &result) == noErr)
            s_has_appearance = LoWord(result) & (1 << gestaltAppearanceExists);
        else
            s_has_appearance = false;
    }
    return s_has_appearance;
}

bool machine::trap_available(uint16_t trap)
{
    TrapType trap_type = get_trap_type(trap);
    if (ToolTrap == trap_type) {
        trap &= 0x03FF;
        if (trap >= get_num_toolbox_traps())
            trap = _Unimplemented;
    }
    return NGetTrapAddress(trap, trap_type) != NGetTrapAddress(_Unimplemented, ToolTrap);
}

TrapType machine::get_trap_type(uint16_t trap)
{
    return (trap & 0x0800) ? ToolTrap : OSTrap;
}

int16_t machine::get_num_toolbox_traps()
{
    return (NGetTrapAddress(_InitGraf, ToolTrap) == NGetTrapAddress(0xAA6E, ToolTrap)) ? 0x200 : 0x400;
}
