#pragma once
#include "instruction.h"
#include "processor_state.h"
#include "instruction_registry.h"
#include "debugging/human_readable_instruction.h"


namespace emulator
{
    struct [[gnu::may_alias]] Comparison_instruction
    {
        uint32_t opcode : opcode_size = Opcode::invalid;
        uint32_t imm_arg : 1 = 0;
        uint32_t r1 : register_size;
        uint32_t r2 : register_size;
    };

    struct [[gnu::may_alias]] Comparison_imm_instruction
    {
        uint32_t opcode : opcode_size = Opcode::invalid;
        uint32_t imm_arg : 1 = 1;
        uint32_t r1 : register_size;
        int32_t imm : 15;
    };

    void cmp(Processor_state& state, Instruction_t instruction);
    REGISTER_INSTRUCTION(Opcode::cmp, cmp)
    REGISTER_INSTRUCTION_DESCRIBER_GENERIC(Opcode::cmp, "cmp", Comparison_instruction, Comparison_imm_instruction)

    void tst(Processor_state& state, Instruction_t instruction);
    REGISTER_INSTRUCTION(Opcode::tst, tst)
    REGISTER_INSTRUCTION_DESCRIBER_GENERIC(Opcode::tst, "tst", Comparison_instruction, void)
}