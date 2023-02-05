#pragma once
#include "instruction.h"
#include "processor_state.h"


namespace emulator
{   
    struct [[gnu::may_alias]] Arithmetic_instruction
    {
        uint64_t opcode : 8 = Opcode::invalid;
        uint64_t pad : 48;
        uint64_t r1 : 4;
        uint64_t r2 : 4;
    };


    void add(Processor_state& state, Instruction_t instruction);
    void sub(Processor_state& state, Instruction_t instruction);
}