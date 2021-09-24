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

    void reset(Mem& mem);
    void checkFlags(uint8_t reg);
    void setCompareFlags(uint8_t cmp, uint8_t reg);
    void pushByte(Mem &mem, int &cycles, uint8_t data);
    void pushWord(Mem &mem, int &cycles, uint8_t data);
    void exec(int cycles, Mem& mem);

    uint8_t fetchZeroPage(Mem& mem, int& cycles, uint8_t zp_addr);
    uint8_t fetchByte(Mem& mem, int& cycles, uint32_t addr);
    uint8_t popByte(Mem &mem, int &cycles);

    uint16_t fetchWord(Mem& mem, int& cycles);
    uint16_t popWord(Mem &mem, int &cycles);

    static uint8_t zpRegAdd(int &cycles, uint8_t reg, uint8_t operand);

};

