#pragma once

#include <iostream>
#include "memory.h"
#include "opcodes.h"

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

    void reset(Mem& mem);
    void check_flags(uint8_t reg);
    void set_compare_flags(uint8_t cmp, uint8_t reg);
    void push_word(Mem &mem, int &cycles, uint8_t data) const;
    void exec(int cycles, Mem& mem);

    uint8_t fetch_zero_page(Mem& mem, int& cycles, uint8_t zp_addr);
    uint8_t fetch_byte(Mem& mem, int& cycles, uint32_t addr);

    uint16_t fetch_word(Mem& mem, int& cycles);
    uint16_t pop_word(Mem &mem, int &cycles) const;

    static uint8_t zp_reg_add(int &cycles, uint8_t reg, uint8_t operand);

};

