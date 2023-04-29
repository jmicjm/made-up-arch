#pragma once
#include "instruction.h"
#include "processor_state.h"
#include "instruction_registry.h"
#include "debugging/human_readable_instruction.h"


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
    REGISTER_INSTRUCTION_DESCRIBER_GENERIC(Opcode::andl, "and", Logical_instruction, void)

    void orl(Processor_state& state, Instruction_t instruction);
    REGISTER_INSTRUCTION(Opcode::orl, orl)
    REGISTER_INSTRUCTION_DESCRIBER_GENERIC(Opcode::orl, "or", Logical_instruction, void)

    void xorl(Processor_state& state, Instruction_t instruction);
    REGISTER_INSTRUCTION(Opcode::xorl, xorl)
    REGISTER_INSTRUCTION_DESCRIBER_GENERIC(Opcode::xorl, "xor", Logical_instruction, void)


    struct [[gnu::may_alias]] Logical_inv_instruction
    {
        uint32_t opcode : opcode_size = Opcode::invl;
        uint32_t rdst : register_size;
        uint32_t rsrc : register_size;
    };

    void invl(Processor_state& state, Instruction_t instruction);
    REGISTER_INSTRUCTION(Opcode::invl, invl)
    REGISTER_INSTRUCTION_DESCRIBER_GENERIC(Opcode::invl, "in", Logical_inv_instruction, void)

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
    REGISTER_INSTRUCTION_DESCRIBER(
        Opcode::shift,
        ([](Instruction_t instr) {
            static const char* types[] = { "ll", "lr", "al", "ar" };
            const auto i = instruction_cast<Shift_instruction>(instr);
            std::string desc = std::string{ "sh" } + types[i.type] + " r" + std::to_string(i.rdst) + " r" + std::to_string(i.r1);
            if (!i.imm_arg) desc += " r" + std::to_string(i.r2);
            else desc += std::to_string(instruction_cast<Shift_imm_instruction>(i).imm);

            return desc;
        })
    )

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
    REGISTER_INSTRUCTION_DESCRIBER(
        Opcode::rotate,
        ([](Instruction_t instr) {
            static const char* types[] = { "l", "r" };
            const auto i = instruction_cast<Rotate_instruction>(instr);
            std::string desc = std::string{ "rot" } + types[i.type] + " r" + std::to_string(i.rdst) + " r" + std::to_string(i.r1);
            if (!i.imm_arg) desc += " r" + std::to_string(i.r2);
            else desc += std::to_string(instruction_cast<Rotate_imm_instruction>(i).imm);

            return desc;
        })
    )
}