#pragma once
#include "instruction.h"
#include "processor_state.h"
#include "instruction_registry.h"
#include <functional>


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

    template<typename op>
    void logicalInstruction(Processor_state& state, Instruction_t instruction)
    {
        const auto instr = instruction_cast<Logical_instruction>(instruction);

        auto& rdst = state.registers[instr.rdst];
        const auto r1 = state.registers[instr.r1];
        const auto r2 = state.registers[instr.r2];

        rdst = op{}(r1, r2);
    }

    REGISTER_INSTRUCTION(Opcode::andl, logicalInstruction<std::bit_and<>>)
    REGISTER_INSTRUCTION(Opcode::orl, logicalInstruction<std::bit_or<>>)
    REGISTER_INSTRUCTION(Opcode::xorl, logicalInstruction<std::bit_xor<>>)

    void invl(Processor_state& state, Instruction_t instruction);
    REGISTER_INSTRUCTION(Opcode::invl, invl)
}