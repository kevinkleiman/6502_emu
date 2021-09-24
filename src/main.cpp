#include <iostream>
#include "cpu.h"
#include "opcodes.h"

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

