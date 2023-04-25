#pragma once
#include <cstdint>


struct [[gnu::may_alias]] Interrupt_vector
{
    uint64_t reset;
    uint64_t invalid_opcode;
    uint64_t invalid_address;
    uint64_t timer0;
    uint64_t timer1;
};