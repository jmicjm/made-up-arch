#pragma once
#include "instruction.h"
#include "processor_state.h"
#include "instruction_registry.h"


namespace emulator
{
    struct [[gnu::may_alias]] Logical_instruction
    {
        uint32_t opcode : opcode_size = Opcode::invalid;
        uint32_t rdst : register_size;
        uint32_t r1 : register_size;
        uint32_t r2 : register_size;
    };

    struct [[gnu::may_alias]] Logical_inv_instruction
    {
        uint32_t opcode : opcode_size = Opcode::invl;
        uint32_t rdst : register_size;
        uint32_t rsrc : register_size;
    };

    void andl(Processor_state& state, Instruction_t instruction);
    REGISTER_INSTRUCTION(Opcode::andl, andl)

    void orl(Processor_state& state, Instruction_t instruction);
    REGISTER_INSTRUCTION(Opcode::orl, orl)

    void xorl(Processor_state& state, Instruction_t instruction);
    REGISTER_INSTRUCTION(Opcode::xorl, xorl)

    void invl(Processor_state& state, Instruction_t instruction);
    REGISTER_INSTRUCTION(Opcode::invl, invl)
}