#pragma once
#include "instruction.h"
#include "processor_state.h"
#include "instruction_registry.h"


namespace emulator
{   
    struct [[gnu::may_alias]] Arithmetic_instruction
    {
        uint32_t opcode : opcode_size = Opcode::invalid;
        uint32_t rdst : register_size;
        uint32_t r1 : register_size;
        uint32_t r2 : register_size;
    };

    void add(Processor_state& state, Instruction_t instruction);
    REGISTER_INSTRUCTION(Opcode::add, add)
    void sub(Processor_state& state, Instruction_t instruction);
    REGISTER_INSTRUCTION(Opcode::sub, sub)
}