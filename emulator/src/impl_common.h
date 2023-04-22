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

    void updateSubFlags(Processor_state& state, auto rdst, auto r1, auto r2)
    {
        updateNZflags(state, rdst);
        state.pswFields().carry = r1 < r2;
        state.pswFields().overflow = msb(r1) != msb(r2) && msb(r1) != msb(rdst);
    }
}