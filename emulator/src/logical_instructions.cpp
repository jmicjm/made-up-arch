#include "logical_instructions.h"
#include "common.h"
#include <functional>


namespace emulator
{
    template<typename op>
    void logical3regInstruction(Processor_state& state, Instruction_t instruction)
    {
        const auto instr = instruction_cast<Logical_instruction>(instruction);

        auto& rdst = state.registers[instr.rdst];
        const auto r1 = state.registers[instr.r1];
        const auto r2 = state.registers[instr.r2];

        rdst = op{}(r1, r2);

        updateNZflags(state, rdst);
    }


    void andl(Processor_state& state, Instruction_t instruction)
    {
        logical3regInstruction<std::bit_and<>>(state, instruction);
    }

    void orl(Processor_state& state, Instruction_t instruction)
    {
        logical3regInstruction<std::bit_or<>>(state, instruction);
    }

    void xorl(Processor_state& state, Instruction_t instruction)
    {
        logical3regInstruction<std::bit_xor<>>(state, instruction);
    }

    void invl(Processor_state& state, Instruction_t instruction)
    {
        const auto instr = instruction_cast<Logical_inv_instruction>(instruction);
        auto& rdst = state.registers[instr.rdst];
        const auto rsrc = state.registers[instr.rsrc];

        rdst = ~rsrc;

        updateNZflags(state, rdst);
    }
}