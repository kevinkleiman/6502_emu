#pragma once

struct Mem
{
    static const uint32_t MAX_MEM = 1024 * 64;      // 64K of memory
    uint8_t data[MAX_MEM];       // should init all elements to 0x0

    // Zeros out all addresses in memory
    void reset_mem();
};