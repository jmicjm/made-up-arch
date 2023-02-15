#pragma once
#include "processor_state.h"


namespace emulator
{
    bool msb(auto value)
    {
        return value >> (sizeof(value) * 8 - 1);
    }

    void updateNZflags(Processor_state& state, auto rdst)
    {
        state.pswFields().negative = msb(rdst);
        state.pswFields().zero = rdst == 0;
    }
}