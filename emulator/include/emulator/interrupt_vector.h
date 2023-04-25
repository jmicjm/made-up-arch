#pragma once
#include <cstdint>


namespace emulator
{
    enum Interrupts
    {
        reset,
        invalid_opcode,
        invalid_address,
        timer0,
        timer1
    };

    struct [[gnu::may_alias]] Interrupt_vector
    {
        uint64_t handlers[5]{};
    };
}