#pragma once

#include <iostream>
#include "memory.h"

namespace h6502 {
    struct CPU;
    // TODO bus
}

struct h6502::CPU {
    // Registers
    uint16_t PC;    // program counter
    uint16_t SP;    // low 8 bits of of next free location [0x0100, 0x1FF]
    uint8_t A;      // accumulator
    uint8_t X;      // x register
    uint8_t Y;      // y register

    // Status Flags
    uint8_t CF: 1;     // carry flag
    uint8_t ZF: 1;     // zero flag
    uint8_t IF: 1;     // interrupt disable
    uint8_t DF: 1;     // decimal mode
    uint8_t BF: 1;     // break command
    uint8_t OF: 1;     // overflow flag
    uint8_t NF: 1;     // negative flag

    // Opcodes
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
            ORA_IM = 0x09,
            LSR_A = 0x4A,
            PHA = 0x48,
            PHP = 0x08,
            PLP = 0x68,
            NOP = 0xEA;

    void reset(Mem& mem);
    void check_flags(uint8_t reg);
    void write_byte(Mem& mem, int& cycles, uint32_t addr, uint8_t data);
    void exec(int cycles, Mem& mem);

    uint8_t fetch_zero_page(Mem& mem, int& cycles, uint8_t zp_addr);
    uint8_t fetch_byte(Mem& mem, int& cycles, uint32_t addr);

    uint16_t fetch_word(Mem& mem, int& cycles);

    static uint8_t zp_reg_add(int &cycles, uint8_t reg, uint8_t operand);
};

