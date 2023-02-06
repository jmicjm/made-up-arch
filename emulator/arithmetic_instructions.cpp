#include "arithmetic_instructions.h"

namespace emulator
{
    void add(Processor_state& state, Instruction_t instruction)
    {
        const auto instr = instruction_cast<Arithmetic_instruction>(instruction);

        auto& rdst = state.registers[instr.rdst];
        const auto r1 = state.registers[instr.r1];
        const auto r2 = state.registers[instr.r2];

        rdst = r1 + r2;
    }

    void sub(Processor_state& state, Instruction_t instruction)
    {
        const auto instr = instruction_cast<Arithmetic_instruction>(instruction);

        auto& rdst = state.registers[instr.rdst];
        const auto r1 = state.registers[instr.r1];
        const auto r2 = state.registers[instr.r2];

        rdst = r1 - r2;
    }
}