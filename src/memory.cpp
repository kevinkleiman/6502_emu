#include "iostream"
#include "memory.h"

// Zeros out all addresses in memory
void Mem::reset_mem()
{
    for (unsigned char & i : data)
    {
        i = 0x0;
    }
}


