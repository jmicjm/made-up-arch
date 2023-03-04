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

    void andl(Processor_state& state, Instruction_t instruction);
    REGISTER_INSTRUCTION(Opcode::andl, andl)

    void orl(Processor_state& state, Instruction_t instruction);
    REGISTER_INSTRUCTION(Opcode::orl, orl)

    void xorl(Processor_state& state, Instruction_t instruction);
    REGISTER_INSTRUCTION(Opcode::xorl, xorl)

    struct [[gnu::may_alias]] Logical_inv_instruction
    {
        uint32_t opcode : opcode_size = Opcode::invl;
        uint32_t rdst : register_size;
        uint32_t rsrc : register_size;
    };

    void invl(Processor_state& state, Instruction_t instruction);
    REGISTER_INSTRUCTION(Opcode::invl, invl)

    enum Shift_type : uint8_t
    {
        logical_left,
        logical_right,
        arithmetic_left,
        arithmetic_right
    };

    struct [[gnu::may_alias]] Shift_instruction
    {
        uint32_t opcode : opcode_size = Opcode::shift;
        uint32_t type : 2;
        uint32_t imm_arg : 1 = 0;
        uint32_t rdst : register_size;
        uint32_t r1 : register_size;
        uint32_t r2 : register_size;
    };

    struct [[gnu::may_alias]] Shift_imm_instruction
    {
        uint32_t opcode : opcode_size = Opcode::shift;
        uint32_t type : 2;
        uint32_t imm_arg : 1 = 1;
        uint32_t rdst : register_size;
        uint32_t r1 : register_size;
        uint32_t imm : 6;
    };

    void shift(Processor_state& state, Instruction_t instruction);
    REGISTER_INSTRUCTION(Opcode::shift, shift)

    enum Rotate_type : uint8_t
    {
        left, right
    };

    struct [[gnu::may_alias]] Rotate_instruction
    {
        uint32_t opcode : opcode_size = Opcode::rotate;
        uint32_t type : 1;
        uint32_t imm_arg : 1 = 0;
        uint32_t rdst : register_size;
        uint32_t r1 : register_size;
        uint32_t r2 : register_size;
    };

    struct [[gnu::may_alias]] Rotate_imm_instruction
    {
        uint32_t opcode : opcode_size = Opcode::rotate;
        uint32_t type : 1;
        uint32_t imm_arg : 1 = 1;
        uint32_t rdst : register_size;
        uint32_t r1 : register_size;
        uint32_t imm : 6;
    };

    void rotate(Processor_state& state, Instruction_t instruction);
    REGISTER_INSTRUCTION(Opcode::rotate, rotate)
}