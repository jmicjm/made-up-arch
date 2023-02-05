#pragma once
#include "instruction.h"
#include "processor_state.h"


namespace emulator
{
    struct [[gnu::may_alias]] Mov_instruction
    {
        uint64_t opcode : 8 = Opcode::mov;
        uint64_t imm_arg : 1 = false;
        uint64_t rdst : 4;
        uint64_t rsrc : 4;
    };

    struct [[gnu::may_alias]] Mov_imm_instruction
    {
        uint64_t opcode : 8 = Opcode::mov;
        uint64_t imm_arg : 1 = true;
        uint64_t rdst : 4;
        uint64_t imm : 51;
    };

    void mov(Processor_state& state, Instruction_t instruction);


    struct [[gnu::may_alias]] Ldr_instruction
    {
        uint64_t opcode : 8 = Opcode::ldr;
        uint64_t size : 2 = Data_type::octobyte;
        uint64_t rdst : 4; 
        uint64_t rbase : 4;
        uint64_t off : 46;
    };

    void ldr(Processor_state& state, Instruction_t instruction);

    struct [[gnu::may_alias]] Str_instruction
    {
        uint64_t opcode : 8 = Opcode::str;
        uint64_t size : 2 = Data_type::octobyte;
        uint64_t rsrc : 4;
        uint64_t rbase : 4;
        uint64_t off : 46;
    };

    void str(Processor_state& state, Instruction_t instruction);
}