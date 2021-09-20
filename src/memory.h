#pragma once

#define MAX 1024 * 64

struct Mem
{
    uint8_t data[MAX];       // should init all elements to 0x0

    // Zeros out all addresses in memory
    void reset_mem();
};