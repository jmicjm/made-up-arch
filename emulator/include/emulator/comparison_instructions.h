#pragma once
#include "instruction.h"
#include "processor_state.h"
#include "instruction_registry.h"


namespace emulator
{
    struct [[gnu::may_alias]] Comparison_instruction
    {
        uint32_t opcode : opcode_size = Opcode::invalid;
        uint32_t r1 : register_size;
        uint32_t r2 : register_size;
    };

    void cmp(Processor_state& state, Instruction_t instruction);
    REGISTER_INSTRUCTION(Opcode::cmp, cmp)

    void tst(Processor_state& state, Instruction_t instruction);
    REGISTER_INSTRUCTION(Opcode::tst, tst)
}