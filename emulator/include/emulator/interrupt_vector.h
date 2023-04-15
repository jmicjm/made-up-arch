#pragma once
#include <cstdint>


struct [[gnu::may_alias]] Interrupt_vector
{
    uint64_t reset;
    uint64_t invalid_opcode;
};