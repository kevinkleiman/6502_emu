#include <iostream>
#include "cpu.h"
#include "opcodes.h"

void h6502::CPU::reset(Mem& mem)
{
    PC = 0x101;    // set program counter to 0x101 temporarily
    SP = 0x100;     // set stack pointer to 0x100-0x01FF (page 0x01)
    mem.reset_mem();    // zero all memory
    CF = ZF = IF = DF = BF = OF = NF = 0;       // zero out flags
    A = X = Y = 0;      // zero out registers
}

void h6502::CPU::checkFlags(uint8_t reg)
{
    if (reg == 0) ZF = 1;
    if (0b10000000 & reg) NF = 1;
}

void h6502::CPU::setCompareFlags(uint8_t cmp, uint8_t reg)
{
    CF = (cmp <= reg) ? 1 : 0;
    ZF = (cmp == reg) ? 1 : 0;
}

void h6502::CPU::pushWord(Mem& mem, int& cycles, uint8_t data)
{
    mem.data[SP] = (data & 0xF);
    mem.data[SP - 1] = (data >> 4);
    SP -= 2;
    cycles -= 2;
}

void h6502::CPU::pushByte(Mem& mem, int& cycles, uint8_t data)
{
    mem.data[SP] = data;
    SP--;
    cycles -= 2;
}

uint8_t h6502::CPU::fetchZeroPage(Mem& mem, int& cycles, uint8_t zp_addr)
{
    PC++;
    cycles--;
    return mem.data[zp_addr];
}

uint8_t h6502::CPU::fetchByte(Mem& mem, int& cycles, uint32_t addr)
{
    PC++;
    cycles--;
    return mem.data[addr];
}

uint8_t h6502::CPU::popByte(Mem& mem, int& cycles)
{
    SP++;
    cycles -= 2;
    return mem.data[SP];
}

uint8_t h6502::CPU::zpRegAdd(int& cycles, uint8_t reg, uint8_t operand)
{
    cycles--;
    return reg + operand;
}

uint16_t h6502::CPU::fetchWord(Mem& mem, int& cycles)
{
    uint16_t word = mem.data[PC];
    PC++;

    word |= (mem.data[PC] << 8);    // concat two address bytes
    PC++;
    cycles -= 2;    // each byte takes a clock cycle, 2 byte address
    return word;
}

uint16_t h6502::CPU::popWord(Mem& mem, int& cycles)
{
    SP += 2;
    uint16_t word = mem.data[SP - 1] << 8 | mem.data[SP];
    mem.data[SP] = mem.data[SP - 1] = 0x0;
    cycles -= 2;
    return word;
}

void h6502::CPU::exec(int cycles, Mem& mem)
{
    while (cycles > 0)
    {
        uint8_t instruction = fetchByte(mem, cycles, PC);
        if (cycles <= 0) break;     // check cycles after instruction fetch cycle... gotta change later this seems dumb
                                    // used for 2 cycle implied instructions mainly
        switch (instruction)
        {
            case LDA_IM:
            {
                A = fetchByte(mem, cycles, PC);
                checkFlags(A);
            } break;

            case LDA_ZP:
            {
                A = fetchByte(mem, cycles, fetchZeroPage(mem, cycles, PC));
                checkFlags(A);
            } break;

            case LDA_AB:
            {
                A = fetchByte(mem, cycles, fetchWord(mem, cycles));
                checkFlags(A);
            } break;

            case LDA_ZP_X:
            {
                A = fetchByte(mem, cycles, zpRegAdd(cycles, X, fetchByte(mem, cycles, PC)));
                checkFlags(A);
            } break;

            case LDX_IM:
            {
                X = fetchByte(mem, cycles, PC);
                checkFlags(X);
            } break;

            case LDX_ZP:
            {
                X = fetchByte(mem, cycles, fetchZeroPage(mem, cycles, PC));
                checkFlags(X);
            } break;

            case LDX_AB:
            {
                X = fetchByte(mem, cycles, fetchWord(mem, cycles));
                checkFlags(X);
            } break;

            case LDX_ZP_Y:
            {
                X = fetchByte(mem, cycles, zpRegAdd(cycles, Y, fetchByte(mem, cycles, PC)));
                checkFlags(Y);
            } break;

            case LDY_IM:
            {
                Y = fetchByte(mem, cycles, PC);
                checkFlags(Y);
            } break;

            case LDY_ZP:
            {
                Y = fetchByte(mem, cycles, fetchZeroPage(mem, cycles, PC));
                checkFlags(Y);
            } break;

            case LDY_AB:
            {
                Y = fetchByte(mem, cycles, fetchWord(mem, cycles));
                checkFlags(Y);
            } break;

            case LDY_ZP_X:
            {
                Y = fetchByte(mem, cycles, zpRegAdd(cycles, X, fetchByte(mem, cycles, PC)));
            } break;

            case CPX_IM:
            {
                uint8_t cmp = fetchByte(mem, cycles, PC);
                setCompareFlags(cmp, X);
            } break;

            case CPX_ZP:
            {
                uint8_t cmp = fetchByte(mem, cycles, fetchByte(mem, cycles, PC));
                setCompareFlags(cmp, X);
            } break;

            case CPX_AB:
            {
                uint8_t cmp = fetchByte(mem, cycles, fetchWord(mem, cycles));
                setCompareFlags(cmp, X);
            } break;

            case CPY_IM:
            {
                uint8_t cmp = fetchByte(mem, cycles, PC);
                setCompareFlags(cmp, Y);
            } break;

            case CPY_ZP:
            {
                uint8_t cmp = fetchByte(mem, cycles, fetchByte(mem, cycles, PC));
                setCompareFlags(cmp, Y);
            } break;

            case CPY_AB:
            {
                uint8_t cmp = fetchByte(mem, cycles, fetchWord(mem, cycles));
                setCompareFlags(cmp, Y);
            } break;

            case DEX:
            {
                X--;
                cycles--;
                checkFlags(X);
            } break;

            case DEY:
            {
                Y--;
                cycles--;
                checkFlags(Y);
            } break;

            case INX:
            {
                X++;
                cycles--;
                checkFlags(X);
            } break;

            case INY:
            {
                Y++;
                cycles--;
                checkFlags(Y);
            } break;

            case JMP_AB:
            {
                uint16_t addr = fetchWord(mem, cycles);
                PC = addr;
            } break;

            case JSR_AB:
            {
                uint16_t addr = fetchWord(mem, cycles);
                pushWord(mem, cycles, PC - 1);
                PC = addr;
                cycles--;
            } break;

            case RTS:
            {
                uint16_t ret_addr = popWord(mem, cycles);
                PC = ret_addr;
                PC++;
                cycles--;
            } break;

            case PHA:
            {
                pushByte(mem, cycles, A);
            } break;

            case PLA:
            {
                A = popByte(mem, cycles);
                cycles--;
            } break;

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
    auto* cpu6502 = new h6502::CPU();

    cpu6502->reset(mem);
    mem.data[0x101] = LDA_IM;
    mem.data[0x101 + 1] = 0x90;
    mem.data[0x101 + 2] = PHA;
    mem.data[0x101 + 3] = LDA_IM;
    mem.data[0x101 + 4] = 0x79;
    mem.data[0x101 + 5] = PLA;

    cpu6502->exec(11, mem);
    delete cpu6502;

    return 0;
}
