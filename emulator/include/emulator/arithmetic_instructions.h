#pragma once
#include "instruction.h"
#include "processor_state.h"
#include "instruction_registry.h"
#include "debugging/human_readable_instruction.h"


namespace emulator
{   
    struct [[gnu::may_alias]] Arithmetic_instruction
    {
        uint32_t opcode : opcode_size = Opcode::invalid;
        uint32_t imm_arg : 1 = 0;
        uint32_t rdst : register_size;
        uint32_t r1 : register_size;
        uint32_t r2 : register_size;
    };

    struct [[gnu::may_alias]] Arithmetic_imm_instruction
    {
        uint32_t opcode : opcode_size = Opcode::invalid;
        uint32_t imm_arg : 1 = 1;
        uint32_t rdst : register_size;
        uint32_t r1 : register_size;
        int32_t imm : 15;
    };

    void add(Processor_state& state, Instruction_t instruction);
    REGISTER_INSTRUCTION(Opcode::add, add)
    REGISTER_INSTRUCTION_DESCRIBER_GENERIC(Opcode::add, "add", Arithmetic_instruction, Arithmetic_imm_instruction)

    void sub(Processor_state& state, Instruction_t instruction);
    REGISTER_INSTRUCTION(Opcode::sub, sub)  
    REGISTER_INSTRUCTION_DESCRIBER_GENERIC(Opcode::sub, "sub", Arithmetic_instruction, Arithmetic_imm_instruction)
}