#include <iostream>
#include "cpu.h"

void h6502::CPU::reset(Mem& mem)
{
    PC = 0x0000;    // set program counter to 0xFFFC ??
    SP = 0x100;     // set stack pointer to 0x100 via c64 wiki??
    mem.reset_mem();    // zero all memory
    CF = ZF = IF = DF = BF = OF = NF = 0;       // zero out flags
    A = X = Y = 0;      // zero out registers
}

void h6502::CPU::check_flags(uint8_t reg)
{
    if (reg == 0) ZF = 1;
    if (0b10000000 & reg) NF = 1;
}

void h6502::CPU::write_byte(Mem& mem, int& cycles, uint32_t addr, uint8_t data)
{
    PC++;
    cycles--;
    mem.data[addr] = data;
}

uint8_t h6502::CPU::fetch_zero_page(Mem& mem, int& cycles, uint8_t zp_addr)
{
    PC++;
    cycles--;
    return mem.data[zp_addr];
}

uint8_t h6502::CPU::fetch_byte(Mem& mem, int& cycles, uint32_t addr)
{
    PC++;
    cycles--;
    return mem.data[addr];
}

uint8_t h6502::CPU::zp_reg_add(int& cycles, uint8_t reg, uint8_t operand)
{
    cycles--;
    return reg + operand;
}

uint16_t h6502::CPU::fetch_word(Mem& mem, int& cycles)
{
    uint16_t word = mem.data[PC];
    PC++;

    word |= (mem.data[PC] << 8);    // concat two address bytes
    PC++;
    cycles -= 2;    // each byte takes a clock cycle, 2 byte address
    return word;
}

void h6502::CPU::exec(int cycles, Mem& mem)
{
    while (cycles > 0)
    {
        uint8_t instruction = fetch_byte(mem, cycles, PC);
        if (cycles == 0) break;     // check cycles after instruction fetch cycle... gotta change later this seems dumb
                                    // used for 2 cycle implied instructions mainly
        switch (instruction)
        {
            // LDA instruction set
            case LDA_IM:
            {
                A = fetch_byte(mem, cycles, PC);
                check_flags(A);
            } break;

            case LDA_ZP:
            {
                A = fetch_byte(mem, cycles, fetch_zero_page(mem, cycles, PC));
                check_flags(A);
            } break;

            case LDA_AB:
            {
                A = fetch_byte(mem, cycles, fetch_word(mem, cycles));
                check_flags(A);
            } break;

            case LDA_ZP_X:
            {
                A = fetch_byte(mem, cycles, zp_reg_add(cycles, X, fetch_byte(mem, cycles, PC)));
                check_flags(A);
            } break;

            // LDX instruction set
            case LDX_IM:
            {
                X = fetch_byte(mem, cycles, PC);
                check_flags(X);
            } break;

            case LDX_ZP:
            {
                X = fetch_byte(mem, cycles, fetch_zero_page(mem, cycles, PC));
                check_flags(X);
            } break;

            case LDX_AB:
            {
                X = fetch_byte(mem, cycles, fetch_word(mem, cycles));
                check_flags(X);
            } break;

            case LDX_ZP_Y:
            {
                X = fetch_byte(mem, cycles, zp_reg_add(cycles, Y, fetch_byte(mem, cycles, PC)));
                check_flags(Y);
            } break;


            // LDY instruction set
            case LDY_IM:
            {
                Y = fetch_byte(mem, cycles, PC);
                check_flags(Y);
            } break;

            case LDY_ZP:
            {
                Y = fetch_byte(mem, cycles, fetch_zero_page(mem, cycles, PC));
                check_flags(Y);
            } break;

            case LDY_AB:
            {
                Y = fetch_byte(mem, cycles, fetch_word(mem, cycles));
                check_flags(Y);
            } break;

            case LDY_ZP_X:
            {
                Y = fetch_byte(mem, cycles, zp_reg_add(cycles, X, fetch_byte(mem, cycles, PC)));
            } break;

            case CPX_IM:
            {
                uint8_t cmp = fetch_byte(mem, cycles, PC);
                CF = (cmp <= X) ? 1 : 0;
                ZF = (cmp == X) ? 1 : 0;
            } break;

            case CPX_ZP:
            {
                uint8_t cmp = fetch_byte(mem, cycles, fetch_byte(mem, cycles, PC));
                CF = (cmp <= X) ? 1 : 0;
                ZF = (cmp == X) ? 1 : 0;
            } break;

            case CPX_AB:
            {
                uint8_t cmp = fetch_byte(mem, cycles, fetch_word(mem, cycles));
                CF = (cmp <= X) ? 1 : 0;
                ZF = (cmp == X) ? 1 : 0;
            } break;

            case CPY_IM:
            {
                uint8_t cmp = fetch_byte(mem, cycles, PC);
                CF = (cmp <= Y) ? 1 : 0;
                ZF = (cmp == Y) ? 1 : 0;
            } break;

            case CPY_ZP:
            {
                uint8_t cmp = fetch_byte(mem, cycles, fetch_byte(mem, cycles, PC));
                CF = (cmp <= Y) ? 1 : 0;
                ZF = (cmp == Y) ? 1 : 0;
            } break;

            case CPY_AB:
            {
                uint8_t cmp = fetch_byte(mem, cycles, fetch_word(mem, cycles));
                CF = (cmp <= Y) ? 1 : 0;
                ZF = (cmp == Y) ? 1 : 0;
            } break;

            case DEX:
            {
                X--;
                cycles--;
                check_flags(X);
            } break;

            case DEY:
            {
                Y--;
                cycles--;
                check_flags(Y);
            } break;

            case INX:
            {
                X++;
                cycles--;
                check_flags(X);
            } break;

            case INY:
            {
                Y++;
                cycles--;
                check_flags(Y);
            } break;

            // NOP
            case NOP:
            {
                cycles--;
            } break;

            default:
            {
                std::cout << "CPU6502: Invalid Instruction" << std::endl;
            } return;
        }
    }
}

int main()
{
    Mem mem;
    h6502::CPU* cpu6502 = new h6502::CPU();

    cpu6502->reset(mem);
    mem.data[0x0000] = h6502::CPU::INX;
    mem.data[0x0000 + 1] = h6502::CPU::NOP;
    mem.data[0x0000 + 2] = h6502::CPU::NOP;
    mem.data[0x0000 + 3] = h6502::CPU::NOP;
    mem.data[0x0000 + 4] = h6502::CPU::LDA_IM;
    mem.data[0x0000 + 5] = 0x69;

    cpu6502->exec(10, mem);
    delete cpu6502;

    return 0;
}
