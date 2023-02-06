#pragma once
#include "instruction.h"
#include "processor_state.h"
#include "instruction_registry.h"


namespace emulator
{
    struct [[gnu::may_alias]] Mov_instruction
    {
        Instruction_t opcode : opcode_size = Opcode::mov;
        Instruction_t imm_arg : 1 = false;
        Instruction_t rdst : register_size;
        Instruction_t rsrc : register_size;
    };

    struct [[gnu::may_alias]] Mov_imm_instruction
    {
        Instruction_t opcode : opcode_size = Opcode::mov;
        Instruction_t imm_arg : 1 = true;
        Instruction_t rdst : register_size;
        Instruction_t imm : 19;
    };

    void mov(Processor_state& state, Instruction_t instruction);
    REGISTER_INSTRUCTION(Opcode::mov, mov)


    struct [[gnu::may_alias]] Ldr_instruction
    {
        Instruction_t opcode : opcode_size = Opcode::ldr;
        Instruction_t size : data_type_size = Data_type::octobyte;
        Instruction_t rdst : register_size;
        Instruction_t rbase : register_size;
        Instruction_t off : 14;
    };

    void ldr(Processor_state& state, Instruction_t instruction);
    REGISTER_INSTRUCTION(Opcode::ldr, ldr)


    struct [[gnu::may_alias]] Str_instruction
    {
        Instruction_t opcode : opcode_size = Opcode::str;
        Instruction_t size : data_type_size = Data_type::octobyte;
        Instruction_t rsrc : register_size;
        Instruction_t rbase : register_size;
        Instruction_t off : 14;
    };

    void str(Processor_state& state, Instruction_t instruction);
    REGISTER_INSTRUCTION(Opcode::str, str)
}