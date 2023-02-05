#pragma once
#include "instruction.h"
#include "processor_state.h"
#include <functional>


namespace emulator
{
    struct [[gnu::may_alias]] Logical_instruction
    {
        Instruction_t opcode : opcode_size = Opcode::invalid;
        Instruction_t r1 : register_size;
        Instruction_t r2 : register_size;
    };

    struct [[gnu::may_alias]] Logical_inv_instruction
    {
        Instruction_t opcode : opcode_size = Opcode::invl;
        Instruction_t r : register_size;
    };

    template<typename op>
    void logicalInstruction(Processor_state& state, Instruction_t instruction)
    {
        const auto instr = instruction_cast<Logical_instruction>(instruction);

        auto& reg1 = state.registers[instr.r1];
        const auto reg2 = state.registers[instr.r2];

        reg1 = op{}(reg1, reg2);
    }

    const auto andl = logicalInstruction<std::bit_and<>>;
    const auto orl = logicalInstruction<std::bit_or<>>;
    const auto xorl = logicalInstruction<std::bit_xor<>>;

    void invl(Processor_state& state, Instruction_t instruction);
}