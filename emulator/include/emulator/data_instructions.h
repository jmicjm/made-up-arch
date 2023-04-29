#pragma once
#include "instruction.h"
#include "processor_state.h"
#include "instruction_registry.h"
#include "debugging/human_readable_instruction.h"


namespace emulator
{
    struct [[gnu::may_alias]] Mov_instruction
    {
        uint32_t opcode : opcode_size = Opcode::mov;
        uint32_t imm_arg : 1 = false;
        uint32_t rdst : register_size;
        uint32_t rsrc : register_size;
    };

    struct [[gnu::may_alias]] Mov_imm_instruction
    {
        uint32_t opcode : opcode_size = Opcode::mov;
        uint32_t imm_arg : 1 = true;
        uint32_t rdst : register_size;
        int32_t imm : 19;
    }; 

    void mov(Processor_state& state, Instruction_t instruction);
    REGISTER_INSTRUCTION(Opcode::mov, mov)
    REGISTER_INSTRUCTION_DESCRIBER_GENERIC(Opcode::mov, "mov", Mov_instruction, Mov_imm_instruction)


    struct [[gnu::may_alias]] Ldr_instruction
    {
        uint32_t opcode : opcode_size = Opcode::ldr;
        uint32_t size : data_type_size = Data_type::octobyte;
        uint32_t rdst : register_size;
        uint32_t rbase : register_size;
        int32_t off : 14;
    };

    void ldr(Processor_state& state, Instruction_t instruction);
    REGISTER_INSTRUCTION(Opcode::ldr, ldr)
    REGISTER_INSTRUCTION_DESCRIBER_GENERIC(Opcode::ldr, "ldr", Ldr_instruction, void)


    struct [[gnu::may_alias]] Str_instruction
    {
        uint32_t opcode : opcode_size = Opcode::str;
        uint32_t size : data_type_size = Data_type::octobyte;
        uint32_t rsrc : register_size;
        uint32_t rbase : register_size;
        int32_t off : 14;
    };

    void str(Processor_state& state, Instruction_t instruction);
    REGISTER_INSTRUCTION(Opcode::str, str)
    REGISTER_INSTRUCTION_DESCRIBER_GENERIC(Opcode::str, "str", Str_instruction, void)

    struct [[gnu::may_alias]] Push_instruction
    {
        uint32_t opcode : opcode_size = Opcode::push;
        uint32_t size : data_type_size = Data_type::octobyte;
        uint32_t rsrc : register_size;
    };

    void push(Processor_state& state, Instruction_t instruction);
    REGISTER_INSTRUCTION(Opcode::push, push)
    REGISTER_INSTRUCTION_DESCRIBER_GENERIC(Opcode::push, "push", Push_instruction, void)

    struct [[gnu::may_alias]] Pop_instruction
    {
        uint32_t opcode : opcode_size = Opcode::pop;
        uint32_t size : data_type_size = Data_type::octobyte;
        uint32_t rdst : register_size;
    };

    void pop(Processor_state& state, Instruction_t instruction);
    REGISTER_INSTRUCTION(Opcode::pop, pop)
    REGISTER_INSTRUCTION_DESCRIBER_GENERIC(Opcode::pop, "pop", Pop_instruction, void)
}