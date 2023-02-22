#include "comparison_instructions.h"
#include "common.h"


namespace emulator
{
    void cmp(Processor_state& state, Instruction_t instruction)
    {
        const auto instr = instruction_cast<Comparison_instruction>(instruction);

        const auto r1 = state.registers[instr.r1];
        const auto r2 = instr.imm_arg ? instruction_cast<Comparison_imm_instruction>(instruction).imm : state.registers[instr.r2];

        updateSubFlags(state, r1 - r2, r1, r2);
    }

    void tst(Processor_state& state, Instruction_t instruction)
    {
        const auto instr = instruction_cast<Comparison_instruction>(instruction);

        const auto r1 = state.registers[instr.r1];
        const auto r2 = state.registers[instr.r2];

        updateNZflags(state, r1 & r2);
    }
}