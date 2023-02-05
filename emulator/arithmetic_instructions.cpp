#include "arithmetic_instructions.h"

namespace emulator
{
    void add(Processor_state& state, Instruction_t instruction)
    {
        const auto instr = instruction_cast<Arithmetic_instruction>(instruction);

        auto& reg1 = state.registers[instr.r1];
        const auto reg2 = state.registers[instr.r2];

        reg1 += reg2;
    }

    void sub(Processor_state& state, Instruction_t instruction)
    {
        const auto instr = instruction_cast<Arithmetic_instruction>(instruction);

        auto& reg1 = state.registers[instr.r1];
        const auto reg2 = state.registers[instr.r2];

        reg1 -= reg2;
    }
}