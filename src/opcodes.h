#pragma once

#include "iostream"
#include "cpu.h"

// Contains all opcodes... will refactor later
static constexpr uint8_t
        LDA_IM = 0XA9,
        LDA_ZP = 0xA5,
        LDA_AB = 0xAD,
        LDA_ZP_X = 0xB5,
        LDX_IM = 0xA2,
        LDX_ZP = 0xA6,
        LDX_AB = 0xAE,
        LDX_ZP_Y = 0xB6,
        LDY_IM = 0xA0,
        LDY_ZP = 0xA4,
        LDY_AB = 0xAC,
        LDY_ZP_X = 0xB4,
        CPX_IM = 0xE0,
        CPX_ZP = 0xE4,
        CPX_AB = 0xEC,
        CPY_IM = 0xC0,
        CPY_ZP = 0xC4,
        CPY_AB = 0xCC,
        DEX = 0xCA,
        DEY = 0x88,
        INX = 0xE8,
        INY = 0xC8,
        JMP_AB = 0x4C,
        JMP_IN = 0x6C,
        JSR_AB = 0x20,
        RTS = 0x60,
        ORA_IM = 0x09,
        LSR_A = 0x4A,
        PHA = 0x48,
        PHP = 0x08,
        PLP = 0x68,
        NOP = 0xEA;

void check_opcode(uint8_t opcode);